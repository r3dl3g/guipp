/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     edit control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_WIN
# include <windowsx.h>
#endif // GUIPP_WIN
#ifdef GUIPP_X11
# include <X11/cursorfont.h>
#endif // GUIPP_X11
# include <locale>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <gui/ctrl/edit.h>
#include <gui/win/clipboard.h>


//#define SHOW_TEXT_AREA

namespace gui {

  namespace ctrl {

    namespace filter {

      using namespace std::placeholders;

      typedef bool (is_fnkt)(std::string::value_type, const std::locale&);

      namespace {

        struct is_not_alpha {
          inline bool operator() (std::string::value_type i) {
            return !std::isalpha(i, std::locale::classic());
          }
        };

        struct is_not_alnum {
          inline bool operator() (std::string::value_type i) {
            return !std::isalnum(i, std::locale::classic());
          }
        };

        struct is_not_digit {
          inline bool operator() (std::string::value_type i) {
            return !std::isdigit(i, std::locale::classic());
          }
        };

        struct is_not_xdigit {
          inline bool operator() (std::string::value_type i) {
            return !std::isxdigit(i, std::locale::classic());
          }
        };

        struct is_not_lower {
          inline bool operator() (std::string::value_type i) {
            return !std::islower(i, std::locale::classic());
          }
        };

        struct is_not_upper {
          inline bool operator() (std::string::value_type i) {
            return !std::isupper(i, std::locale::classic());
          }
        };

        struct is_not_float {
          inline bool operator() (std::string::value_type i) {
            return !(std::isdigit(i, std::locale::classic()) || (i == '.') || (i == ',') || (i == '-'));
          }
        };

        struct is_not_integer {
          inline bool operator() (std::string::value_type i) {
            return std::isdigit(i, std::locale::classic()) || (i == '-');
          }
        };

      }

      std::string alpha_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_alpha()), t.end());
        return t;
      }

      std::string alpha_numeric_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_alnum()), t.end());
        return t;
      }

      std::string digits_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_digit()), t.end());
        return t;
      }

      std::string xdigits_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_xdigit()), t.end());
        return t;
      }

      std::string lower_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_lower()), t.end());
        return t;
      }

      std::string upper_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_upper()), t.end());
        return t;
      }

      std::string float_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_float()), t.end());
        return t;
      }

      std::string integer_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_integer()), t.end());
        return t;
      }

      std::string unsigned_filter (std::string t) {
        t.erase(std::remove_if(t.begin(), t.end(), is_not_digit()), t.end());
        return t;
      }
    }

    namespace detail {

      edit_base::data::data ()
        : cursor_pos(0)
        , text_limit(std::numeric_limits<pos_t>::max())
        , scroll_pos(0)
        , last_mouse_point(core::native_point::undefined)
        , insert_mode(false)
      {}

      edit_base::edit_base () {
        set_background(color::white);
      }

      edit_base::~edit_base () {
        if (is_valid()) {
          win::global::unregister_utf8_window(*this);
        }
      }

      void edit_base::create (win::container& parent,
                              const std::string& txt,
                              const core::rectangle& place) {
        create(parent, place);
        set_text(txt);
      }

      void edit_base::create (win::container& parent,
                              const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
        prepare_input();
      }

      void edit_base::init_text (const std::string& t) {
        init();
        set_text(t);
      }

      void edit_base::init () {
        data.cursor_pos = 0;
        data.scroll_pos = 0;
        data.selection.clear();
        notify_content_changed();
        invalidate();
      }

      void edit_base::set_text_filter (text_filter f) {
        data.filter = f;
      }

      std::string edit_base::filter_text (const std::string& t) const {
        if (data.filter) {
          return data.filter(t);
        } else {
          return t;
        }
      }

      void edit_base::prepare_input () {
        win::global::register_utf8_window(*this);
      }

      void edit_base::set_selection (const edit_base::range& sel, event_source) {
        if (sel != data.selection) {
          data.selection = sel;
          invalidate();
          notify_selection_changed(event_source::logic);
        }
      }

      edit_base::range edit_base::get_selection () const {
        if (data.selection.empty() && is_insert_mode()) {
          return range(get_cursor_pos(), get_cursor_pos() + 1);
        }
        return data.selection;
      }

      void edit_base::set_cursor_pos (pos_t pos, bool shift) {
        pos_t new_pos = std::min(pos, get_text_length());
        auto new_sel = data.selection;
        if (shift) {
          if (data.cursor_pos == new_sel.end()) {
            new_sel = {new_sel.begin(), new_pos};
          } else if (data.cursor_pos == new_sel.begin()) {
            new_sel = {new_pos, new_sel.end()};
          } else {
            new_sel = range(new_pos);
          }
          new_sel.sort();
        } else {
          new_sel = range(new_pos);
        }
        data.cursor_pos = new_pos;

        set_selection(new_sel, event_source::logic);
        make_selection_visible(true);
      }

      void edit_base::make_selection_visible (bool update) {
        // make cursor pos visible
        auto old_pos = data.scroll_pos;
        if (data.cursor_pos < data.scroll_pos) {
          data.scroll_pos = 0;
        }

        const auto text = get_text();
        core::size sz = draw::font::system().get_text_size(text.substr(data.scroll_pos, data.cursor_pos - data.scroll_pos));
        core::size max_sz = client_size();
        max_sz -= {6, 4};
        while ((sz.width() > max_sz.width()) && (data.scroll_pos < data.cursor_pos)) {
          ++(data.scroll_pos);
          sz = draw::font::system().get_text_size(text.substr(data.scroll_pos, data.cursor_pos - data.scroll_pos));
        }

        if (update || (old_pos != data.scroll_pos)) {
          invalidate();
        }
      }

      edit_base::pos_t edit_base::get_cursor_pos () const {
        return data.cursor_pos;
      }

      edit_base::pos_t edit_base::get_scroll_pos () const {
        return data.scroll_pos;
      }

      void edit_base::set_text_limit (pos_t max_chars) {
        data.text_limit = max_chars;
      }

      edit_base::pos_t edit_base::get_text_limit () const {
        return data.text_limit;
      }

      edit_base::pos_t edit_base::get_text_length () const {
        return (pos_t)get_text().size();
      }

      bool edit_base::is_insert_mode () const {
        return data.insert_mode;
      }

      void edit_base::set_insert_mode (bool b) {
        data.insert_mode = b;
      }

      void edit_base::replace_selection (const std::string& new_text) {
        range sel = get_selection();
        std::string text = get_text();
        text.replace(sel.begin(), sel.end() - sel.begin(), new_text);
        set_text(filter_text(text));
        set_cursor_pos(sel.begin() + new_text.size(), false);
        notify_content_changed();
        invalidate();
      }

      std::string edit_base::get_text_in_range (const edit_base::range& r) const {
        if (r.empty()) {
          return std::string();
        }
        return get_text().substr(r.begin(), r.end() - r.begin());
      }

      std::string edit_base::get_selected_text () const {
        return get_text_in_range(get_selection());
      }

      edit_base::pos_t edit_base::get_position_at_point (const core::point& pt) const {
        const auto text = get_text();
        pos_t max_chars = text.size();
        for (pos_t i = data.scroll_pos + 1; i < max_chars; ++i) {
          core::size sz = draw::font::system().get_text_size(text.substr(data.scroll_pos, i - data.scroll_pos));
          if (sz.width() >= pt.x()) {
            return i - 1;
          }
        }
        return max_chars;
      }

      void edit_base::handle_key (os::key_state keystate,
                                  os::key_symbol keycode,
                                  const std::string& chars) {
        bool shift = core::shift_key_bit_mask::is_set(keystate);
        bool ctrl = core::control_key_bit_mask::is_set(keystate);

        switch (keycode) {
        case core::keys::left:
        case core::keys::numpad::left:
          if (ctrl) {
            // next word begin
            if (data.cursor_pos > 1) {
              std::string::size_type pos = util::string::find_left_space(get_text(), data.cursor_pos);
              if (pos != std::string::npos) {
                set_cursor_pos(pos, shift);
                return;
              }
            }
          } else if (data.cursor_pos > 0) {
            set_cursor_pos(util::utf8::get_left_char(get_text(), data.cursor_pos), shift);
            return;
          }
          break;
        case core::keys::right:
        case core::keys::numpad::right:
          if (ctrl) {
            set_cursor_pos(util::string::find_right_space(get_text(), data.cursor_pos), shift);
          } else if (data.cursor_pos < get_text_length()) {
            set_cursor_pos(util::utf8::get_right_char(get_text(), data.cursor_pos), shift);
          }
          break;
        case core::keys::home:
        case core::keys::numpad::home:
          set_cursor_pos(0, shift);
          break;
        case core::keys::end:
        case core::keys::numpad::end:
          set_cursor_pos(get_text_length(), shift);
          break;
        case core::keys::del:
        case core::keys::numpad::del:
          if (data.selection.empty()) {
            std::size_t cp = data.cursor_pos + 1;
            auto text = get_text();
            while ((cp < get_text_length()) && util::utf8::is_continuation_char(text.at(cp))) {
              ++cp;
            }
            text.replace(data.cursor_pos, cp - data.cursor_pos, std::string());
            set_text(filter_text(text));
            invalidate();
            notify_content_changed();
          } else {
            replace_selection(std::string());
            set_cursor_pos(data.selection.begin(), false);
          }
          break;
        case core::keys::back_space:
          if (data.selection.empty()) {
            if (data.cursor_pos > 0) {
              std::size_t cp = data.cursor_pos - 1;
              auto text = get_text();
              while ((cp > 0) && util::utf8::is_continuation_char(text.at(cp))) {
                --cp;
              }
              text.replace(cp, data.cursor_pos - cp, std::string());
              set_text(filter_text(text));
              set_cursor_pos(cp, false);
              notify_content_changed();
            }
          } else {
            replace_selection(std::string());
            set_cursor_pos(data.selection.begin(), false);
          }
          break;
        case core::keys::escape:
          set_selection(range(), event_source::keyboard);
          notify_selection_cancel();
          break;
        case core::keys::clear:
          set_selection(range(0, get_text().size()), event_source::keyboard);
          replace_selection(std::string());
          set_cursor_pos(0, false);
          break;
        case core::keys::tab:
          break;
        case core::keys::enter:
          notify_selection_commit();
          break;
        case core::keys::insert:
          set_insert_mode(!is_insert_mode());
          invalidate();
          break;
        default: {
          if (ctrl) {
            switch (keycode) {
            case core::keys::a:
              // select all
              set_selection(range(0, get_text().size()), event_source::keyboard);
              break;
            case core::keys::v: {
              win::clipboard::get().get_text(*this, [&](const std::string & t) {
                replace_selection(t);
              });
              break;
            case core::keys::c:
              win::clipboard::get().set_text(*this, get_selected_text());
              break;
            case core::keys::x:
              win::clipboard::get().set_text(*this, get_selected_text());
              replace_selection(std::string());
              break;
            }
            default:
              logging::debug() << "Key Ctrl + 0x" << std::hex << keycode;
              break;
            }
          } else if (!chars.empty()) {
            replace_selection(chars);
            set_cursor_pos(data.selection.end(), false);
          }
        }
        }
      }

      void edit_base::register_handler () {
        on_any_key_down(util::bind_method(this, &edit_base::handle_key));
        on_left_btn_down([&](os::key_state state, const core::native_point& pt) {
          take_focus();
          data.last_mouse_point = pt;
          set_cursor_pos(get_position_at_point(surface_to_client(pt)), core::shift_key_bit_mask::is_set(state));
        });
        on_left_btn_up([&](os::key_state, const core::native_point&) {
          data.last_mouse_point = core::native_point::undefined;
        });
        on_left_btn_dblclk([&](os::key_state, const core::native_point& pt) {
          take_focus();
          data.last_mouse_point = pt;
          pos_t p = get_position_at_point(surface_to_client(pt));
          set_cursor_pos(p);
          const auto text = get_text();
          pos_t l = util::string::find_left_space(text, p);
          pos_t r = util::string::find_right_space(text, p);
          set_selection(range(l, r), event_source::mouse);
        });
        on_mouse_move([&](os::key_state keys, const core::native_point& pt) {
          if ((data.last_mouse_point != core::native_point::undefined) && core::left_button_bit_mask::is_set(keys)) {
            set_cursor_pos(get_position_at_point(surface_to_client(pt)), true);
          }
        });
      }

    } // detail

  } // ctrl

} // gui

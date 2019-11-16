/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: edit control
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef WIN32
# include <windowsx.h>
# include <locale>
#endif // WIN32
#ifdef X11
# include <X11/cursorfont.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/clipboard.h>


//#define SHOW_TEXT_AREA

namespace gui {

  namespace ctrl {

    namespace paint {

      void edit_line (const draw::graphics& graph,
                      const core::rectangle& area,
                      const std::string& text,
                      const draw::font& fnt,
                      os::color foreground,
                      os::color background,
                      const text_origin origin,
                      const core::range<size_t>& selection,
                      const size_t cursor_pos,
                      const size_t scroll_pos,
                      const bool has_focus) {

        using namespace gui::draw;
        clip clp(graph, area);

        graph.fill(rectangle(area), background);

        core::point::type y1 = area.y();
        core::point::type y2 = area.y2();

        if (has_focus && (selection.begin() < selection.end())) {
          gui::core::rectangle a1 = area;
          auto begin = scroll_pos;
          core::point::type left = area.x();
          if (selection.begin() > scroll_pos) {
            std::string t1 = text.substr(scroll_pos, selection.begin() - scroll_pos);
            begin = selection.begin();
            graph.text(bounding_box(t1, a1, origin), fnt, foreground);
            left = a1.x2();
            graph.text(text_box(t1, core::rectangle(core::point(area.x(), y1), core::point(left, y2)), origin),
                       fnt, foreground);
          } else {
            a1.x2(a1.x());
          }

          gui::core::rectangle a2 = area.with_x(left);
          if (selection.end() > scroll_pos) {
            std::string t2 = text.substr(begin, selection.end() - begin);
            graph.text(bounding_box(t2, a2, origin), fnt, foreground);
            left = a2.x2();
            a2 = core::rectangle(core::point(a2.x(), y1), core::point(left, y2));
            graph.fill(rectangle(a2), color::highLightColor());
            graph.text(text_box(t2, core::rectangle(a2), origin), fnt, color::highLightTextColor());
            begin = selection.end();
          }

          if (begin < text.length()) {
            std::string t3 = text.substr(begin);
            graph.text(text_box(t3, core::rectangle(core::point(left, y1), core::point(area.x2(), y2)), origin),
                       fnt, foreground);
          }

        } else {
          graph.text(text_box(text.substr(scroll_pos), area, origin), fnt, foreground);
        }

        if (has_focus && (cursor_pos < std::numeric_limits<detail::edit_base::pos_t>::max())) {
          gui::core::rectangle cursor_area = area;
          graph.text(bounding_box(text.substr(scroll_pos, cursor_pos - scroll_pos), cursor_area, origin),
                     fnt, foreground);
          graph.frame(line(core::point(cursor_area.x2(), y1),
                           core::point(cursor_area.x2(), y2)),
                      foreground);
        }
#ifdef SHOW_TEXT_AREA
        graph.text(bounding_box(text, area, origin), fnt, foreground);
        graph.frame(rectangle(area), pen(foreground, pen::dot));
#endif // SHOW_TEXT_AREA
      }

    } // paint

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
        , last_mouse_point(core::point::undefined)
      {}

      edit_base::edit_base () {
        init();
      }

      edit_base::edit_base (const edit_base& rhs)
        : super(rhs)
        , data(rhs.data)
      {
        init();
      }

      edit_base::edit_base (edit_base&& rhs)
        : super(std::move(rhs))
        , data(std::move(rhs.data))
      {
        init();
      }

      void edit_base::init () {
        set_accept_focus(true);
      }

      edit_base::~edit_base () {
        win::global::unregister_utf8_window(get_id());
      }

      void edit_base::create (const win::container& parent,
                              const std::string& txt,
                              const core::rectangle& place) {
        create(parent, place);
        set_text(txt);
      }

      void edit_base::create (const win::container& parent,
                              const core::rectangle& place) {
        super::create(clazz<>::get(), parent, place);
        prepare_input();
      }

      void edit_base::set_text (const std::string& t) {
        data.text = t;
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

      const std::string& edit_base::get_text () const {
        return data.text;
      }

      void edit_base::prepare_input () {
        win::global::register_utf8_window(get_id());
      }

      void edit_base::set_selection (const edit_base::range& sel, event_source) {
        data.selection = sel;
        invalidate();
      }

      edit_base::range edit_base::get_selection () const {
        return data.selection;
      }

      void edit_base::set_cursor_pos (pos_t pos, bool shift) {
        pos_t new_pos = std::min(pos, get_text_length());
        if (shift) {
          if (data.cursor_pos == data.selection.end()) {
            data.selection = {data.selection.begin(), new_pos};
          } else if (data.cursor_pos == data.selection.begin()) {
            data.selection = {new_pos, data.selection.end()};
          } else {
            data.selection = range(new_pos);
          }
          data.selection.sort();
        } else {
          data.selection = range(new_pos);
        }
        data.cursor_pos = new_pos;

        make_selection_visible(true);
      }

      void edit_base::make_selection_visible (bool update) {
        // make cursor pos visible
        auto old_pos = data.scroll_pos;
        if (data.cursor_pos < data.scroll_pos) {
          data.scroll_pos = 0;
        }

        core::size sz = draw::font::system().get_text_size(data.text.substr(data.scroll_pos, data.cursor_pos - data.scroll_pos));
        core::size max_sz = client_size();
        max_sz -= {6, 4};
        while ((sz.width() > max_sz.width()) && (data.scroll_pos < data.cursor_pos)) {
          ++(data.scroll_pos);
          sz = draw::font::system().get_text_size(data.text.substr(data.scroll_pos, data.cursor_pos - data.scroll_pos));
        }

        if (update || (old_pos != data.scroll_pos)) {
          invalidate();
        }
      }

      edit_base::pos_t edit_base::get_cursor_pos () const {
        return data.cursor_pos;
      }

      void edit_base::set_text_limit (pos_t max_chars) {
        data.text_limit = max_chars;
      }

      edit_base::pos_t edit_base::get_text_limit () const {
        return data.text_limit;
      }

      edit_base::pos_t edit_base::get_text_length () const {
        return (pos_t)data.text.size();
      }

      void edit_base::replace_selection (const std::string& text) {
        range sel = get_selection();
        std::string new_text = filter_text(text);
        data.text.replace(sel.begin(), sel.end() - sel.begin(), new_text);
        set_cursor_pos(sel.begin() + new_text.size(), false);
        notify_content_changed();
        invalidate();
      }

      std::string edit_base::get_text_in_range (const edit_base::range& r) const {
        if (r.empty()) {
          return std::string();
        }
        return data.text.substr(r.begin(), r.end() - r.begin());
      }

      std::string edit_base::get_selected_text () const {
        return get_text_in_range(get_selection());
      }

      edit_base::pos_t edit_base::get_position_at_point (const core::point& pt) {
        pos_t max_chars = data.text.size();
        for (pos_t i = data.scroll_pos + 1; i < max_chars; ++i) {
          core::size sz = draw::font::system().get_text_size(data.text.substr(data.scroll_pos, i - data.scroll_pos));
          if (sz.width() >= pt.x()) {
            return i - 1;
          }
        }
        return max_chars;
      }

      void edit_base::notify_content_changed () const {
        send_client_message(this, detail::CONTENT_CHANGED_MESSAGE);
      }

      void edit_base::handle_key (os::key_state keystate,
                                  os::key_symbol keycode,
                                  const std::string& chars) {
        bool shift = win::shift_key_bit_mask::is_set(keystate);
        bool ctrl = win::control_key_bit_mask::is_set(keystate);

        switch (keycode) {
        case win::keys::left:
        case win::keys::numpad::left:
          if (ctrl) {
            // next word begin
            if (data.cursor_pos > 1) {
              std::string::size_type pos = util::string::find_left_space(data.text, data.cursor_pos);
              if (pos != std::string::npos) {
                set_cursor_pos(pos, shift);
                return;
              }
            }
          } else if (data.cursor_pos > 0) {
            set_cursor_pos(util::utf8::get_left_char(data.text, data.cursor_pos), shift);
            return;
          }
          break;
        case win::keys::right:
        case win::keys::numpad::right:
          if (ctrl) {
            set_cursor_pos(util::string::find_right_space(data.text, data.cursor_pos), shift);
          } else if (data.cursor_pos < get_text_length()) {
            set_cursor_pos(util::utf8::get_right_char(data.text, data.cursor_pos), shift);
          }
          break;
        case win::keys::home:
        case win::keys::numpad::home:
          set_cursor_pos(0, shift);
          break;
        case win::keys::end:
        case win::keys::numpad::end:
          set_cursor_pos(get_text_length(), shift);
          break;
        case win::keys::del:
        case win::keys::numpad::del:
          if (data.selection.empty()) {
            std::size_t cp = data.cursor_pos + 1;
            while ((cp < get_text_length()) && util::utf8::is_continuation_char(data.text.at(cp))) {
              ++cp;
            }
            data.text.replace(data.cursor_pos, cp - data.cursor_pos, std::string());
            invalidate();
            notify_content_changed();
          } else {
            replace_selection(std::string());
            set_cursor_pos(data.selection.begin(), false);
          }
          break;
        case win::keys::back_space:
          if (data.selection.empty()) {
            if (data.cursor_pos > 0) {
              std::size_t cp = data.cursor_pos - 1;
              while ((cp > 0) && util::utf8::is_continuation_char(data.text.at(cp))) {
                --cp;
              }
              data.text.replace(cp, data.cursor_pos - cp, std::string());
              set_cursor_pos(cp, false);
              notify_content_changed();
            }
          } else {
            replace_selection(std::string());
            set_cursor_pos(data.selection.begin(), false);
          }
          break;
        case win::keys::escape:
          set_selection(range(), event_source::keyboard);
          send_client_message(this, detail::SELECTION_CANCEL_MESSAGE);
          break;
        case win::keys::clear:
          set_selection(range(0, data.text.size()), event_source::keyboard);
          replace_selection(std::string());
          set_cursor_pos(0, false);
          break;
        case win::keys::tab:
          break;
        case win::keys::enter:
          send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
          break;
        default: {
          if (ctrl) {
            switch (keycode) {
            case win::keys::a:
              // select all
              set_selection(range(0, data.text.size()), event_source::keyboard);
              break;
            case win::keys::v: {
              win::clipboard::get().get_text(*this, [&](const std::string & t) {
                replace_selection(t);
              });
              break;
            case win::keys::c:
              win::clipboard::get().set_text(*this, get_selected_text());
              break;
            case win::keys::x:
              win::clipboard::get().set_text(*this, get_selected_text());
              replace_selection(std::string());
              break;
            }
            default:
              LogDebug << "Key Ctrl + 0x" << std::hex << keycode;
              break;
            }
          } else if (chars.size()) {
            replace_selection(chars);
            set_cursor_pos(data.selection.end(), false);
          }
        }
        }
      }

      void edit_base::register_handler () {
        on_any_key_down(util::bind_method(this, &edit_base::handle_key));
        on_left_btn_down([&](os::key_state state, const core::point& pt) {
          take_focus();
          data.last_mouse_point = pt;
          set_cursor_pos(get_position_at_point(pt), win::shift_key_bit_mask::is_set(state));
        });
        on_left_btn_up([&](os::key_state, const core::point&) {
          data.last_mouse_point = core::point::undefined;
        });
        on_left_btn_dblclk([&](os::key_state, const core::point& pt) {
          take_focus();
          data.last_mouse_point = pt;
          pos_t p = get_position_at_point(pt);
          set_cursor_pos(p);
          pos_t l = util::string::find_left_space(data.text, p);
          pos_t r = util::string::find_right_space(data.text, p);
          set_selection(range(l, r), event_source::mouse);
        });
        on_mouse_move([&](os::key_state keys, const core::point& pt) {
          if ((data.last_mouse_point != core::point::undefined) && win::left_button_bit_mask::is_set(keys)) {
            set_cursor_pos(get_position_at_point(pt), true);
          }
        });
      }

    } // detail

  } // ctrl

} // gui

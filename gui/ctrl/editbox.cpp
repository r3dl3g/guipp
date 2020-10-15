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
 * @brief     C++ API: textbox controls
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <gui/ctrl/editbox.h>
#include <gui/ctrl/edit.h>
#include <gui/win/clipboard.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      editbox_base::editbox_base () {
        init();
      }

      editbox_base::~editbox_base () {
        win::global::unregister_utf8_window(get_id());
      }

      void editbox_base::init () {
        data.lines.emplace_back(std::string());
        set_accept_focus(true);
        on_create(util::bind_method(this, &editbox_base::handle_create));
        on_any_key_down(util::bind_method(this, &editbox_base::handle_key));
      }

      void editbox_base::create (const win::container& parent,
                                 const core::rectangle& r) {
        create(clazz::get(), parent, r);
      }

      void editbox_base::handle_create (window*, const core::rectangle&) {
        win::global::register_utf8_window(get_id());
      }

      std::string& editbox_base::current_line () {
        return data.lines[std::max(0, std::min < int > (data.cursor_pos.y(), static_cast<int>(data.lines.size())))];
      }

      void editbox_base::handle_key (os::key_state keystate,
                                     os::key_symbol keycode,
                                     const std::string& chars) {
        bool shift = win::shift_key_bit_mask::is_set(keystate);
        bool ctrl = win::control_key_bit_mask::is_set(keystate);
        std::string& current = current_line();

        switch (keycode) {
        case win::keys::left:
        case win::keys::numpad::left:
          if (ctrl) {
            // next word begin
            set_cursor_pos(find_prev_word(data.cursor_pos), shift);
          } else if (data.cursor_pos.x() > 0) {
            set_cursor_pos({static_cast<decltype(data.cursor_pos.x())>(util::utf8::get_left_char(current, data.cursor_pos.x())), data.cursor_pos.y()}, shift);
            return;
          } else if (data.cursor_pos.y() > 0) {
            auto row = data.cursor_pos.y() - 1;
            set_cursor_pos({position::end.x(), row}, shift);
          }
          break;
        case win::keys::right:
        case win::keys::numpad::right:
          if (ctrl) {
            set_cursor_pos(find_next_word(data.cursor_pos), shift);
          } else if (data.cursor_pos.x() < current.size()) {
            set_cursor_pos({static_cast<decltype(data.cursor_pos.x())>(util::utf8::get_right_char(current, data.cursor_pos.x())), data.cursor_pos.y()}, shift);
          } else if (data.cursor_pos.y() < (row_count() - 1)) {
            set_cursor_pos({0, data.cursor_pos.y() + 1}, shift);
          }
          break;
        case win::keys::up:
        case win::keys::numpad::up:
          if (ctrl) {} else if (data.cursor_pos.y() > 0) {
            set_cursor_pos({data.cursor_pos.x(), data.cursor_pos.y() - 1}, shift);
          }
          break;
        case win::keys::down:
        case win::keys::numpad::down:
          if (ctrl) {} else if (data.cursor_pos.y() < (row_count() - 1)) {
            set_cursor_pos({data.cursor_pos.x(), data.cursor_pos.y() + 1}, shift);
          }
          break;
        case win::keys::home:
        case win::keys::numpad::home:
          if (ctrl) {
            set_cursor_pos(position::zero, shift);
          } else {
            set_cursor_pos({0, data.cursor_pos.y()}, shift);
          }
          break;
        case win::keys::end:
        case win::keys::numpad::end:
          if (ctrl) {
            set_cursor_pos(position::end, shift);
          } else {
            set_cursor_pos({position::end.x(), data.cursor_pos.y()}, shift);
          }
          break;
        case win::keys::del:
        case win::keys::numpad::del:
          if (data.selection.empty()) {
            if (current.empty()) {
              erase_line(data.cursor_pos.y());
            } else if (data.cursor_pos.x() < current.size()) {
              std::size_t cp = data.cursor_pos.x() + 1;
              while ((cp < current.size()) && util::utf8::is_continuation_char(current.at(cp))) {
                ++cp;
              }
              current.replace(data.cursor_pos.x(), cp - data.cursor_pos.x(), std::string());
              notify_content_changed();
            } else if (data.cursor_pos.y() < (row_count() - 1)) {
              current.append(data.lines[data.cursor_pos.y() + 1]);
              erase_line(data.cursor_pos.y() + 1);
            }
          } else {
            replace_selection(std::string());
            set_cursor_pos(data.selection.begin(), false);
          }
          break;
        case win::keys::back_space:
          if (data.selection.empty()) {
            if (current.empty()) {
              erase_line(data.cursor_pos.y());
              set_cursor_pos({position::end.x(), data.cursor_pos.y() - 1}, false);
            } else if (data.cursor_pos.x() > 0) {
              std::size_t cp = data.cursor_pos.x() - 1;
              while ((cp > 0) && util::utf8::is_continuation_char(current.at(cp))) {
                --cp;
              }
              current.replace(cp, data.cursor_pos.x() - cp, std::string());
              notify_content_changed();
              set_cursor_pos({static_cast<decltype(data.cursor_pos.x())>(cp), data.cursor_pos.y()}, false);
            } else if (data.cursor_pos.y() > 0) {
              auto row = data.cursor_pos.y() - 1;
              std::string& prev = data.lines[row];
              position pos(static_cast<position::type>(prev.size()), row);
              prev.append(current);
              erase_line(data.cursor_pos.y());
              set_cursor_pos(pos, false);
              invalidate();
            }
          } else {
            replace_selection(std::string());
            set_cursor_pos(data.selection.begin(), false);
          }
          break;
        case win::keys::escape:
          set_selection({});
          break;
        case win::keys::clear:
          set_selection({position::zero, position::end});
          replace_selection(std::string());
          set_cursor_pos(position::zero, false);
          break;
        case win::keys::tab:
          break;
        case win::keys::enter: {
          if (!data.selection.empty()) {
            replace_selection(std::string());
            set_cursor_pos(data.selection.begin(), false);
          }
          auto rest = current.substr(data.cursor_pos.x());
          current.erase(data.cursor_pos.x());
          auto row = data.cursor_pos.y() + 1;
          data.lines.insert(std::next(data.lines.begin(), row), rest);
          notify_content_changed();
          set_cursor_pos({0, row}, false);
          break;
        }
        default: {
          if (ctrl) {
            switch (keycode) {
            case win::keys::a:
              // select all
              set_selection({position::zero, position::end});
              break;
            case win::keys::v: {
              win::clipboard::get().get_text(*this, [&](const std::string & t) {
                                          replace_selection(t);
                                        });
              break;
            case win::keys::x:
              win::clipboard::get().set_text(*this, get_selected_text());
              replace_selection(std::string());
              break;
            }
            default:
              clog::debug() << "Key Ctrl + 0x" << std::hex << keycode;
              break;
            }
          } else if (chars.size()) {
            replace_selection(chars);
            set_cursor_pos(data.selection.end(), false);
          }
        }
        }
      }

      // --------------------------------------------------------------------------

    } // namespace detail

  } // ctrl

} // gui

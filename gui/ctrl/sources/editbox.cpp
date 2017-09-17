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
// Common includes
//
#include <string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "editbox.h"
#include "edit.h"
#include "clipboard.h"

namespace gui {

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      const no_erase_window_class editbox::clazz = no_erase_window_class(typeid(editbox).name());

      // --------------------------------------------------------------------------
      editbox_base::editbox_base () {
        init();
      }

      editbox_base::~editbox_base () {
        global::unregister_utf8_window(get_id());
      }

      void editbox_base::init () {
        data.lines.emplace_back(std::string());
        set_accept_focus(true);
        register_event_handler(REGISTER_FUNCTION, create_event(this, &editbox_base::handle_create));
        register_event_handler(REGISTER_FUNCTION, any_key_down_event(this, &editbox_base::handle_key));
        enable_select_by_mouse();
      }

      // --------------------------------------------------------------------------
      void editbox_base::handle_create (window*, const core::rectangle&) {
        global::register_utf8_window(get_id());
      }

      std::string& editbox_base::current_line () {
        return data.lines[std::max(0, std::min<int>(data.cursor_pos.row, static_cast<int>(data.lines.size())))];
      }

      void editbox_base::handle_key (os::key_state keystate,
                                os::key_symbol keycode,
                                const std::string& chars) {
        bool shift = shift_key_bit_mask::is_set(keystate);
        bool ctrl = control_key_bit_mask::is_set(keystate);
        std::string& current = current_line();

        using namespace ibr;

        switch (keycode) {
          case keys::left:
          case keys::numpad::left:
            if (ctrl) {
              // next word begin
              set_cursor_pos(find_prev_word(data.cursor_pos), shift);
            } else if (data.cursor_pos.column > 0) {
              set_cursor_pos({utf8::get_left_char(current, data.cursor_pos.column), data.cursor_pos.row}, shift);
              return;
            } else if (data.cursor_pos.row > 0) {
              auto row = data.cursor_pos.row - 1;
              set_cursor_pos({position::max, row}, shift);
            }
            break;
          case keys::right:
          case keys::numpad::right:
            if (ctrl) {
              set_cursor_pos(find_next_word(data.cursor_pos), shift);
            } else if (data.cursor_pos.column < current.size()) {
              set_cursor_pos({utf8::get_right_char(current, data.cursor_pos.column), data.cursor_pos.row}, shift);
            } else if (data.cursor_pos.row < (row_count() - 1)) {
              set_cursor_pos({0, data.cursor_pos.row + 1}, shift);
            }
            break;
          case keys::up:
          case keys::numpad::up:
            if (ctrl) {
            } else if (data.cursor_pos.row > 0) {
              set_cursor_pos({data.cursor_pos.column, data.cursor_pos.row - 1}, shift);
            }
            break;
          case keys::down:
          case keys::numpad::down:
            if (ctrl) {
            } else if (data.cursor_pos.row < (row_count() - 1)) {
              set_cursor_pos({data.cursor_pos.column, data.cursor_pos.row + 1}, shift);
            }
            break;
          case keys::home:
          case keys::numpad::home:
            if (ctrl) {
              set_cursor_pos(position::zero, shift);
            } else {
              set_cursor_pos({0, data.cursor_pos.row}, shift);
            }
            break;
          case keys::end:
          case keys::numpad::end:
            if (ctrl) {
              set_cursor_pos(position::end, shift);
            } else {
              set_cursor_pos({position::max, data.cursor_pos.row}, shift);
            }
            break;
          case keys::del:
          case keys::numpad::del:
            if (data.selection.empty()) {
              if (current.empty()) {
                erase_line(data.cursor_pos.row);
              } else if (data.cursor_pos.column < current.size()) {
                std::size_t cp = data.cursor_pos.column + 1;
                while ((cp < current.size()) && utf8::is_continuation_char(current.at(cp))) {
                  ++cp;
                }
                current.replace(data.cursor_pos.column, cp - data.cursor_pos.column, std::string());
                notify_content_changed();
              } else if (data.cursor_pos.row < (row_count() - 1)) {
                current.append(data.lines[data.cursor_pos.row + 1]);
                erase_line(data.cursor_pos.row + 1);
              }
            } else {
              replace_selection(std::string());
              set_cursor_pos(data.selection.first, false);
            }
            break;
          case keys::back_space:
            if (data.selection.empty()) {
              if (current.empty()) {
                erase_line(data.cursor_pos.row);
                set_cursor_pos({position::max, data.cursor_pos.row - 1}, false);
              } else if (data.cursor_pos.column > 0) {
                std::size_t cp = data.cursor_pos.column - 1;
                while ((cp > 0) && utf8::is_continuation_char(current.at(cp))) {
                  --cp;
                }
                current.replace(cp, data.cursor_pos.column - cp, std::string());
                notify_content_changed();
                set_cursor_pos({cp, data.cursor_pos.row}, false);
              } else if (data.cursor_pos.row > 0) {
                auto row = data.cursor_pos.row - 1;
                std::string& prev = data.lines[row];
                position pos(prev.size(), row);
                prev.append(current);
                erase_line(data.cursor_pos.row);
                set_cursor_pos(pos, false);
                redraw_later();
              }
            } else {
              replace_selection(std::string());
              set_cursor_pos(data.selection.first, false);
            }
            break;
          case keys::escape:
            set_selection({});
            break;
          case keys::clear:
            set_selection({position::zero, position::end});
            replace_selection(std::string());
            set_cursor_pos(position::zero, false);
            break;
          case keys::tab:
            break;
          case keys::enter: {
            if (!data.selection.empty()) {
              replace_selection(std::string());
              set_cursor_pos(data.selection.first, false);
            }
            auto rest = current.substr(data.cursor_pos.column);
            current.erase(data.cursor_pos.column);
            auto row = data.cursor_pos.row + 1;
            data.lines.insert(std::next(data.lines.begin(), row), rest);
            notify_content_changed();
            set_cursor_pos({0, row}, false);
            break;
          }
          default: {
            if (ctrl) {
              switch (keycode) {
                case keys::a:
                  // select all
                  set_selection({position::zero, position::end});
                  break;
                case keys::v: {
                  clipboard::get().get_text(*this, [&](const std::string& t) {
                    replace_selection(t);
                  });
                  break;
                case keys::x:
                  clipboard::get().set_text(*this, get_selected_text());
                  replace_selection(std::string());
                  break;
                }
                default:
                  LogDebug << "Key Ctrl + 0x" << std::hex << keycode;
                  break;
              }
            } else if (chars.size()) {
              replace_selection(chars);
              set_cursor_pos(data.selection.last, false);
            }
          }
        }
      }
      // --------------------------------------------------------------------------

    } // namespace detail

  } // win

} // gui

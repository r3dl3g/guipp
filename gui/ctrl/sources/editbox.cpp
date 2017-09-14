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
        register_event_handler(REGISTER_FUNCTION, key_down_event(this, &editbox_base::handle_key));
        register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state, const core::point& pt) {
          take_focus();
          data.last_mouse_point = pt;
          set_cursor_pos(get_position_at_point(pt));
        }));
        register_event_handler(REGISTER_FUNCTION, left_btn_up_event([&](os::key_state, const core::point& pt) {
          data.last_mouse_point = core::point::undefined;
        }));
        register_event_handler(REGISTER_FUNCTION, left_btn_dblclk_event([&](os::key_state, const core::point& pt) {
          take_focus();
          data.last_mouse_point = pt;
          const auto p = get_position_at_point(pt);
          const auto l = find_prev_word(p);
          const auto r = find_next_word(p);
          set_cursor_pos(p);
          set_selection({l, r});
        }));
        register_event_handler(REGISTER_FUNCTION, mouse_move_event([&](os::key_state keys, const core::point& pt) {
          if ((data.last_mouse_point != core::point::undefined) && left_button_bit_mask::is_set(keys)) {
            set_cursor_pos(get_position_at_point(pt), true);
          }
        }));
      }

      // --------------------------------------------------------------------------
      void editbox_base::handle_create (window*, const core::rectangle&) {
        global::register_utf8_window(get_id());
      }

      std::string& editbox_base::current_line () {
        return data.lines[std::max(0, std::min<int>(data.cursor_pos.row, static_cast<int>(data.lines.size())))];
      }

      editbox_base::position editbox_base::find_prev_word (const textbox_base::position& pos) {
        if (pos.column > 0) {
          std::string::size_type p = ibr::string::find_left_space(data.lines[pos.row], pos.column);
          if (p != std::string::npos) {
            return{p, pos.row};
          }
        }
        if (pos.row > 0) {
          return{data.lines[pos.row - 1].size(), pos.row - 1};
        }
        return pos;
      }

      editbox_base::position editbox_base::find_next_word (const textbox_base::position& pos) {
        if (pos.column < data.lines[pos.row].size()) {
          std::string::size_type p = ibr::string::find_right_space(data.lines[pos.row], pos.column);
          if (p != std::string::npos) {
            return{p, pos.row};
          }
        }
        if (pos.row < (row_count() - 1)) {
          return{0, pos.row + 1};
        }
        return pos;
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
              switch (toupper(keycode)) {
                case 'A':
                  // select all
                  set_selection({position::zero, position::end});
                  break;
                case 'V': {
                  clipboard::get().get_text(*this, [&](const std::string& t) {
                    replace_selection(t);
                  });
                  break;
                case 'C':
                case 'X':
                  clipboard::get().set_text(*this, get_selected_text());
                  if (toupper(keycode) == 'X') {
                    replace_selection(std::string());
                  }
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

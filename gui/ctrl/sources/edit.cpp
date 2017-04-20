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
#include <windowsx.h>
#endif // WIN32

// --------------------------------------------------------------------------
//
// Library includes
//
#include "edit.h"

//#define SHOW_TEXT_AREA

namespace gui {

  namespace win {

    namespace detail {

      window_class edit_base::clazz = gui::win::window_class("EDIT++", gui::color::white);

      const std::string white_space = " (){}[],.;:'\"!@#$%^&/*-+";

      edit_base::edit_base ()
        : cursor_pos(0)
        , text_limit(std::numeric_limits<pos_t>::max())
        , scroll_pos(0)
#ifdef X11
        , im(0)
        , ic(0)
#endif // X11
        , last_mouse_point(core::point::undefined)
      {}

      edit_base::~edit_base () {
#ifdef X11
        detail::s_window_ic_map.erase(get_id());
        XDestroyIC(ic);
        ic = 0;
        XCloseIM(im);
        im = 0;
#endif // X11
      }

      void edit_base::prepare_input () {
#ifdef X11
        im = XOpenIM(core::global::get_instance(), NULL, NULL, NULL);
        XIMStyle app_supported_styles = XIMPreeditNone | XIMPreeditNothing | XIMPreeditArea |
                                        XIMStatusNone | XIMStatusNothing | XIMStatusArea;

        XIMStyles *im_supported_styles;
        /* figure out which styles the IM can support */
        XGetIMValues(im, XNQueryInputStyle, &im_supported_styles, NULL);
        XIMStyle best_style = 0x0F1F;
        for (int i = 0; i < im_supported_styles->count_styles; ++i) {
          XIMStyle style = im_supported_styles->supported_styles[i];
          if ((style & app_supported_styles) == style) {/* if we can handle it */
            best_style = std::min(style, best_style);
          }
        }
        XFree(im_supported_styles);

        ic = XCreateIC(im,
                       XNInputStyle, best_style,
                       XNClientWindow, get_id(),
                       NULL);

        detail::s_window_ic_map[get_id()] = ic;
#endif // X11
      }

      void edit_base::set_selection (const edit_base::range& sel) {
        selection = sel;
        redraw_later();
      }

      edit_base::range edit_base::get_selection () const {
        return selection;
      }

      void edit_base::set_cursor_pos (pos_t pos, bool shift) {
        pos_t new_pos = std::min(pos, get_text_length());
        if (shift) {
          if (cursor_pos == selection.last) {
            selection.last = new_pos;
          } else if (cursor_pos == selection.first) {
            selection.first = new_pos;
          } else {
            selection = range(new_pos);
          }
          selection.sort();
        } else {
          selection = range(new_pos);
        }
        cursor_pos = new_pos;

        // make cursor pos visible
        if (cursor_pos < scroll_pos) {
          scroll_pos = 0;
        }

        core::size sz = draw::font::system().get_text_size(text.substr(scroll_pos, cursor_pos - scroll_pos));
        core::size max_sz = client_size();
        max_sz -= {6, 4};
        while ((sz.width() > max_sz.width()) && (scroll_pos < cursor_pos)) {
          ++scroll_pos;
          sz = draw::font::system().get_text_size(text.substr(scroll_pos, cursor_pos - scroll_pos));
        }

        redraw_later();
      }

      edit_base::pos_t edit_base::get_cursor_pos () const {
        return cursor_pos;
      }

      void edit_base::set_text_limit (pos_t max_chars) {
        text_limit = max_chars;
      }

      edit_base::pos_t edit_base::get_text_limit () const {
        return text_limit;
      }

      edit_base::pos_t edit_base::get_text_length () const {
        return (pos_t)text.size();
      }

      void edit_base::replace_selection (const std::string &new_text) {
        range sel = get_selection();
        text.replace(sel.first, sel.last - sel.first, new_text);
        set_selection(range(sel.first + new_text.size()));
        redraw_later();
      }

      namespace utf8 {
        bool is_continuation_char (char ch) {
          return ((ch & 0b11000000) == 0b10000000);
        }
      }

      edit_base::pos_t edit_base::get_char_at_point (const core::point& pt) {
          pos_t max_chars = text.size();
          for (pos_t i = scroll_pos + 1; i < max_chars; ++i) {
            core::size sz = draw::font::system().get_text_size(text.substr(scroll_pos, i - scroll_pos));
            if (sz.width() >= pt.x()) {
              return i - 1;
            }
          }
          return max_chars;
      }

      namespace paint {
        void edit_box(const draw::graphics& graph,
                      const win::window& win,
                      const std::string& text,
                      draw::text_origin origin,
                      const detail::edit_base::range& selection,
                      detail::edit_base::pos_t cursor_pos,
                      detail::edit_base::pos_t scroll_pos) {
          using namespace gui::draw;
          gui::core::rectangle area = win.client_area();
          draw::frame::sunken_relief(graph, area);
          area.shrink({3, 2});
          draw::clip clp(graph, area);
          core::point_type y1 = area.y() + 2;
          core::point_type y2 = area.y2() - 2;
          if (selection.first < selection.last) {
            gui::core::rectangle a1 = area;
            if (selection.first > scroll_pos) {
              graph.text(draw::bounding_box(text.substr(scroll_pos, selection.first - scroll_pos), a1, origin),
                         font::system(), color::black);
            } else {
              a1.x2(a1.x());
            }
            gui::core::rectangle a2 = area;
            graph.text(draw::bounding_box(text.substr(scroll_pos, selection.last - scroll_pos), a2, origin),
                       font::system(), color::black);
            graph.fill(draw::rectangle(core::point(a1.x2(), y1),
                                       core::point(a2.x2(), y2)),
                       color::light_blue);
          }
          if (cursor_pos < std::numeric_limits<detail::edit_base::pos_t>::max()) {
            gui::core::rectangle cursor_area = area;
            graph.text(draw::bounding_box(text.substr(scroll_pos, cursor_pos - scroll_pos), cursor_area, origin),
                       font::system(), color::black);
            graph.frame(line(core::point(cursor_area.x2(), y1),
                             core::point(cursor_area.x2(), y2)),
                             color::black);
          }
          graph.text(draw::text_box(text.substr(scroll_pos), area, origin), font::system(), color::black);
#ifdef SHOW_TEXT_AREA
          graph.text(draw::bounding_box(text, area, origin), font::system(), color::black);
          graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
#endif // SHOW_TEXT_AREA
        }
      }

      std::string::size_type find_left_space (const std::string& text, std::size_t cursor_pos) {
        std::string::size_type pos = text.find_last_not_of(white_space, cursor_pos - 1);
        if (pos != std::string::npos) {
          std::string::size_type pos2 = text.find_last_of(white_space, pos);
          if (pos2 != std::string::npos) {
            return pos2 + 1;
          }
        }
        return 0;
      }

      std::string::size_type find_right_space(const std::string& text, std::size_t cursor_pos) {
        std::string::size_type pos = text.find_first_of(white_space, cursor_pos + 1);
        if (pos != std::string::npos) {
          std::string::size_type pos2 = text.find_first_not_of(text[pos], pos);
          if (pos2 != std::string::npos) {
            return pos2;
          } else {
            return pos;
          }
        } else {
          return text.length();
        }
      }

      void edit_base::register_handler (alignment_h alignment) {
        register_event_handler(paint_event([&, alignment] (const gui::draw::graphics& graph) {
          paint::edit_box(graph, *this, text, (draw::text_origin)alignment, selection, cursor_pos, scroll_pos);
        }));
        register_event_handler(key_down_event([&] (os::key_state keystate,
                                                   os::key_symbol keycode,
                                                   const std::string& chars) {
          bool shift = shift_key_bit_mask::is_set(keystate);
          bool ctrl = control_key_bit_mask::is_set(keystate);

          switch (keycode) {
            case keys::left:
            case keys::numpad::left:
              if (ctrl) {
                // next word begin
                if (cursor_pos > 1) {
                  std::string::size_type pos = find_left_space(text, cursor_pos);
                  if (pos != std::string::npos) {
                    set_cursor_pos(pos, shift);
                    return;
                  }
                }
              } else if (cursor_pos > 0) {
                std::size_t cp = cursor_pos - 1;
                while ((cp > 0) && utf8::is_continuation_char(text.at(cp))) {
                  --cp;
                }
                set_cursor_pos(cp, shift);
                return;
              }
              set_cursor_pos(0, shift);
              break;
            case keys::right:
            case keys::numpad::right:
              if (ctrl) {
                set_cursor_pos(find_right_space(text, cursor_pos), shift);
              } else if (cursor_pos < get_text_length ()) {
                std::size_t cp = cursor_pos + 1;
                while ((cp < get_text_length ()) && utf8::is_continuation_char(text.at(cp))) {
                  ++cp;
                }
                set_cursor_pos(cp, shift);
              }
              break;
            case keys::home:
            case keys::numpad::home:
              set_cursor_pos(0, shift);
              break;
            case keys::end:
            case keys::numpad::end:
              set_cursor_pos(get_text_length(), shift);
              break;
            case keys::del:
            case keys::numpad::del:
              if (selection.empty()) {
                std::size_t cp = cursor_pos + 1;
                while ((cp < get_text_length ()) && utf8::is_continuation_char(text.at(cp))) {
                  ++cp;
                }
                text.replace(cursor_pos, cp - cursor_pos, std::string());
                redraw_later();
              } else {
                replace_selection(std::string());
                set_cursor_pos(selection.first, false);
              }
              break;
            case keys::back_space:
              if (selection.empty()) {
                if (cursor_pos > 0) {
                  std::size_t cp = cursor_pos - 1;
                  while ((cp > 0) && utf8::is_continuation_char(text.at(cp))) {
                    --cp;
                  }
                  text.replace(cp, cursor_pos - cp, std::string());
                  set_cursor_pos(cp, false);
                }
              } else {
                replace_selection(std::string());
                set_cursor_pos(selection.first, false);
              }
              break;
            case keys::escape:
              set_selection(range());
              break;
            case keys::clear:
              set_selection(range(0, text.size()));
              replace_selection(std::string());
              set_cursor_pos(0, false);
              break;
            case keys::tab:
            case keys::enter:
              break;
            // TBD: cut, copy, paste
            case 'a':
              if (ctrl) {
                // select all
                set_selection(range(0, text.size()));
              }
              // fall throught
            default: {
              if (ctrl) {
                LogDebug << "Key Ctrl + 0x" << std::hex << keycode;
              } else if (chars.size()) {
                replace_selection(chars);
                set_cursor_pos(selection.last, false);
              }
            }
          }
        }));
        register_event_handler(left_btn_down_event([&](const core::point& pt) {
          take_focus();
          last_mouse_point = pt;
          set_cursor_pos(get_char_at_point(pt));
        }));
        register_event_handler(left_btn_up_event([&](const core::point& pt) {
          last_mouse_point = core::point::undefined;
        }));
        register_event_handler(left_btn_dblclk_event([&](const core::point& pt) {
          take_focus();
          last_mouse_point = pt;
          pos_t p = get_char_at_point(pt);
          set_cursor_pos(p);
          pos_t l = find_left_space(text, p);
          pos_t r = find_right_space(text, p);
          set_selection(range(l, r));
        }));
        register_event_handler(mouse_move_event([&](unsigned int keys, const core::point& pt) {
          if ((last_mouse_point != core::point::undefined) && left_button_bit_mask::is_set(keys)) {
            set_cursor_pos(get_char_at_point(pt), true);
          }
        }));
      }

    } // detail

  } // win

} // gui

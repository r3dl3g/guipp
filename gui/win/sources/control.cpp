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
* @brief     C++ API: basic controls
*
* @file
*/


// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "control.h"

//#define SHOW_TEXT_AREA

namespace gui {

  namespace layout {


  }

  namespace win {

    namespace detail {

      edit_base::edit_base ()
        : cursor_pos(0)
        , text_limit(std::numeric_limits<pos_t>::max())
      {}

      window_class edit_base::register_edit_class (gui::win::alignment_h a) {
#ifdef WIN32
        return window_class::sub_class("MyEdit", "EDIT",
                                        a | ES_AUTOHSCROLL | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
                                        WS_EX_NOPARENTNOTIFY);
#endif // WIN32
#ifdef X11
        return gui::win::window_class::custom_class("EDIT",
                                                    0,
                                                    ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                                    PointerMotionMask | KeyPressMask | FocusChangeMask |
                                                    StructureNotifyMask | SubstructureRedirectMask,
                                                    a, 0, 0,
                                                    gui::draw::color::white);
#endif // X11
      }

      void edit_base::register_handler () {
#ifdef X11
        register_event_handler(gui::win::paint_event([&] (gui::draw::graphics& graph) {
          using namespace gui::draw;
          gui::core::rectangle area = client_area();
          graph.draw_relief(area, true, true);
          area.shrink({3, 2});
          text_origin origin = (text_origin)get_window_class()->ex_style;
          int y1 = area.y() + 2;
          int y2 = area.y2() - 2;
          if (selection.first < selection.last) {
            gui::core::rectangle a1 = area;
            graph.text(draw::bounding_box(text.substr(0, selection.first), a1, origin),
                       font::system(), color::black);
            gui::core::rectangle a2 = area;
            graph.text(draw::bounding_box(text.substr(0, selection.last), a2, origin),
                       font::system(), color::black);
            graph.fill(draw::rectangle(core::point(a1.x2(), y1),
                                       core::point(a2.x2(), y2)),
                       color::lightBlue);
          }
          if (cursor_pos < std::numeric_limits<pos_t>::max()) {
            gui::core::rectangle cursor_area = area;
            graph.text(draw::bounding_box(text.substr(0, cursor_pos), cursor_area, origin),
                       font::system(), color::black);
            graph.draw_line(core::point(cursor_area.x2(), y1),
                            core::point(cursor_area.x2(), y2),
                            draw::pen(color::black, draw::pen::solid, 1));
          }
          graph.text(draw::text_box(text, area, origin), font::system(), color::black);
#ifdef SHOW_TEXT_AREA
          graph.text(draw::bounding_box(text, area, origin), font::system(), color::black);
          graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
#endif // SHOW_TEXT_AREA
        }));
        register_event_handler(gui::win::key_down_event([&] (unsigned int keystate, KeySym keycode, XKeyEvent* e) {
          char text[8] = {0};
          KeySym key;
          int len = XLookupString(e, text, 8, &key, 0);
          if (len) {
            replace_selection(std::string(text, len));
          } else {
            bool shift = (keystate & ShiftMask) != 0;
            switch (keycode) {
              case XK_Left:
              case XK_KP_Left:
                set_cursor_pos(get_cursor_pos() > 0 ? get_cursor_pos() - 1 : 0, shift);
                return true;
              case XK_Right:
              case XK_KP_Right:
                set_cursor_pos(get_cursor_pos() + 1, shift);
                return true;
              case XK_Home:
              case XK_KP_Home:
                set_cursor_pos(0, shift);
                return true;
              case XK_End:
              case XK_KP_End:
                set_cursor_pos(get_text_length(), shift);
                return true;
            }
          }
        }));
#endif // X11
      }

#ifdef WIN32
      void edit_base::set_selection (const edit_base::range& sel) {
        Edit_SetSel(get_id(), sel.first, sel.last);
      }

      edit_base::range edit_base::get_selection () const {
        range sel;
        SendMessage(get_id(), EM_GETSEL, (WPARAM)&sel.first, (LPARAM)&sel.last);
        Edit_GetSel(get_id(), sel.first, sel.last);
      }

      edit_base::pos_t edit_base::get_cursor_pos () const {
        return (pos_t)SendMessage(get_id(), EM_GETTHUMB, 0, 0);
      }

      void edit_base::set_cursor_pos (pos_t pos, bool shift) {
        set_selection (selection(pos, pos));
      }

      void edit_base::set_text_limit (pos_t max_chars) {
        SendMessage(get_id(), EM_LIMITTEXT, (WPARAM)max_chars, 0);
      }

      edit_base::pos_t edit_base::get_text_limit () const {
        return (pos_t)SendMessage(get_id(), EM_GETLIMITTEXT, 0, 0);
      }

      edit_base::pos_t edit_base::get_text_length () const {
        return (pos_t)Edit_GetTextLength(get_id());
      }

      void edit_base::replace_selection (const std::string &new_text) {
        Edit_ReplaceSel(get_id(), (LPARAM)text.c_str());
      }
#endif // WIN32
#ifdef X11
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
#endif // X11

    } // detail

  } // win

} // gui

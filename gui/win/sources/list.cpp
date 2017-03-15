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
* @brief     C++ API: list
*
* @file
*/

#ifdef WIN32
#define WINVER 0x0602
#include <windowsx.h>
#define NOMINMAX
#endif // WIN32

// --------------------------------------------------------------------------
//
// Common includes
//
//#ifdef X11
//# include <X11/Xlib.h>
//#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include "list.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------

    // --------------------------------------------------------------------------
#ifdef X11
    Atom SELECTION_CHANGE_MESSAGE = 0;

    bool selection_changed_message_match::operator() (const core::event& e) {
      return (e.type == ClientMessage) && (e.xclient.message_type == SELECTION_CHANGE_MESSAGE);
    }
#endif // X11

    // --------------------------------------------------------------------------
    window_class list::clazz;

    list::list ()
#ifdef X11
      : item_count(0)
      , selection(-1)
      , gc(0)
      , offset(0)
#endif // X11
    {
#ifdef X11
      if (!SELECTION_CHANGE_MESSAGE) {
        SELECTION_CHANGE_MESSAGE = XInternAtom(core::global::get_instance(), "SELECTION_CHANGE_MESSAGE", False);
      }
#endif // X11
      if (!clazz.is_valid()) {
#ifdef WIN32
        clazz = win::window_class::sub_class("MyListBox",
                                             "LISTBOX",
                                             LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_NODATA | LBS_NOINTEGRALHEIGHT |
                                             WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
                                             WS_EX_NOPARENTNOTIFY);
#else // !WIN32
        clazz = window_class::custom_class("LISTBOX",
                                           1,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                           FocusChangeMask | KeyPressMask,
                                           0, 0, 0,
                                           draw::color::white);
#endif // !WIN32
      }
      register_event_handler(this, &list::list_handle_event);
    }

    void list::set_count (size_t count) {
#ifdef WIN32
      SendMessage(get_id(), LB_SETCOUNT, count, 0);
#endif // WIN32
#ifdef X11
      item_count = count;
      redraw_later();
#endif // X11
    }

    size_t list::get_count () const {
#ifdef WIN32
      return (size_t)ListBox_GetCount(get_id());
#endif // WIN32
#ifdef X11
      return item_count;
#endif // X11
    }

    void list::set_selection (int sel) {
#ifdef WIN32
      ListBox_SetCurSel(get_id(), sel);
      SendMessage(GetParent(get_id()), WM_COMMAND, MAKEWPARAM(get_owner_draw_id(), LBN_SELCHANGE), (LPARAM)get_id());
#endif // WIN32
#ifdef X11
      selection = std::min(std::max(0, sel), (int)get_count() - 1);
      // Make selection visible
      const int sel_pos = item_size.height() * selection;
      const core::size sz = size();
      if (offset + sel_pos < 0) {
        offset = -sel_pos;
      } else if (offset + sel_pos + item_size.height() > sz.height()) {
        offset = sz.height() - (sel_pos + item_size.height());
      }
      send_client_message(this, SELECTION_CHANGE_MESSAGE);
      redraw_later();
#endif // X11
    }

    int list::get_selection () const {
#ifdef WIN32
      return ListBox_GetCurSel(get_id());
#endif // WIN32
#ifdef X11
      return selection;
#endif // X11
    }

    void list::set_scroll_pos (int pos) {
#ifdef WIN32
      SetScrollPos(get_id(), SB_VERT, -pos, true);
#endif // WIN32
#ifdef X11
      const int max_delta = std::max(0, (item_size.height() * (int)get_count()) - size().height());
      offset = std::max(std::min(0, pos), -max_delta);
      redraw_later();
#endif // X11
    }

    int list::get_scroll_pos () const {
#ifdef WIN32
      return -GetScrollPos(get_id(), SB_VERT);
#endif // WIN32
#ifdef X11
      return offset;
#endif // X11
    }

    void list::set_drawer(std::function<item_draw> drawer, int item_height) {
      this->drawer = drawer;
      set_item_size( { 0, core::size::type(item_height) } );
    }

    void list::draw_item (draw::graphics& g, int idx, const core::rectangle& place, bool selected) {
      if (drawer) {
        drawer(g, idx, place, selected);
      }
    }

    bool list::list_handle_event (const core::event& e,
                                  core::event_result& result) {
#ifdef WIN32
      if (e.type == WM_DRAWITEM) {
        PDRAWITEMSTRUCT pdis = (PDRAWITEMSTRUCT)e.param_2;
        // If there are no list box items, skip this message. 
        if (pdis->itemID == -1) {
          return false;
        }
        switch (pdis->itemAction) {
          case ODA_SELECT:
          case ODA_DRAWENTIRE: {
            draw::graphics g(get_id(), pdis->hDC);
            core::rectangle place(pdis->rcItem);
            bool selected = (pdis->itemState & ODS_SELECTED);
            draw_item(g, pdis->itemID, place, selected);
          }
        }
        return true;
      }
#endif // WIN32
#ifdef X11
      switch (e.type) {
        case Expose: {
          if (!gc) {
            gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
          }
          draw::graphics g(e.xexpose.window, gc);
          core::rectangle place = client_area();
          const int max_y = place.y2();
          const int max_idx = (int)get_count();
          const int first = -offset / item_size.height();

          place.top_left({place.x(), core::point::type(offset + item_size.height() * first)});
          place.height(item_size.height());

          for(int idx = first; (idx < max_idx) && (place.y() < max_y); ++idx, place.move(item_size)) {
            draw_item(g, idx, place, selection == idx);
          }
          XFlushGC(e.xexpose.display, gc);
          return true;
        }
        case ButtonPress:
          if (e.xbutton.button == Button1) {
            last_mouse_point = core::point(e.xbutton);
            moved = false;
            return true;
          }
          break;
        case ButtonRelease:
          switch (e.xbutton.button) {
            case Button1:
              if (!moved) {
                const int new_selection = (e.xbutton.y - offset) / item_size.height();
                selection = new_selection == selection ? -1 : new_selection;
                send_client_message(this, SELECTION_CHANGE_MESSAGE);
                redraw_later();
                return true;
              }
              break;
            case Button4: { // Y-Wheel
              set_scroll_pos(offset + item_size.height());
              moved = true;
              return true;
            }
            case Button5: { // Y-Wheel
              set_scroll_pos(offset - item_size.height());
              moved = true;
              return true;
            }
          }
          break;
        case MotionNotify:
          if ((e.xmotion.state & Button1Mask) == Button1Mask) {
            int dy = e.xmotion.y - last_mouse_point.y();
            set_scroll_pos(offset + dy);
            last_mouse_point = core::point(e.xmotion);
            moved = true;
            return true;
          }
          break;
        case KeyPress: {
          KeySym key;
          char text[8] = {0};
          XLookupString(const_cast<XKeyEvent*>(&e.xkey), text, 8, &key, 0);
          switch (key) {
            case XK_Up:
            case XK_KP_Up:
              set_selection(get_selection() - 1);
              return true;
            case XK_Down:
            case XK_KP_Down:
              set_selection(get_selection() + 1);
              return true;
            case XK_Page_Up:
            case XK_KP_Page_Up:
              set_selection(get_selection() - (size().height() / item_size.height()));
              return true;
            case XK_Page_Down:
            case XK_KP_Page_Down:
              set_selection(get_selection() + (size().height() / item_size.height()));
              return true;
            case XK_Home:
            case XK_KP_Home:
              set_selection(0);
              return true;
            case XK_End:
            case XK_KP_End:
              set_selection((int)get_count() - 1);
              return true;
          }
          break;
        }
      }
#endif // X11
      return false;
    }

    list::~list () {
#ifdef X11
      if (gc) {
        if (core::global::get_instance()) {
          XFreeGC(core::global::get_instance(), gc);
        }
        gc = 0;
      }
#endif // X11
    }

    void list::draw_text_item (draw::graphics& g,
                               const std::string& text,
                               const core::rectangle& place,
                               bool selected) {
      using namespace draw;
      g.fill(rectangle(place), selected ? color::highLightColor : color::white);
      g.text(text_box(text, place, vcenter_left), font::system(),
             selected ? color::highLightTextColor : color::windowTextColor);
    }

  } // win

} // gui

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
#endif // WIN32

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "list.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    window_class list::clazz;

    // --------------------------------------------------------------------------
#ifdef WIN32
    list::list () {
      if (!clazz.is_valid()) {
        clazz = win::window_class::sub_class("MyListBox",
                                             "LISTBOX",
                                             LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_NODATA | LBS_NOINTEGRALHEIGHT |
                                             WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                             WS_EX_NOPARENTNOTIFY);
      }
      register_event_handler(this, &list::list_handle_event);
    }

    void list::create (const container& parent,
                 const core::rectangle& place) {
      super::create(clazz, parent, place);
    }

    void list::set_count (size_t count) {
      SendMessage(get_id(), LB_SETCOUNT, count, 0);
    }

    size_t list::get_count () const {
      return (size_t)ListBox_GetCount(get_id());
    }

    void list::set_selection (int sel) {
      ListBox_SetCurSel(get_id(), sel);
      SendMessage(GetParent(get_id()), WM_COMMAND, MAKEWPARAM(get_owner_draw_id(), LBN_SELCHANGE), (LPARAM)get_id());
    }

    int list::get_selection () const {
      return ListBox_GetCurSel(get_id());
    }

    void list::set_scroll_pos (int pos) {
      SendMessage(get_id(), LB_SETTOPINDEX, (LONG)ceil((double)pos / (double)get_item_size().height()), 0);
    }

    int list::get_scroll_pos () const {
      return GetScrollPos(get_id(), SB_VERT) * get_item_size().height();
    }

    void  list::enable_vscroll_bar(bool enable) {
      // first check, if needed.
      if (enable && (get_count() * get_item_height() > size().height())) {
        ShowScrollBar(get_id(), SB_VERT, true);
      }
      else {
        ShowScrollBar(get_id(), SB_VERT, false);
      }
    }

    bool list::is_vscroll_bar_enabled() const {
      return get_style(WS_VSCROLL) == WS_VSCROLL;
    }

    bool list::list_handle_event (const core::event& e,
                                  core::event_result& result) {
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
      return false;
    }

    list::~list ()
    {}

#endif // WIN32

#ifdef X11
    list::list ()
      : item_count(0)
      , selection(-1)
    {
      if (!clazz.is_valid()) {
        clazz = window_class::custom_class("LISTBOX",
                                           1,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                           FocusChangeMask | KeyPressMask | StructureNotifyMask,
                                           0, 0, 0,
                                           draw::color::white);
      }
      register_event_handler(this, &list::list_handle_event);
      scrollbar.register_event_handler(win::scroll_event([&](int) {
        redraw_later();
      }));
    }

    void list::create (const container& parent,
                       const core::rectangle& place) {
      super::create(clazz, parent, place);
      scrollbar.create(*reinterpret_cast<container*>(this), get_vscroll_bar_area());
    }

    void list::set_count (size_t count) {
      item_count = count;

      core::size::type ih = get_item_height();
      int h = (ih * (int)item_count) - size().height();

      scrollbar.set_max(std::max(h, 0));
      scrollbar.set_step(ih);
      scrollbar.set_visible((h > 0) && is_vscroll_bar_enabled());

      redraw_later();
    }

    size_t list::get_count () const {
      return item_count;
    }

    void list::set_selection (int sel) {
      selection = std::min(std::max(0, sel), (int)get_count() - 1);
      // Make selection visible
      const int sel_pos = get_item_height() * selection;
      const core::size sz = size();

      if (sel_pos < get_scroll_pos()) {
        set_scroll_pos(sel_pos);
      } else if (sel_pos + get_item_height() - get_scroll_pos() > sz.height()) {
        set_scroll_pos(sel_pos + get_item_height() - sz.height());
      }
      send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
      redraw_later();
    }

    int list::get_selection () const {
      return selection;
    }

    void list::set_scroll_pos (int pos) {
      const int max_delta = std::max(0, (get_item_height() * (int)get_count()) - size().height());
      scrollbar.set_value(std::min(std::max(0, pos), max_delta));
      redraw_later();
    }

    int list::get_scroll_pos () const {
      return scrollbar.get_value();
    }

    core::rectangle list::get_vscroll_bar_area () {
      core::rectangle r(size());
      r.x(r.x2() - core::point::type(16));
      r.width(16);
      return r;
    }

    void list::enable_vscroll_bar(bool enable) {
      scrollbar.enable(enable);
      scrollbar.set_visible(enable && scrollbar.get_max());
    }

    bool list::is_vscroll_bar_enabled() const {
      return scrollbar.is_enabled();
    }

    bool list::list_handle_event (const core::event& e,
                                  core::event_result& result) {
      switch (e.type) {
        case Expose: {
          draw::graphics g(e.xexpose.window, get_graphics(e));
          core::rectangle place = client_area();
          const int max_y = place.y2();
          const int max_idx = (int)get_count();
          const int first = get_scroll_pos() / get_item_height();

          place.top_left({place.x(), core::point::type(get_item_height() * first - get_scroll_pos())});
          place.height(get_item_height());

          for(int idx = first; (idx < max_idx) && (place.y() < max_y); ++idx, place.move(get_item_size())) {
            draw_item(g, idx, place, get_selection() == idx);
          }
          g.flush();
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
                const int new_selection = (e.xbutton.y + get_scroll_pos()) / get_item_height();
                set_selection(new_selection == get_selection() ? -1 : new_selection);
                send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
                redraw_later();
                return true;
              }
              last_mouse_point = core::point::undefined;
              break;
            case Button4: { // Y-Wheel
              set_scroll_pos(get_scroll_pos() - get_item_height());
              moved = true;
              return true;
            }
            case Button5: { // Y-Wheel
              set_scroll_pos(get_scroll_pos() + get_item_height());
              moved = true;
              return true;
            }
          }
          break;
        case MotionNotify:
          if ((e.xmotion.state & Button1Mask) == Button1Mask) {
            if (last_mouse_point != core::point::undefined) {
                int dy = last_mouse_point.y() - e.xmotion.y;
                set_scroll_pos(get_scroll_pos() + dy);
                moved = true;
            }
            last_mouse_point = core::point(e.xmotion);
            return true;
          }
          break;
        case ConfigureNotify: {
          scrollbar.place(get_vscroll_bar_area());
          core::size::type ih = get_item_height();
          int h = (ih * (int)item_count) - size().height();

          scrollbar.set_max(std::max(h, 0));
          scrollbar.set_visible((h > 0) && is_vscroll_bar_enabled());
          break;
        }
        case KeyPress: {
          KeySym key = XLookupKeysym(const_cast<XKeyEvent*>(&e.xkey), 0);
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
              set_selection(get_selection() - (size().height() / get_item_height()));
              return true;
            case XK_Page_Down:
            case XK_KP_Page_Down:
              set_selection(get_selection() + (size().height() / get_item_height()));
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
      return false;
    }

    list::~list () {
    }

#endif // X11

  } // win

} // gui

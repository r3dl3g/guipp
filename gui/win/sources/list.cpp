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
#ifdef WIN32
    // --------------------------------------------------------------------------
    namespace detail {
      list::list () {
        register_event_handler(this, &list::list_handle_event);
      }

      size_t list::get_count () const {
        return (size_t)ListBox_GetCount(get_id());
      }

      int list::get_selection () const {
        return ListBox_GetCurSel(get_id());
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
              draw_item(pdis->itemID, g, place, draw::color::windowColor, selected);
            }
          }
          return true;
        }
        return false;
      }

      list::~list ()
      {}

    }
    // --------------------------------------------------------------------------
    template<>
    listT<false>::listT() {
      if (!clazz.is_valid()) {
        clazz = win::window_class::sub_class("MyVListBox",
          "LISTBOX",
          LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_NODATA | LBS_NOINTEGRALHEIGHT | LBS_MULTICOLUMN |
          WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
          WS_EX_NOPARENTNOTIFY);
      }
      register_event_handler(size_event([&](const core::size& sz) {
        SendMessage(get_id(), LB_SETITEMHEIGHT, 0, sz.height());
      }));
    }

    template<>
    void listT<false>::create (const container& parent,
                               const core::rectangle& place) {
      super::create(clazz, parent, place);
    }

    template<>
    void listT<false>::set_count (size_t count) {
      SendMessage(get_id(), LB_SETCOUNT, count, 0);
    }

    template<>
    void listT<false>::set_selection (int sel) {
      ListBox_SetCurSel(get_id(), sel);
      SendMessage(GetParent(get_id()), WM_COMMAND, MAKEWPARAM(get_owner_draw_id(), LBN_SELCHANGE), (LPARAM)get_id());
    }

    template<>
    int listT<false>::get_scroll_pos () const {
      return GetScrollPos(get_id(), SB_HORZ) * get_item_size().width();
    }

    template<>
    int listT<true>::get_scroll_pos() const {
      return GetScrollPos(get_id(), SB_VERT) * get_item_size().height();
    }

    template<>
    void listT<false>::set_scroll_pos(int pos) {
      SendMessage(get_id(), LB_SETTOPINDEX, (LONG)ceil((double)pos / (double)get_item_size().width()), 0);
    }

    template<>
    void listT<true>::set_scroll_pos(int pos) {
      SendMessage(get_id(), LB_SETTOPINDEX, (LONG)ceil((double)pos / (double)get_item_size().height()), 0);
    }

    template<>
    void listT<false>::enable_scroll_bar (bool enable) {
      // first check, if needed.
      if (enable && (get_count() * get_item_width() > size().width())) {
        ShowScrollBar(get_id(), SB_HORZ, true);
      } else {
        ShowScrollBar(get_id(), SB_HORZ, false);
      }
    }

    template<>
    bool listT<false>::is_scroll_bar_enabled () const {
      return get_style(WS_HSCROLL) == WS_HSCROLL;
    }

    template<>
    bool listT<false>::is_scroll_bar_visible () const {
      return get_style(WS_HSCROLL) == WS_HSCROLL;
    }

    // --------------------------------------------------------------------------
    template<>
    listT<true>::listT() {
      if (!clazz.is_valid()) {
        clazz = win::window_class::sub_class("MyHListBox",
          "LISTBOX",
          LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_NODATA | LBS_NOINTEGRALHEIGHT |
          WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
          WS_EX_NOPARENTNOTIFY);
      }
    }

    template<>
    void  listT<true>::enable_scroll_bar(bool enable) {
      // first check, if needed.
      if (enable && (get_count() * get_item_height() > size().height())) {
        ShowScrollBar(get_id(), SB_VERT, true);
      }
      else {
        ShowScrollBar(get_id(), SB_VERT, false);
      }
    }

    template<>
    void listT<true>::create (const container& parent,
                              const core::rectangle& place) {
      super::create(clazz, parent, place);
    }

    template<>
    void listT<true>::set_count (size_t count) {
      SendMessage(get_id(), LB_SETCOUNT, count, 0);
    }

    template<>
    void listT<true>::set_selection (int sel) {
      ListBox_SetCurSel(get_id(), sel);
      SendMessage(GetParent(get_id()), WM_COMMAND, MAKEWPARAM(get_owner_draw_id(), LBN_SELCHANGE), (LPARAM)get_id());
    }

    template<>
    bool listT<true>::is_scroll_bar_enabled() const {
      return get_style(WS_VSCROLL) == WS_VSCROLL;
    }

    template<>
    bool listT<true>::is_scroll_bar_visible() const {
      return get_style(WS_VSCROLL) == WS_VSCROLL;
    }

    template<>
    core::size listT<false>::calc_item_size(core::size::type item_height) const {
      SendMessage(get_id(), LB_SETCOLUMNWIDTH, item_height, 0);
      SendMessage(get_id(), LB_SETITEMHEIGHT, 0, client_size().height());
      return { item_height, client_size().height() };
    }

    template<>
    core::size listT<true>::calc_item_size(core::size::type item_height) const {
      SendMessage(get_id(), LB_SETITEMHEIGHT, 0, item_height);
      return{ client_size().width(), item_height };
    }

#endif // WIN32
    // --------------------------------------------------------------------------

    template<>
    core::size listT<false>::client_size () const {
      core::size sz = super::client_size();
      if (is_scroll_bar_visible()) {
        sz.height(sz.height() - scroll_bar::get_scroll_bar_width());
      }
      return sz;
    }

    template<>
    core::size listT<true>::client_size() const {
      core::size sz = super::client_size();
      if (is_scroll_bar_visible()) {
        sz.width(sz.width() - scroll_bar::get_scroll_bar_width());
      }
      return sz;
    }

#ifdef X11
    // --------------------------------------------------------------------------
    namespace detail {
      list::list ()
        : item_count(0), selection(-1)
      {}

      size_t list::get_count () const {
        return item_count;
      }

      int list::get_selection () const {
        return selection;
      }

      bool list::list_handle_event (const core::event& e,
                                    core::event_result& result) {
        return false;
      }

      list::~list () {
      }
    }

    // --------------------------------------------------------------------------
    template<>
    listT<false>::listT () {
      if (!clazz.is_valid()) {
        clazz = window_class::custom_class("HLISTBOX",
                                           0,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                           FocusChangeMask | KeyPressMask | StructureNotifyMask,
                                           0, 0, 0,
                                           draw::color::white);
      }
      register_event_handler(this, &listT<false>::listT_handle_event);
      scrollbar.register_event_handler(win::scroll_event([&] (int) {
        redraw_later();
      }));
    }

    template<>
    void listT<false>::create (const container& parent,
                               const core::rectangle& place) {
      super::create(clazz, parent, place);
      scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area());
      adjust_scroll_bar();
    }

    template<>
    void listT<false>::set_count (size_t count) {
      item_count = count;

      core::size::type iw = get_item_width();
      int w = (iw * (int)item_count) - size().width();

      scrollbar.set_max(std::max(w, 0));
      scrollbar.set_step(iw);
      scrollbar.set_visible((w > 0) && is_scroll_bar_enabled());

      redraw_later();
    }

    template<>
    void listT<false>::set_selection (int sel) {
      selection = std::min(std::max(0, sel), (int)get_count() - 1);
      // Make selection visible
      const int sel_pos = get_item_width() * selection;
      const core::size sz = size();

      if (sel_pos < get_scroll_pos()) {
        set_scroll_pos(sel_pos);
      } else if (sel_pos + get_item_width() - get_scroll_pos() > sz.width()) {
        set_scroll_pos(sel_pos + get_item_width() - sz.width());
      }
      send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
      redraw_later();
    }

    template<>
    void listT<false>::set_scroll_pos (int pos) {
      const int max_delta = std::max(0, (get_item_width() * (int)get_count()) - size().width());
      scrollbar.set_value(std::min(std::max(0, pos), max_delta));
      redraw_later();
    }

    template<>
    int listT<false>::get_scroll_pos () const {
      return scrollbar.get_value();
    }

    template<>
    core::rectangle listT<false>::get_scroll_bar_area () {
      core::rectangle r(size());
      r.y(r.y2() - core::point::type(16));
      r.height(16);
      return r;
    }

    template<>
    void listT<false>::enable_scroll_bar (bool enable) {
      scrollbar.enable(enable);
      scrollbar.set_visible(enable && scrollbar.get_max());
    }

    template<>
    bool listT<false>::is_scroll_bar_enabled () const {
      return scrollbar.is_enabled();
    }

    template<>
    bool listT<false>::is_scroll_bar_visible () const {
      return scrollbar.is_visible();
    }

    template<>
    core::size listT<false>::calc_item_size(core::size::type item_height) const {
      return { item_height, client_size().height() };
    }

    template<>
    bool listT<false>::listT_handle_event (const core::event& e,
                                           core::event_result& result) {
      switch (e.type) {
        case Expose: {
          draw::graphics g(e.xexpose.window, get_graphics(e));
          core::rectangle place = client_area();
          const int max_x = place.x2();
          const int max_idx = (int)get_count();
          const int first = get_scroll_pos() / get_item_width();

          place.top_left({core::point::type(get_item_width() * first - get_scroll_pos()), place.y()});
          place.width(get_item_width() - 1);

          for(int idx = first; (idx < max_idx) && (place.x() < max_x); ++idx, place.move_x(get_item_width())) {
            draw_item(idx, g, place, get_selection() == idx);
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
                const int new_selection = (e.xbutton.x + get_scroll_pos()) / get_item_width();
                set_selection(new_selection == get_selection() ? -1 : new_selection);
                send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
                redraw_later();
                return true;
              }
              last_mouse_point = core::point::undefined;
              break;
            case 6: { // X-Wheel
              set_scroll_pos(get_scroll_pos() - get_item_width());
              moved = true;
              return true;
            }
            case 7: { // X-Wheel
              set_scroll_pos(get_scroll_pos() + get_item_width());
              moved = true;
              return true;
            }
          }
          break;
        case MotionNotify:
          if (left_button_bit_mask::is_set(e.xmotion.state)) {
            if (last_mouse_point != core::point::undefined) {
              int dx = last_mouse_point.x() - e.xmotion.x;
              set_scroll_pos(get_scroll_pos() + dx);
              moved = true;
            }
            last_mouse_point = core::point(e.xmotion);
            return true;
          }
          break;
        case ConfigureNotify: {
          scrollbar.place(get_scroll_bar_area());
          adjust_scroll_bar();
          break;
        }
        case KeyPress: {
          KeySym key = XLookupKeysym(const_cast<XKeyEvent*>(&e.xkey), 0);
          switch (key) {
            case XK_Left:
            case XK_KP_Left:
              set_selection(get_selection() - 1);
              return true;
            case XK_Right:
            case XK_KP_Right:
              set_selection(get_selection() + 1);
              return true;
            case XK_Page_Up:
            case XK_KP_Page_Up:
              set_selection(get_selection() - (size().width() / get_item_width()));
              return true;
            case XK_Page_Down:
            case XK_KP_Page_Down:
              set_selection(get_selection() + (size().width() / get_item_width()));
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

    template<>
    void listT<false>::adjust_scroll_bar () {
      core::size::type iw = get_item_width();
      int w = (iw * (int)item_count) - size().width();

      scrollbar.set_max(std::max(w, 0));
      scrollbar.set_visible((w > 0) && is_scroll_bar_enabled());
    }

    // --------------------------------------------------------------------------
    template<>
    listT<true>::listT () {
      if (!clazz.is_valid()) {
        clazz = window_class::custom_class("VLISTBOX",
                                           0,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                           FocusChangeMask | KeyPressMask | StructureNotifyMask,
                                           0, 0, 0,
                                           draw::color::white);
      }
      register_event_handler(this, &listT<true>::listT_handle_event);
      scrollbar.register_event_handler(win::scroll_event([&] (int) {
        redraw_later();
      }));
    }

    template<>
    void listT<true>::create (const container& parent,
                              const core::rectangle& place) {
      super::create(clazz, parent, place);
      scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area());
      adjust_scroll_bar();
    }

    template<>
    void listT<true>::set_count (size_t count) {
      item_count = count;

      core::size::type ih = get_item_height();
      int h = (ih * (int)item_count) - size().height();

      scrollbar.set_max(std::max(h, 0));
      scrollbar.set_step(ih);
      scrollbar.set_visible((h > 0) && is_scroll_bar_enabled());

      redraw_later();
    }

    template<>
    void listT<true>::set_selection (int sel) {
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

    template<>
    void listT<true>::set_scroll_pos (int pos) {
      const int max_delta = std::max(0, (get_item_height() * (int)get_count()) - size().height());
      scrollbar.set_value(std::min(std::max(0, pos), max_delta));
      redraw_later();
    }

    template<>
    int listT<true>::get_scroll_pos () const {
      return scrollbar.get_value();
    }

    template<>
    core::rectangle listT<true>::get_scroll_bar_area () {
      core::rectangle r(size());
      r.x(r.x2() - core::point::type(16));
      r.width(16);
      return r;
    }

    template<>
    void listT<true>::enable_scroll_bar (bool enable) {
      scrollbar.enable(enable);
      scrollbar.set_visible(enable && scrollbar.get_max());
    }

    template<>
    bool listT<true>::is_scroll_bar_enabled () const {
      return scrollbar.is_enabled();
    }

    template<>
    bool listT<true>::is_scroll_bar_visible () const {
      return scrollbar.is_visible();
    }

    template<>
    core::size listT<true>::calc_item_size(core::size::type item_height) const {
      return{ client_size().width(), item_height };
    }

    template<>
    bool listT<true>::listT_handle_event (const core::event& e,
                                          core::event_result& result) {
      switch (e.type) {
        case Expose: {
          draw::graphics g(e.xexpose.window, get_graphics(e));
          core::rectangle place = client_area();
          const int max_y = place.y2();
          const int max_idx = (int)get_count();
          const int first = get_scroll_pos() / get_item_height();

          place.top_left({place.x(), core::point::type(get_item_height() * first - get_scroll_pos())});
          place.height(get_item_height() - 1);

          for(int idx = first; (idx < max_idx) && (place.y() < max_y); ++idx, place.move_y(get_item_height())) {
            draw_item(idx, g, place, get_selection() == idx);
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
          if (left_button_bit_mask::is_set(e.xmotion.state)) {
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
          scrollbar.place(get_scroll_bar_area());
          adjust_scroll_bar();
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

    template<>
    void listT<true>::adjust_scroll_bar () {
      core::size::type ih = get_item_height();
      int h = (ih * (int)item_count) - size().height();

      scrollbar.set_max(std::max(h, 0));
      scrollbar.set_visible((h > 0) && is_scroll_bar_enabled());
    }

#endif // X11

  } // win

} // gui

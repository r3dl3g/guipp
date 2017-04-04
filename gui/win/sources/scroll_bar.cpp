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
#include "scroll_bar.h"


namespace gui {

  namespace win {

    core::size::type scroll_bar::get_scroll_bar_width () {
      return core::size::type(16);
    }

#ifdef WIN32
// --------------------------------------------------------------------------
    namespace detail {
      template<>
      scroll_barT<false>::scroll_barT () {
        if (!clazz.is_valid()) {
          clazz = win::window_class::sub_class("MyScrollBar",
            "SCROLLBAR",
            SBS_VERT | WS_VSCROLL | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            WS_EX_NOPARENTNOTIFY);
        }
      }

      template<>
      scroll_barT<true>::scroll_barT () {
        if (!clazz.is_valid()) {
          clazz = win::window_class::sub_class("MyScrollBar",
            "SCROLLBAR",
            SBS_HORZ | WS_HSCROLL | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            WS_EX_NOPARENTNOTIFY);
        }
      }

      template<>
      scroll_barT<false>::~scroll_barT () {}

      template<>
      scroll_barT<true>::~scroll_barT () {}
    }

    bool scroll_matcher::operator() (const core::event& e) {
      return (e.type == WM_VSCROLL) || (e.type == WM_HSCROLL);
    }

    int get_scroll_pos (const core::event& e) {
      SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS, 0, 0, 0, 0, 0 };
      GetScrollInfo(e.id, SB_CTL, &si);
      return si.nPos;
    }

    scroll_bar::scroll_bar () {
      register_event_handler(this, &scroll_bar::scroll_handle_event);
    }

    void scroll_bar::create (const window_class& type,
                             const container& parent,
                             const core::rectangle& place) {
      super::create(type, parent, place);
      SCROLLINFO si = { sizeof(SCROLLINFO), SIF_RANGE| SIF_POS| SIF_PAGE, 0, 100, 10, 0, 0 };
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    bool scroll_bar::scroll_handle_event (const core::event& e, core::event_result& result) {
      switch (e.type) {
      case WM_VSCROLL:
      case WM_HSCROLL: {
        short int nScrollCode = get_param1_low<short int>(e);
        SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL, 0, 0, 0, 0, 0 };
        GetScrollInfo(e.id, SB_CTL, &si);
        switch (nScrollCode) {
        case SB_BOTTOM:
          set_value(si.nMin);
          break;
        case SB_TOP:
          set_value(si.nMax);
          break;
        case SB_LINEUP:
          set_value(si.nPos - 1);
          break;
        case SB_LINEDOWN:
          set_value(si.nPos + 1);
          break;
        case SB_PAGEUP:
          set_value(si.nPos - si.nPage);
          break;
        case SB_PAGEDOWN:
          set_value(si.nPos + si.nPage);
          break;
        case SB_THUMBTRACK:
          set_value(si.nTrackPos);
          break;
        case SB_THUMBPOSITION:
          set_value(si.nTrackPos);
          break;
        case SB_ENDSCROLL:
          set_value(si.nPos);
          break;
        }
      }
      default:
        break;
      }
      return false;
    }

    int scroll_bar::get_min () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return si.nMin;
    }

    int scroll_bar::get_max () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return si.nMax;
    }

    int scroll_bar::get_step () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_PAGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return si.nPage;
    }

    int scroll_bar::get_value () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_POS, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return si.nPos;
    }

    void scroll_bar::set_min (int i) {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.nMin = i;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_max (int i) {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.nMax = i;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_min_max(int mi, int ma) {
      SCROLLINFO si = { sizeof(SCROLLINFO), SIF_RANGE, mi, ma, 0, 0, 0 };
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_step (int i) {
      SCROLLINFO si = { sizeof(SCROLLINFO), SIF_PAGE, 0, 0, (UINT)i, 0, 0 };
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_value (int i) {
      set_value (i, false);
    }

    void scroll_bar::set_value (int i, bool notify) {
      if (i != get_value()) {
        SCROLLINFO si = {sizeof(SCROLLINFO), SIF_POS, 0, 0, 0, i, i};
        SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
      }
    }

#endif // WIN32

#ifdef X11

    // --------------------------------------------------------------------------
    namespace detail {
      Atom SCROLLBAR_MESSAGE = 0;
    }
    // --------------------------------------------------------------------------
    scroll_bar::scroll_bar ()
      : min(0)
      , max(100)
      , step(10)
      , value(0)
    {
      if (!detail::SCROLLBAR_MESSAGE) {
        detail::SCROLLBAR_MESSAGE = XInternAtom(core::global::get_instance(), "SCROLLBAR_MESSAGE", False);
      }
    }

    void scroll_bar::create (const window_class& type,
                             const container& parent,
                             const core::rectangle& place) {
      super::create(type, parent, place);
    }

    int scroll_bar::get_min () const {
      return min;
    }

    int scroll_bar::get_max () const {
      return max;
    }

    int scroll_bar::get_step () const {
      return step;
    }

    int scroll_bar::get_value () const {
      return value;
    }

    void scroll_bar::set_min (int i) {
      min = i;
      value = std::max(value, min);
      redraw_later();
    }

    void scroll_bar::set_max (int i) {
      max = i;
      value = std::min(value, max);
      redraw_later();
    }

    void scroll_bar::set_min_max (int mi, int ma) {
      min = mi;
      max = ma;
      value = std::max(value, min);
      redraw_later();
    }

    void scroll_bar::set_step (int i) {
      step = i;
      redraw_later();
    }

    void scroll_bar::set_value (int i) {
      set_value(i, false);
    }

    void scroll_bar::set_value (int i, bool notify) {
      i = std::min(std::max(i, min), max);
      if (i != value) {
        value = i;
        redraw_later();
        if (notify) {
          send_client_message(this, detail::SCROLLBAR_MESSAGE, value);
        }
      }
    }


    bool scroll_bar::scroll_handle_event (const core::event& e, core::event_result& result) {
      return false;
    }

    namespace detail {
      // --------------------------------------------------------------------------
      template<>
      scroll_barT<false>::scroll_barT ()
        : state(Nothing_pressed)
        , last_position(0)
        , gc(0) {
        if (!clazz.is_valid()) {
          clazz = window_class::custom_class("VSCROLLBAR",
                                             0,
                                             ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                             FocusChangeMask | KeyPressMask,
                                             0, 0, 0,
                                             draw::color::veryLightGray);
        }
        register_event_handler(this, &scroll_barT<false>::scroll_handle_eventT);
      }

      template<>
      scroll_barT<false>::~scroll_barT () {
        if (gc) {
          if (core::global::get_instance()) {
            XFreeGC(core::global::get_instance(), gc);
          }
          gc = 0;
        }
      }

      // --------------------------------------------------------------------------
      template<>
      scroll_barT<true>::scroll_barT ()
        : state(Nothing_pressed)
        , last_position(0)
        , gc(0) {
        if (!clazz.is_valid()) {
          clazz = window_class::custom_class("HSCROLLBAR",
                                             0,
                                             ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                             FocusChangeMask | KeyPressMask,
                                             0, 0, 0,
                                             draw::color::veryLightGray);
        }
        register_event_handler(this, &scroll_barT<true>::scroll_handle_eventT);
      }

      template<>
      scroll_barT<true>::~scroll_barT () {
        if (gc) {
          if (core::global::get_instance()) {
            XFreeGC(core::global::get_instance(), gc);
          }
          gc = 0;
        }
      }

      // --------------------------------------------------------------------------

      template<>
      core::size::type scroll_barT<true>::button_size (const core::rectangle& place) const {
        return place.height();
      }

      template<>
      core::size::type scroll_barT<false>::button_size (const core::rectangle& place) const {
        return place.width();
      }

      template<>
      int scroll_barT<true>::space_size (const core::rectangle& place) const {
        return place.width() - button_size(place) * 2 - 2;
      }

      template<>
      int scroll_barT<false>::space_size (const core::rectangle& place) const {
        return place.height() - button_size(place) * 2 - 2;
      }

      template<>
      core::rectangle scroll_barT<true>::down_button_place (const core::rectangle& place) const {
        core::size::type sz = button_size(place);
        return core::rectangle(place.x2() - sz, place.y(), sz, sz);
      }

      template<>
      core::rectangle scroll_barT<false>::down_button_place (const core::rectangle& place) const {
        core::size::type sz = button_size(place);
        return core::rectangle(place.x(), place.y2() - sz, sz, sz);
      }

      template<>
      core::rectangle scroll_barT<true>::page_up_place (const core::rectangle& place) const {
        core::size::type sz = button_size(place);
        core::size::type bottom = (core::size::type)thumb_top(place);
        return core::rectangle(sz, place.y(), bottom - sz, sz);
      }

      template<>
      core::rectangle scroll_barT<false>::page_up_place (const core::rectangle& place) const {
        core::size::type sz = button_size(place);
        core::size::type bottom = (core::size::type)thumb_top(place);
        return core::rectangle(place.x(), sz, sz, bottom - sz);
      }

      template<>
      core::rectangle scroll_barT<true>::page_down_place (const core::rectangle& place) const {
        core::size::type sz = button_size(place);
        core::point::type top = thumb_top(place) + thumb_size(place);
        return core::rectangle(top, place.y(), place.x2() - sz - top, sz);
      }

      template<>
      core::rectangle scroll_barT<false>::page_down_place (const core::rectangle& place) const {
        core::size::type sz = button_size(place);
        core::point::type top = thumb_top(place) + thumb_size(place);
        return core::rectangle(place.x(), top, sz, place.y2() - sz - top);
      }

      template<>
      core::rectangle scroll_barT<true>::thumb_button_place (const core::rectangle& place) const {
        core::size::type sz = button_size(place);
        core::size::type tsz = thumb_size(place);
        core::point::type pos = thumb_top(place);
        return core::rectangle(pos, place.y(), tsz, sz);
      }

      template<>
      core::rectangle scroll_barT<false>::thumb_button_place (const core::rectangle& place) const {
        core::size::type sz = button_size(place);
        core::size::type tsz = thumb_size(place);
        core::point::type pos = thumb_top(place);
        return core::rectangle(place.x(), pos, sz, tsz);
      }

      template<>
      bool scroll_barT<true>::scroll_handle_eventT (const core::event& e,
                                                    core::event_result& result) {
        switch (e.type) {
          case Expose: {
            if (!gc) {
              gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
            }
            draw::graphics g(e.xexpose.window, gc);
            core::rectangle place = client_area();

            auto up = up_button_place(place);
            g.fill(draw::rectangle(up), draw::color::buttonColor);
            draw::frame::deep_relief(g, up, state == Up_button_pressed);
            g.text(draw::text_box("<", up, draw::center),
                   draw::font::system(), is_enabled() ? draw::color::black : draw::color::gray);

            auto down = down_button_place(place);
            g.fill(draw::rectangle(down), draw::color::buttonColor);
            draw::frame::deep_relief(g, down, state == Down_button_pressed);
            g.text(draw::text_box(">", down, draw::center),
                   draw::font::system(), is_enabled() ? draw::color::black : draw::color::gray);

            auto thumb = thumb_button_place(place);
            g.fill(draw::rectangle(thumb), draw::color::buttonColor);
            draw::frame::raised_deep_relief(g, thumb);

            switch (state) {
              case Page_up_pressed:
                g.fill(draw::rectangle(page_up_place(place)), draw::color::lightGray);
                break;
              case Page_down_pressed:
                g.fill(draw::rectangle(page_down_place(place)), draw::color::lightGray);
                break;
            }
            return true;
          }
          case ButtonPress:
            if (e.xbutton.button == Button1) {
              last_mouse_point = core::point(e.xbutton);
              last_position = get_value();
              core::rectangle place = client_area();
              if (up_button_place(place).is_inside(last_mouse_point)) {
                state = Up_button_pressed;
              } else if (down_button_place(place).is_inside(last_mouse_point)) {
                state = Down_button_pressed;
              } else if (thumb_button_place(place).is_inside(last_mouse_point)) {
                state = Thumb_button_pressed;
              } else if (page_up_place(place).is_inside(last_mouse_point)) {
                state = Page_up_pressed;
              } else if (page_down_place(place).is_inside(last_mouse_point)) {
                state = Page_down_pressed;
              } else {
                state = Nothing_pressed;
              }
              redraw_later();
              return true;            }
            break;
          case ButtonRelease:
            switch (e.xbutton.button) {
              case Button1: {
                auto pt = core::point(e.xbutton);
                core::rectangle place = client_area();
                switch (state) {
                  case Up_button_pressed:
                    if (up_button_place(place).is_inside(pt)) {
                      set_value(get_value() - 1, true);
                    }
                    break;
                  case Down_button_pressed:
                    if (down_button_place(place).is_inside(pt)) {
                      set_value(get_value() + 1, true);
                    }
                    break;
                  case Page_up_pressed:
                    if (page_up_place(place).is_inside(pt)) {
                      set_value(get_value() - get_step(), true);
                    }
                    break;
                  case Page_down_pressed:
                    if (page_down_place(place).is_inside(pt)) {
                      set_value(get_value() + get_step(), true);
                    }
                    break;
                }
                break;
              }
              case 6: { // X-Wheel
                set_value(get_value() - get_step(), true);
                return true;
              }
              case 7: { // X-Wheel
                set_value(get_value() + get_step(), true);
                return true;
              }
            }
            state = Nothing_pressed;
            redraw_later();
            break;
          case MotionNotify:
            if (left_button_bit_mask::is_set(e.xmotion.state)) {
              // check if on thumb
              if (state == Thumb_button_pressed) {
                int delta = (int)((e.xmotion.x - last_mouse_point.x()) / get_scale());
                set_value(last_position + delta, true);
              }
              return true;
            }
            break;
          case KeyPress: {
            KeySym key = get_keycode<XKeyEvent>(e);
            switch (key) {
              case XK_Left:
              case XK_KP_Left:
                set_value(get_value() - 1, true);
                return true;
              case XK_Right:
              case XK_KP_Right:
                set_value(get_value() + 1, true);
                return true;
              case XK_Page_Up:
              case XK_KP_Page_Up:
                set_value(get_value() - get_step(), true);
                return true;
              case XK_Page_Down:
              case XK_KP_Page_Down:
                set_value(get_value() + get_step(), true);
                return true;
              case XK_Home:
              case XK_KP_Home:
                set_value(get_min(), true);
                return true;
              case XK_End:
              case XK_KP_End:
                set_value(get_min(), true);
                return true;
            }
            break;
          }
        }
        return false;
      }

      template<>
      bool scroll_barT<false>::scroll_handle_eventT (const core::event& e,
                                                     core::event_result& result) {
        switch (e.type) {
          case Expose: {
            if (!gc) {
              gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
            }
            draw::graphics g(e.xexpose.window, gc);
            core::rectangle place = client_area();

            auto up = up_button_place(place);
            g.fill(draw::rectangle(up), draw::color::buttonColor);
            draw::frame::deep_relief(g, up, state == Up_button_pressed);
            g.text(draw::text_box(u8"\u2227", up, draw::center),
                   draw::font::system(), is_enabled() ? draw::color::black : draw::color::gray);

            auto down = down_button_place(place);
            g.fill(draw::rectangle(down), draw::color::buttonColor);
            draw::frame::deep_relief(g, down, state == Down_button_pressed);
            g.text(draw::text_box(u8"\u2228", down, draw::center),
                   draw::font::system(), is_enabled() ? draw::color::black : draw::color::gray);

            auto thumb = thumb_button_place(place);
            g.fill(draw::rectangle(thumb), draw::color::buttonColor);
            draw::frame::raised_deep_relief(g, thumb);

            switch (state) {
              case Page_up_pressed:
                g.fill(draw::rectangle(page_up_place(place)), draw::color::lightGray);
                break;
              case Page_down_pressed:
                g.fill(draw::rectangle(page_down_place(place)), draw::color::lightGray);
                break;
            }
            return true;
          }
          case ButtonPress:
            if (e.xbutton.button == Button1) {
              last_mouse_point = core::point(e.xbutton);
              last_position = get_value();
              core::rectangle place = client_area();
              if (up_button_place(place).is_inside(last_mouse_point)) {
                state = Up_button_pressed;
              } else if (down_button_place(place).is_inside(last_mouse_point)) {
                state = Down_button_pressed;
              } else if (thumb_button_place(place).is_inside(last_mouse_point)) {
                state = Thumb_button_pressed;
              } else if (page_up_place(place).is_inside(last_mouse_point)) {
                state = Page_up_pressed;
              } else if (page_down_place(place).is_inside(last_mouse_point)) {
                state = Page_down_pressed;
              } else {
                state = Nothing_pressed;
              }
              redraw_later();
              return true;
            }
            break;
          case ButtonRelease:
            switch (e.xbutton.button) {
              case Button1: {
                auto pt = core::point(e.xbutton);
                core::rectangle place = client_area();
                switch (state) {
                  case Up_button_pressed:
                    if (up_button_place(place).is_inside(pt)) {
                      set_value(get_value() - 1, true);
                    }
                    break;
                  case Down_button_pressed:
                    if (down_button_place(place).is_inside(pt)) {
                      set_value(get_value() + 1, true);
                    }
                    break;
                  case Page_up_pressed:
                    if (page_up_place(place).is_inside(pt)) {
                      set_value(get_value() - get_step(), true);
                    }
                    break;
                  case Page_down_pressed:
                    if (page_down_place(place).is_inside(pt)) {
                      set_value(get_value() + get_step(), true);
                    }
                    break;
                }
                break;
              }
              case Button4: { // Y-Wheel
                set_value(get_value() - get_step(), true);
                return true;
              }
              case Button5: { // Y-Wheel
                set_value(get_value() + get_step(), true);
                return true;
              }
            }
            state = Nothing_pressed;
            redraw_later();
            break;
          case MotionNotify:
            if (left_button_bit_mask::is_set(e.xmotion.state)) {
              // check if on thumb
              if (state == Thumb_button_pressed) {
                int delta = (int)((e.xmotion.y - last_mouse_point.y()) / get_scale());
                set_value(last_position + delta, true);
              }
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
                set_value(get_value() - 1, true);
                return true;
              case XK_Down:
              case XK_KP_Down:
                set_value(get_value() + 1, true);
                return true;
              case XK_Page_Up:
              case XK_KP_Page_Up:
                set_value(get_value() - get_step(), true);
                return true;
              case XK_Page_Down:
              case XK_KP_Page_Down:
                set_value(get_value() + get_step(), true);
                return true;
              case XK_Home:
              case XK_KP_Home:
                set_value(get_min(), true);
                return true;
              case XK_End:
              case XK_KP_End:
                set_value(get_min(), true);
                return true;
            }
            break;
          }
        }
        return false;
      }
    }

#endif // X11

  } // win

} // gui

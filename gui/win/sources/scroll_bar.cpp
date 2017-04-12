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

    scroll_bar::type scroll_bar::get_scroll_bar_width () {
      return scroll_bar::type(17);
    }

#ifdef WIN32
// --------------------------------------------------------------------------
    namespace detail {

      template<>
      scroll_bar_class<false>::scroll_bar_class ()
        : window_class(sub_class("MyScrollBar",
                                 "SCROLLBAR",
                                 SBS_VERT | /*WS_VSCROLL | */WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                 WS_EX_NOPARENTNOTIFY))
      {}

      template<>
      scroll_bar_class<true>::scroll_bar_class ()
        : window_class(sub_class("MyScrollBar",
                                 "SCROLLBAR",
                                 SBS_HORZ | /*WS_HSCROLL | */WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                 WS_EX_NOPARENTNOTIFY))
      {}

      template<>
      void scroll_bar_class<false>::prepare (window* win) const {
        window_class::prepare(win);
      }

      template<>
      void scroll_bar_class<true>::prepare (window* win) const {
        window_class::prepare(win);
      }

      template<>
      scroll_barT<false>::scroll_barT ()
      {}

      template<>
      scroll_barT<true>::scroll_barT ()
      {}

      template<>
      scroll_barT<false>::~scroll_barT ()
      {}

      template<>
      scroll_barT<true>::~scroll_barT ()
      {}

    }

    bool scroll_matcher::operator() (const core::event& e) {
      return (e.type == WM_VSCROLL) || (e.type == WM_HSCROLL);
    }

    core::point::type get_scroll_pos(const core::event& e) {
      SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS, 0, 0, 0, 0, 0 };
      GetScrollInfo(e.id, SB_CTL, &si);
      return static_cast<core::point::type>(si.nPos);
    }

    scroll_bar::scroll_bar () {
      register_event_handler(this, &scroll_bar::scroll_handle_event);
    }

    void scroll_bar::create (const window_class& type,
                             const container& parent,
                             const core::rectangle& place) {
      super::create(type, parent, place);
      SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL, 0, 100, 10, 0, 0 };
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    bool scroll_bar::scroll_handle_event (const core::event& e, os::event_result& result) {
      switch (e.type) {
      case WM_VSCROLL:
      case WM_HSCROLL: {
        short int nScrollCode = get_param1_low<short int>(e);
        SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL, 0, 0, 0, 0, 0 };
        GetScrollInfo(e.id, SB_CTL, &si);
        switch (nScrollCode) {
        case SB_BOTTOM:
          set_value(static_cast<type>(si.nMin));
          break;
        case SB_TOP:
          set_value(static_cast<type>(si.nMax));
          break;
        case SB_LINEUP:
          set_value(static_cast<type>(si.nPos - 1));
          break;
        case SB_LINEDOWN:
          set_value(static_cast<type>(si.nPos + 1));
          break;
        case SB_PAGEUP:
          set_value(static_cast<type>(si.nPos - si.nPage));
          break;
        case SB_PAGEDOWN:
          set_value(static_cast<type>(si.nPos + si.nPage));
          break;
        case SB_THUMBTRACK:
          set_value(static_cast<type>(si.nTrackPos));
          break;
        //case SB_THUMBPOSITION:
        //  set_value(static_cast<type>(si.nTrackPos));
        //  break;
        //case SB_ENDSCROLL:
        //  set_value(static_cast<type>(si.nPos));
        //  break;
        }
      }
      default:
        break;
      }
      return false;
    }

    scroll_bar::type scroll_bar::get_min () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return static_cast<type>(si.nMin);
    }

    scroll_bar::type scroll_bar::get_max () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return static_cast<type>(si.nMax - si.nPage);
    }

    scroll_bar::type scroll_bar::get_step () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_PAGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return static_cast<type>(si.nPage);
    }

    scroll_bar::type scroll_bar::get_value () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_POS, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return static_cast<type>(si.nPos);
    }

    scroll_bar::State scroll_bar::get_state () {
      SCROLLBARINFO info = { 0 };
      info.cbSize = sizeof(SCROLLBARINFO);
      if (GetScrollBarInfo(get_id(), OBJID_CLIENT, &info)) {
        for (int i = Up_button_pressed; i <= Down_button_pressed; ++i) {
          if ((info.rgstate[i] & STATE_SYSTEM_PRESSED) == STATE_SYSTEM_PRESSED) {
            return State(i);
          }
        }
      }
      return Nothing_pressed;
    }

    void scroll_bar::set_min (type mi) {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.nMin = static_cast<int>(mi);
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_max (type ma) {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.nMax = static_cast<int>(ma) + si.nPage;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_min_max (type mi, type ma) {
      SCROLLINFO si = { sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE, 0, 0, 0, 0, 0 };
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.nMin = static_cast<int>(mi);
      si.nMax = static_cast<int>(ma) + si.nPage;
      si.fMask = SIF_RANGE;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
      LogDebug << "Scroll_bar min:" << si.nMin << ", max:" << si.nMax;
    }

    void scroll_bar::set_step (type s) {
      SCROLLINFO si = { sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE, 0, 0, 0, 0, 0 };
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.nMax -= si.nPage;
      si.nPage = (UINT)s;
      si.nMax += si.nPage;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_min_max_step (type mi, type ma, type s) {
      SCROLLINFO si = {
        sizeof(SCROLLINFO),
        SIF_RANGE | SIF_PAGE,
        static_cast<int>(mi),
        static_cast<int>(ma + s),
        (UINT)s,
        0,
        0
      };
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_min_max_step_value (type mi, type ma, type s, type v) {
      SCROLLINFO si = {
        sizeof(SCROLLINFO),
        SIF_ALL,
        static_cast<int>(mi),
        static_cast<int>(ma + s),
        (UINT)s,
        static_cast<int>(v),
        static_cast<int>(v)
      };
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_value (type v) {
      set_value (v, false);
    }

    void scroll_bar::set_value (type v, bool notify) {
      if (v != get_value()) {
        SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS | SIF_TRACKPOS, 0, 0, 0, static_cast<int>(v), static_cast<int>(v) };
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
      , state(Nothing_pressed)
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

    scroll_bar::type scroll_bar::get_min () const {
      return min;
    }

    scroll_bar::type scroll_bar::get_max () const {
      return max;
    }

    scroll_bar::type scroll_bar::get_step () const {
      return step;
    }

    scroll_bar::type scroll_bar::get_value () const {
      return value;
    }

    scroll_bar::State scroll_bar::get_state () {
      return state;
    }

    void scroll_bar::set_min (type mi) {
      min = mi;
      value = std::max(value, min);
      redraw_later();
    }

    void scroll_bar::set_max (type ma) {
      max = ma;
      value = std::min(value, max);
      redraw_later();
    }

    void scroll_bar::set_min_max (type mi, type ma) {
      min = mi;
      max = ma;
      value = std::max(value, min);
      redraw_later();
    }

    void scroll_bar::set_step (type s) {
      step = s;
      redraw_later();
    }

    void scroll_bar::set_min_max_step (type mi, type ma, type s) {
      min = mi;
      max = ma;
      step = s;
      redraw_later();
    }

    void scroll_bar::set_min_max_step_value(type mi, type ma, type s, type v) {
      set_min_max_step(mi, ma, s);
      set_value(v);
    }

    void scroll_bar::set_value (type v) {
      set_value(v, false);
    }

    void scroll_bar::set_value (type v, bool notify) {
      v = std::min(std::max(v, min), max);
      if (v != value) {
        value = v;
        redraw_later();
        if (notify) {
          send_client_message(this, detail::SCROLLBAR_MESSAGE, value);
        }
      }
    }


    bool scroll_bar::scroll_handle_event (const core::event& e, os::event_result& result) {
      return false;
    }

    void scroll_bar::set_state (State s) {
      state = s;
    }

    void scroll_bar::draw_scrollbar (draw::graphics &g,
                                     const std::string& up_char,
                                     const std::string& down_char,
                                     const core::rectangle& up,
                                     const core::rectangle& down,
                                     const core::rectangle& thumb,
                                     const core::rectangle& page_up,
                                     const core::rectangle& page_down) const {

      if (!page_up.empty()) {
        g.fill(draw::rectangle(page_up), state == Page_up_pressed ? draw::color::lightGray()
                                                                  : draw::color::veryLightGray());
      }
      if (!page_down.empty()) {
        g.fill(draw::rectangle(page_down), state == Page_down_pressed ? draw::color::lightGray()
                                                                      : draw::color::veryLightGray());
      }
      if (!up.empty()) {
        g.fill(draw::rectangle(up), draw::color::buttonColor());
        draw::frame::relief(g, up, state == Up_button_pressed);
        g.text(draw::text_box(up_char, up, draw::center),
               draw::font::system(), is_enabled() ? draw::color::black()
                                                  : draw::color::gray());
      }
      if (!down.empty()) {
        g.fill(draw::rectangle(down), draw::color::buttonColor());
        draw::frame::relief(g, down, state == Down_button_pressed);
        g.text(draw::text_box(down_char, down, draw::center),
               draw::font::system(), is_enabled() ? draw::color::black()
                                                  : draw::color::gray());
      }
      if (!thumb.empty()) {
        g.fill(draw::rectangle(thumb), draw::color::buttonColor());
        draw::frame::raised_relief(g, thumb);
      }
    }

    namespace detail {

      template<>
      scroll_bar_class<false>::scroll_bar_class ()
        : window_class(custom_class("VSCROLLBAR",
                                    0,
                                    ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                    FocusChangeMask | KeyPressMask,
                                    0, 0, 0,
                                    draw::color::veryLightGray()))
      {}

      template<>
      scroll_bar_class<true>::scroll_bar_class ()
        : window_class(custom_class("HSCROLLBAR",
                                    0,
                                    ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                    FocusChangeMask | KeyPressMask,
                                    0, 0, 0,
                                    draw::color::veryLightGray()))
      {}

      template<>
      void scroll_bar_class<false>::prepare (window* win) const {
        window_class::prepare(win);
        unsigned long mask = CWBackPixmap;
        XSetWindowAttributes wa;
        wa.background_pixmap = None;
        XChangeWindowAttributes(core::global::get_instance(), win->get_id(), mask, &wa);
      }

      template<>
      void scroll_bar_class<true>::prepare (window* win) const {
        window_class::prepare(win);
        unsigned long mask = CWBackPixmap;
        XSetWindowAttributes wa;
        wa.background_pixmap = None;
        XChangeWindowAttributes(core::global::get_instance(), win->get_id(), mask, &wa);
      }

      // --------------------------------------------------------------------------
      template<>
      scroll_barT<false>::scroll_barT ()
        : last_position(0)
        , gc(0) {
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
        : last_position(0)
        , gc(0) {
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
      const std::string left_char = "<";
      const std::string right_char = ">";

      template<>
      bool scroll_barT<true>::scroll_handle_eventT (const core::event& e,
                                                    os::event_result& result) {
        switch (e.type) {
          case Expose: {
            draw::graphics g(e.xexpose.window, get_param<0, draw::graphics>(e));

            auto geo = get_geometry();
            auto up = up_button_place(geo);
            auto down = down_button_place(geo);
            auto thumb = thumb_button_place(geo);
            auto page_up = page_up_place(geo);
            auto page_down = page_down_place(geo);

            draw_scrollbar(g, left_char, right_char, up, down, thumb, page_up, page_down);

            return true;
          }
          case ButtonPress:
            if (e.xbutton.button == Button1) {
              last_mouse_point = core::point(e.xbutton);
              last_position = get_value();

              auto geo = get_geometry();

              if (up_button_place(geo).is_inside(last_mouse_point)) {
                set_state(Up_button_pressed);
              } else if (down_button_place(geo).is_inside(last_mouse_point)) {
                set_state(Down_button_pressed);
              } else if (thumb_button_place(geo).is_inside(last_mouse_point)) {
                set_state(Thumb_button_pressed);
              } else if (page_up_place(geo).is_inside(last_mouse_point)) {
                set_state(Page_up_pressed);
              } else if (page_down_place(geo).is_inside(last_mouse_point)) {
                set_state(Page_down_pressed);
              } else {
                set_state(Nothing_pressed);
              }
              redraw_later();
              return true;            }
            break;
          case ButtonRelease:
            switch (e.xbutton.button) {
              case Button1: {
                auto pt = core::point(e.xbutton);

                auto geo = get_geometry();

                switch (get_state()) {
                  case Up_button_pressed:
                    if (up_button_place(geo).is_inside(pt)) {
                      set_value(get_value() - 1, true);
                    }
                    break;
                  case Down_button_pressed:
                    if (down_button_place(geo).is_inside(pt)) {
                      set_value(get_value() + 1, true);
                    }
                    break;
                  case Page_up_pressed:
                    if (page_up_place(geo).is_inside(pt)) {
                      set_value(get_value() - get_step(), true);
                    }
                    break;
                  case Page_down_pressed:
                    if (page_down_place(geo).is_inside(pt)) {
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
            set_state(Nothing_pressed);
            redraw_later();
            break;
          case MotionNotify:
            if (left_button_bit_mask::is_set(e.xmotion.state)) {
              // check if on thumb
              if (get_state() == Thumb_button_pressed) {
                type delta = (type)(e.xmotion.x - last_mouse_point.x()) / get_scale();
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

      const std::string up_char = u8"\u2227";
      const std::string down_char = u8"\u2228";

      template<>
      bool scroll_barT<false>::scroll_handle_eventT (const core::event& e,
                                                     os::event_result& result) {
        switch (e.type) {
          case Expose: {
            draw::graphics g(e.xexpose.window, get_param<0, draw::graphics>(e));

            auto geo = get_geometry();
            auto up = up_button_place(geo);
            auto down = down_button_place(geo);
            auto thumb = thumb_button_place(geo);
            auto page_up = page_up_place(geo);
            auto page_down = page_down_place(geo);

            draw_scrollbar(g, up_char, down_char, up, down, thumb, page_up, page_down);

            return true;
          }
          case ButtonPress:
            if (e.xbutton.button == Button1) {
              last_mouse_point = core::point(e.xbutton);
              last_position = get_value();

              auto geo = get_geometry();

              if (up_button_place(geo).is_inside(last_mouse_point)) {
                set_state(Up_button_pressed);
              } else if (down_button_place(geo).is_inside(last_mouse_point)) {
                set_state(Down_button_pressed);
              } else if (thumb_button_place(geo).is_inside(last_mouse_point)) {
                set_state(Thumb_button_pressed);
              } else if (page_up_place(geo).is_inside(last_mouse_point)) {
                set_state(Page_up_pressed);
              } else if (page_down_place(geo).is_inside(last_mouse_point)) {
                set_state(Page_down_pressed);
              } else {
                set_state(Nothing_pressed);
              }
              redraw_later();
              return true;
            }
            break;
          case ButtonRelease:
            switch (e.xbutton.button) {
              case Button1: {
                auto pt = core::point(e.xbutton);

                auto geo = get_geometry();

                switch (get_state()) {
                  case Up_button_pressed:
                    if (up_button_place(geo).is_inside(pt)) {
                      set_value(get_value() - 1, true);
                    }
                    break;
                  case Down_button_pressed:
                    if (down_button_place(geo).is_inside(pt)) {
                      set_value(get_value() + 1, true);
                    }
                    break;
                  case Page_up_pressed:
                    if (page_up_place(geo).is_inside(pt)) {
                      set_value(get_value() - get_step(), true);
                    }
                    break;
                  case Page_down_pressed:
                    if (page_down_place(geo).is_inside(pt)) {
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
            set_state(Nothing_pressed);
            redraw_later();
            break;
          case MotionNotify:
            if (left_button_bit_mask::is_set(e.xmotion.state)) {
              // check if on thumb
              if (get_state() == Thumb_button_pressed) {
                type delta = (type)(e.xmotion.y - last_mouse_point.y()) / get_scale();
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

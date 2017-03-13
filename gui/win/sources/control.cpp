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


namespace gui {

  namespace win {

    window_class scroll_bar::clazz;

#ifdef WIN32
// --------------------------------------------------------------------------
    scroll_bar::scroll_bar ()
      : min(0)
      , max(100)
      , step(10)
      , current(0)
      , horizontal(false)
      , gc(0)
    {
      if (!clazz.is_valid()) {
        clazz = win::window_class::sub_class("MyScrollBar",
                                             "SCROLLBAR",
                                             SBS_VERT | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER,
                                             WS_EX_NOPARENTNOTIFY);
      }
    }

    scroll_bar::~scroll_bar ()
    {}

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

    int scroll_bar::get_current () const {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_POS, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      return si.nPos;
    }

    void scroll_bar::set_min (int i) {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.min = i;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_max (int i) {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.max = i;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_step (int i) {
      SCROLLINFO si = { sizeof( SCROLLINFO ), SIF_PAGE, 0, 0, i, 0, 0 };
      SetScrollInfo( get_id(), SB_CTL, &si, TRUE  );
    }

    void scroll_bar::set_current (int i) {
      i = std::min(std::max(i, min), max);
      if (i != get_current()) {
        SCROLLINFO si = {sizeof(SCROLLINFO), SIF_POS, 0, 0, 0, i, i};
        SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
      }
    }

#endif // WIN32

#ifdef X11

    // --------------------------------------------------------------------------
    scroll_bar::scroll_bar ()
      : min(0)
      , max(100)
      , step(10)
      , current(0)
      , horizontal(false)
      , gc(0)
    {
      if (!clazz.is_valid()) {
        clazz = window_class::custom_class("SCROLLBAR",
                                           0,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                           FocusChangeMask | KeyPressMask,
                                           0, 0, 0,
                                           draw::color::white);
      }
      register_event_handler(this, &scroll_bar::scroll_handle_event);
    }

    scroll_bar::~scroll_bar () {
      if (gc) {
        if (core::global::get_instance()) {
          XFreeGC(core::global::get_instance(), gc);
        }
        gc = 0;
      }
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

    int scroll_bar::get_current () const {
      return current;
    }

    void scroll_bar::set_min (int i) {
      min = i;
      current = std::max(current, min);
      redraw_later();
    }

    void scroll_bar::set_max (int i) {
      max = i;
      current = std::min(current, max);
      redraw_later();
    }

    void scroll_bar::set_step (int i) {
      step = i;
      redraw_later();
    }

    void scroll_bar::set_current (int i) {
      i = std::min(std::max(i, min), max);
      if (i != current) {
        current = i;
        redraw_later();
      }
    }

    bool scroll_bar::scroll_handle_event (const core::event& e,
                                          core::event_result& result) {
      switch (e.type) {
        case Expose: {
          if (!gc) {
            gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
          }
          draw::graphics g(e.xexpose.window, gc);
          core::rectangle place = client_area();
          place.bottom_right(place.bottom_right() - core::size(1, 1));

          core::rectangle btn = place;
          int width = btn.width();
          core::size::type y2 = width;
          core::size::type y3 = place.y2() - y2;

          btn.y(y2 + 1);
          int space = y3 - y2 - 2;
          btn.height(space);
          g.draw_relief(btn, true);

          btn.y(place.y());
          btn.height(y2);
          btn.shrink({1, 1});
          g.fill(draw::rectangle(btn), draw::color::buttonColor);
          g.draw_relief(btn, false);
          g.text(draw::text_box(u8" \u25b2", btn, draw::center), draw::font::system(), is_enabled() ? draw::color::black : draw::color::gray);

          btn.y(y3 + 1);
          g.fill(draw::rectangle(btn), draw::color::buttonColor);
          g.draw_relief(btn, false);
          g.text(draw::text_box(u8" \u25bc", btn, draw::center), draw::font::system(), is_enabled() ? draw::color::black : draw::color::gray);

          btn.y(current + y2 + 1);
          btn.height(std::max(space - get_max(), width));
          //btn.shrink({1, 1});
          g.fill(draw::rectangle(btn), draw::color::buttonColor);
          g.draw_relief(btn, false);

          XFlushGC(e.xexpose.display, gc);
          return true;
        }
        case ButtonPress:
          if (e.xbutton.button == Button1) {
            last_mouse_point = core::point(e.xbutton);
            return true;
          }
          break;
        case ButtonRelease:
          switch (e.xbutton.button) {
            case Button1: {
              auto pt = core::point(e.xbutton);
              // check, which area (up, page-up, thumb, page-down, down);
              core::rectangle place = client_area();
              place.bottom_right(place.bottom_right() - core::size(1, 1));

              core::rectangle btn = place;
              core::size::type y2 = btn.width();
              core::size::type y3 = place.y2() - y2;

              btn.height(y2);
              if (btn.is_inside(pt)) {
                set_current(get_current() - 1);
              } else {
                btn.y(y3);
                if (btn.is_inside(pt)) {
                  set_current(get_current() + 1);
                }
              }

              break;
            }
            case Button4: { // Y-Wheel
              set_current(get_current() - 1);
              return true;
            }
            case Button5: { // Y-Wheel
              set_current(get_current() + 1);
              return true;
            }
          }
          break;
        case MotionNotify:
          if ((e.xmotion.state & Button1Mask) == Button1Mask) {
            // check if on thumb
            int dy = e.xmotion.y - last_mouse_point.y();
            last_mouse_point = core::point(e.xmotion);
            set_current(get_current() + dy);
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
              set_current(get_current() - 1);
              return true;
            case XK_Down:
            case XK_KP_Down:
              set_current(get_current() + 1);
              return true;
            case XK_Page_Up:
            case XK_KP_Page_Up:
              set_current(get_current() - step);
              return true;
            case XK_Page_Down:
            case XK_KP_Page_Down:
              set_current(get_current() + step);
              return true;
            case XK_Home:
            case XK_KP_Home:
              set_current(min);
              return true;
            case XK_End:
            case XK_KP_End:
              set_current(max);
              return true;
          }
          break;
        }
      }
      return false;
    }

#endif // X11

  } // win

} // gui

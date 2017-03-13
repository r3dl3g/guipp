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
    scroll_bar::scroll_bar (bool horizontal) {
      if (!clazz.is_valid()) {
        clazz = win::window_class::sub_class("MyScrollBar",
                                             "SCROLLBAR",
                                             (horizontal ? SBS_HORI | WS_HSCROLL : SBS_VERT | WS_VSCROLL) |
                                              WS_CHILD | WS_VISIBLE,
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
      si.nMin = i;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_max (int i) {
      SCROLLINFO si = {sizeof(SCROLLINFO), SIF_RANGE, 0, 0, 0, 0, 0};
      GetScrollInfo(get_id(), SB_CTL, &si);
      si.nMax = i;
      SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
    }

    void scroll_bar::set_step (int i) {
      SCROLLINFO si = { sizeof( SCROLLINFO ), SIF_PAGE, 0, 0, (UINT)i, 0, 0 };
      SetScrollInfo( get_id(), SB_CTL, &si, TRUE  );
    }

    void scroll_bar::set_current (int i) {
      if (i != get_current()) {
        SCROLLINFO si = {sizeof(SCROLLINFO), SIF_POS, 0, 0, 0, i, i};
        SetScrollInfo(get_id(), SB_CTL, &si, TRUE);
      }
    }

#endif // WIN32

#ifdef X11

    // --------------------------------------------------------------------------
    scroll_bar::scroll_bar (bool horizontal)
      : min(0)
      , max(100)
      , step(10)
      , current(0)
      , horizontal(horizontal)
      , state(Nothing_pressed)
      , gc(0)
    {
      if (!clazz.is_valid()) {
        clazz = window_class::custom_class("SCROLLBAR",
                                           0,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                           FocusChangeMask | KeyPressMask,
                                           0, 0, 0,
                                           draw::color::veryLightGray);
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

    core::size::type scroll_bar::button_size (const core::rectangle& place) const {
      return horizontal ? place.height() : place.width();
    }

    core::rectangle scroll_bar::up_button_place (const core::rectangle& place) const {
      core::size::type sz = button_size(place);
      return core::rectangle(place.x(), place.y(), sz, sz);
    }

    core::rectangle scroll_bar::down_button_place (const core::rectangle& place) const {
      core::size::type sz = button_size(place);
      return horizontal ? core::rectangle(place.x2() - sz, place.y(), sz, sz)
                        : core::rectangle(place.x(), place.y2() - sz, sz, sz);
    }

    core::rectangle scroll_bar::page_up_place (const core::rectangle& place) const {
      core::size::type sz = button_size(place);
      core::size::type bottom = (core::size::type)thumb_top(place);
      return horizontal ? core::rectangle(sz, place.y(), bottom - sz, sz)
                        : core::rectangle(place.x(), sz, sz, bottom - sz);
    }

    core::rectangle scroll_bar::page_down_place (const core::rectangle& place) const {
      core::size::type sz = button_size(place);
      core::point::type top = thumb_top(place) + thumb_size(place);
      return horizontal ? core::rectangle(top, place.y(), place.x2() - sz - top, sz)
                        : core::rectangle(place.x(), top, sz, place.y2() - sz - top);
    }

    core::point::type scroll_bar::thumb_top (const core::rectangle& place) const {
      core::size::type sz = button_size(place);
      return core::point::type(sz + current + 1);
    }

    core::size::type scroll_bar::thumb_size (const core::rectangle& place) const {
      core::size::type sz = button_size(place);
      return std::max(core::size::type((horizontal ? place.width()
                                                   : place.height()) - sz * 2 - 2 - get_max()), sz);
    }

    core::rectangle scroll_bar::thumb_button_place (const core::rectangle& place) const {
      core::size::type sz = button_size(place);
      core::size::type tsz = thumb_size(place);
      core::point::type pos = thumb_top(place);
      return horizontal ? core::rectangle(pos, place.y(), tsz, sz)
                        : core::rectangle(place.x(), pos, sz, tsz);
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

          auto up = up_button_place(place);
          g.fill(draw::rectangle(up), draw::color::buttonColor);
          g.draw_relief(up, state == Up_button_pressed);
          g.text(draw::text_box(horizontal ? " < " : u8" \u2227 ", up, draw::center),
                 draw::font::system(), is_enabled() ? draw::color::black : draw::color::gray);

          auto down = down_button_place(place);
          g.fill(draw::rectangle(down), draw::color::buttonColor);
          g.draw_relief(down, state == Down_button_pressed);
          g.text(draw::text_box(horizontal ? " > " : u8" \u2228 ", down, draw::center),
                 draw::font::system(), is_enabled() ? draw::color::black : draw::color::gray);

          auto thumb = thumb_button_place(place);
          g.fill(draw::rectangle(thumb), draw::color::buttonColor);
          g.draw_relief(thumb, false);

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
                    set_current(get_current() - 1);
                  }
                  break;
                case Down_button_pressed:
                  if (down_button_place(place).is_inside(pt)) {
                    set_current(get_current() + 1);
                  }
                  break;
                case Page_up_pressed:
                  if (page_up_place(place).is_inside(pt)) {
                    set_current(get_current() - step);
                  }
                  break;
                case Page_down_pressed:
                  if (page_down_place(place).is_inside(pt)) {
                    set_current(get_current() + step);
                  }
                  break;
              }
              break;
            }
            case Button4: { // Y-Wheel
              if (!horizontal) {
                set_current(get_current() - 1);
              }
              return true;
            }
            case Button5: { // Y-Wheel
              if (!horizontal) {
                set_current(get_current() + 1);
              }
              return true;
            }
            case 6: { // X-Wheel
              if (horizontal) {
                set_current(get_current() - 1);
              }
              return true;
            }
            case 7: { // X-Wheel
              if (horizontal) {
                set_current(get_current() + 1);
              }
              return true;
            }
          }
          state = Nothing_pressed;
          redraw_later();
          break;
        case MotionNotify:
          if ((e.xmotion.state & Button1Mask) == Button1Mask) {
            // check if on thumb
            if (state == Thumb_button_pressed) {
              int delta = horizontal ?  e.xmotion.x - last_mouse_point.x()
                                     : e.xmotion.y - last_mouse_point.y();
              last_mouse_point = core::point(e.xmotion);
              set_current(get_current() + delta);
            }
            return true;
          }
          break;
        case KeyPress: {
          KeySym key;
          char text[8] = {0};
          XLookupString(const_cast<XKeyEvent*>(&e.xkey), text, 8, &key, 0);
          if (horizontal) {
            switch (key) {
              case XK_Left:
              case XK_KP_Left:
                set_current(get_current() - 1);
                return true;
              case XK_Right:
              case XK_KP_Right:
                set_current(get_current() + 1);
                return true;
            }
          } else {
            switch (key) {
              case XK_Up:
              case XK_KP_Up:
                set_current(get_current() - 1);
                return true;
              case XK_Down:
              case XK_KP_Down:
                set_current(get_current() + 1);
                return true;
            }
          }
          switch (key) {
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

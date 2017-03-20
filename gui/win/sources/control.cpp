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

    namespace detail {

      template <>
      sliderT<false>::sliderT () {
        if (!clazz.is_valid()) {
#ifdef WIN32
          clazz = win::window_class::custom_class("SLIDER",
                                                  0,
                                                  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                                  WS_EX_NOPARENTNOTIFY,
                                                  nullptr,
                                                  LoadCursor(nullptr, IDC_ARROW),
                                                  (HBRUSH)(COLOR_BUTTON + 1));
#endif // WIN32
#ifdef X11
          core::cursor_id cursor = XCreateFontCursor(core::global::get_instance(), 108);
          clazz = window_class::custom_class("SLIDER",
                                             0,
                                             ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                             StructureNotifyMask,
                                             0, 0, cursor,
                                             draw::color::buttonColor);
#endif // X11
        }
        register_event_handler(win::mouse_move_event([&] (unsigned int keys,
                                                          const core::point& p) {
          if (keys == IF_WIN32(MK_LBUTTON) IF_X11(Button1Mask)) {
            core::point pt = position();
            int dx = p.x() - last_mouse_point.x();
            pt.x(core::point::type(std::min(max, std::max(min, pt.x() + dx))));
            move(pt);
          }
        }));
      }

      template <>
      sliderT<true>::sliderT () {
        if (!clazz.is_valid()) {
#ifdef WIN32
          clazz = win::window_class::custom_class("SLIDER",
                                                  0,
                                                  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                                  WS_EX_NOPARENTNOTIFY,
                                                  nullptr,
                                                  LoadCursor(nullptr, IDC_ARROW),
                                                  (HBRUSH)(COLOR_BUTTON + 1));
#endif // WIN32
#ifdef X11
          core::cursor_id cursor = XCreateFontCursor(core::global::get_instance(), 116);
          clazz = window_class::custom_class("SLIDER",
                                             0,
                                             ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                             StructureNotifyMask,
                                             0, 0, cursor,
                                             draw::color::buttonColor);
#endif // X11
        }
        register_event_handler(win::mouse_move_event([&] (unsigned int keys,
                                                          const core::point& p) {
          if (keys == IF_WIN32(MK_LBUTTON) IF_X11(Button1Mask)) {
            core::point pt = position();
            int dy = p.y() - last_mouse_point.y();
            pt.y(core::point::type(std::min(max, std::max(min, pt.y() + dy))));
            move(pt);
          }
        }));
      }
    }

    slider::slider ()
      : min(0)
      , max(std::numeric_limits<int>::max())
#ifdef X11
      , gc(0)
#endif // X11
    {
      register_event_handler(this, &slider::slider_handle_event);
    }

    slider::~slider () {
#ifdef X11
      if (gc) {
        if (core::global::get_instance()) {
          XFreeGC(core::global::get_instance(), gc);
        }
        gc = 0;
      }
#endif
    }

    void slider::set_min (int i) {
      min = i;
      redraw_later();
    }

    void slider::set_max (int i) {
      max = i;
      redraw_later();
    }

    void slider::set_min_max (int mi, int ma) {
      min = mi;
      max = ma;
      redraw_later();
    }

    bool slider::slider_handle_event (const core::event& e, core::event_result& result) {
      switch (e.type) {
        case Expose: {
#ifdef X11
          if (!gc) {
            gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
          }
          draw::graphics g(e.xexpose.window, gc);
#endif
#ifdef WIN32
          PAINTSTRUCT ps;
          core::graphics_id id = BeginPaint(e.id, &ps);
          draw::graphics g(e.id, id);
#endif
          core::rectangle place = client_area();
          g.fill(draw::rectangle(place), draw::color::buttonColor);
          g.draw_relief(place, false);

#ifdef WIN32
          EndPaint(e.id, &ps);
#endif
          return true;
        }
        case ButtonPress:
          if (e.xbutton.button == Button1) {
            last_mouse_point = core::point(e.xbutton);
            return true;
          }
          break;
        case ButtonRelease:
          if (e.xbutton.button == Button1) {
            last_mouse_point = {-1, -1};
            return true;
          }
          break;
        default:
          return false;
      }
    }

  } // win

} // gui

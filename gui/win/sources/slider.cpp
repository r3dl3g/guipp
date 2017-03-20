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
#include "slider.h"


namespace gui {

  namespace win {

    namespace detail {

      template <>
      sliderT<false>::sliderT () {
        if (!clazz.is_valid()) {
#ifdef WIN32
          clazz = win::window_class::custom_class("VSLIDER",
                                                  0,
                                                  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                                  WS_EX_NOPARENTNOTIFY,
                                                  nullptr,
                                                  LoadCursor(nullptr, IDC_SIZEWE),
                                                  (HBRUSH)(COLOR_BTNFACE + 1));
#endif // WIN32
#ifdef X11
          core::cursor_id cursor = XCreateFontCursor(core::global::get_instance(), 108);
          clazz = window_class::custom_class("VSLIDER",
                                             0,
                                             ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                             StructureNotifyMask,
                                             0, 0, cursor,
                                             draw::color::buttonColor);
#endif // X11
        }
        register_event_handler(win::mouse_move_event([&] (unsigned int keys,
                                                          const core::point& p) {
          if ((last_mouse_point != core::point::undefined) && (keys == IF_WIN32(MK_LBUTTON) IF_X11(Button1Mask))) {
            core::point pt = position();
            int dx = p.x() - last_mouse_point.x();
            pt.x(core::point::type(std::min(max, std::max(min, (int)pt.x() + dx))));
            move(pt);
          }
        }));
      }

      template <>
      sliderT<true>::sliderT () {
        if (!clazz.is_valid()) {
#ifdef WIN32
          clazz = win::window_class::custom_class("HSLIDER",
                                                  0,
                                                  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                                  WS_EX_NOPARENTNOTIFY,
                                                  nullptr,
                                                  LoadCursor(nullptr, IDC_SIZENS),
                                                  (HBRUSH)(COLOR_BTNFACE + 1));
#endif // WIN32
#ifdef X11
          core::cursor_id cursor = XCreateFontCursor(core::global::get_instance(), 116);
          clazz = window_class::custom_class("HSLIDER",
                                             0,
                                             ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                             StructureNotifyMask,
                                             0, 0, cursor,
                                             draw::color::buttonColor);
#endif // X11
        }
        register_event_handler(win::mouse_move_event([&] (unsigned int keys,
                                                          const core::point& p) {
          if ((last_mouse_point != core::point::undefined) && (keys == IF_WIN32(MK_LBUTTON) IF_X11(Button1Mask))) {
            core::point pt = position();
            int dy = p.y() - last_mouse_point.y();
            pt.y(core::point::type(std::min(max, std::max(min, (int)pt.y() + dy))));
            move(pt);
          }
          return;
        }));
      }
    }

    slider::slider ()
      : min(0)
      , max(std::numeric_limits<int>::max())
    {
      register_event_handler(win::left_btn_down_event([&](const core::point& pt) {
        IF_WIN32(SetCapture(get_id()));
        last_mouse_point = pt;
      }));
      register_event_handler(win::left_btn_up_event([&](const core::point& pt) {
        IF_WIN32(ReleaseCapture());
        last_mouse_point = core::point::undefined;
      }));
      register_event_handler(win::paint_event([&](draw::graphics& g) {
        core::rectangle place = client_area();
        g.fill(draw::rectangle(place), draw::color::buttonColor);
        g.draw_relief(place, false);
      }));
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

  } // win

} // gui

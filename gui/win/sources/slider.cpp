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
      slider_t<false>::slider_t () {
        if (!clazz.is_valid()) {
#ifdef WIN32
          clazz = win::window_class::custom_class("VSLIDER",
                                                  0,
                                                  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                                  WS_EX_NOPARENTNOTIFY,
                                                  nullptr,
                                                  LoadCursor(nullptr, IDC_SIZEWE),
                                                  //CreateSolidBrush(0xff0000));
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
          if ((last_mouse_point != core::point::undefined) && left_button_bit_mask::is_set(keys)) {
            core::point pt = position();
            core::point::type new_x = std::min<core::point::type>(max, std::max<core::point::type>(min, pt.x() + p.x() - last_mouse_point.x()));
            core::point::type dx = new_x - pt.x();
            if (dx != 0) {
              pt.x(new_x);
              move(pt);
              send_client_message(this, detail::SLIDER_MESSAGE, os(dx));
            }
          }
        }));
      }

      template <>
      slider_t<true>::slider_t () {
        if (!clazz.is_valid()) {
#ifdef WIN32
          clazz = win::window_class::custom_class("HSLIDER",
                                                  0,
                                                  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                                  WS_EX_NOPARENTNOTIFY,
                                                  nullptr,
                                                  LoadCursor(nullptr, IDC_SIZENS),
                                                  //CreateSolidBrush(0x00ff00));
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
          if ((last_mouse_point != core::point::undefined) && left_button_bit_mask::is_set(keys)) {
            core::point pt = position();
            core::point::type new_y = std::min<core::point::type>(max, std::max<core::point::type>(min, pt.y() + p.y() - last_mouse_point.y()));
            core::point::type dy = new_y - pt.y();
            if (dy != 0) {
              pt.y(new_y);
              move(pt);
              send_client_message(this, detail::SLIDER_MESSAGE, os(dy));
            }
           }
          return;
        }));
      }

#ifdef X11
      Atom SLIDER_MESSAGE = 0;
#endif // X11
//#ifdef WIN32
//      core::event_id SLIDER_MESSAGE = 0;
//#endif //WIN32

      slider::slider ()
        : min(0)
        , max(std::numeric_limits<type>::max())
      {
#ifdef WIN32
//        if (!detail::SLIDER_MESSAGE) {
//          detail::SLIDER_MESSAGE = RegisterWindowMessage("SLIDER_MESSAGE");
//        }
#endif //WIN32
#ifdef X11
        if (!detail::SLIDER_MESSAGE) {
          detail::SLIDER_MESSAGE = XInternAtom(core::global::get_instance(), "SLIDER_MESSAGE", False);
        }
#endif // X11
        register_event_handler(left_btn_down_event([&](const core::point& pt) {
          IF_WIN32(SetCapture(get_id()));
          IF_X11(XGrabPointer(core::global::get_instance(), get_id(), False,
                              ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                              GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
          last_mouse_point = pt;
        }));
        register_event_handler(left_btn_up_event([&](const core::point& pt) {
          IF_WIN32(ReleaseCapture());
          IF_X11(XUngrabPointer(core::global::get_instance(), CurrentTime));
          last_mouse_point = core::point::undefined;
        }));
      }

      void slider::set_min (type i) {
        min = i;
        redraw_later();
      }

      void slider::set_max (type i) {
        max = i;
        redraw_later();
      }

      void slider::set_min_max (type mi, type ma) {
        min = mi;
        max = ma;
        redraw_later();
      }

    } // detail

  } // win

} // gui

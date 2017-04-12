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
* @brief     C++ API: label
*
* @file
*/

#include "label.h"

//#define SHOW_TEXT_AREA

namespace gui {

  namespace win {

    namespace detail {

      window_class label_base::create_label_class (win::alignment_h a) {
#ifdef WIN32
        return window_class::sub_class("MyStatic", "STATIC",
                                        a | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
                                        WS_EX_NOPARENTNOTIFY);
#endif // WIN32
#ifdef X11
        return gui::win::window_class::custom_class("STATIC",
                                                    0,
                                                    ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                                    PointerMotionMask |
                                                    StructureNotifyMask | SubstructureRedirectMask |
                                                    FocusChangeMask |
                                                    EnterWindowMask | LeaveWindowMask,
                                                    a, 0, 0,
                                                    draw::brush(draw::color::buttonColor()));
#endif // X11
      }

      void label_base::register_handler () {
#ifdef X11
        register_event_handler(paint_event([&] (const draw::graphics& graph) {
          using namespace gui::draw;
          gui::core::rectangle area = client_area();
          text_origin origin = (text_origin)get_window_class()->get_ex_style();
          graph.text(draw::text_box(text, area, origin),
                     font::system(),
                     color::black());
#ifdef SHOW_TEXT_AREA
          graph.text(draw::bounding_box(text, area, origin), font::system(), color::black());
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
#endif // SHOW_TEXT_AREA
        }));
#endif // X11
      }
    } // detail

  } // win

} // gui

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

      window_class label_base::clazz("STATIC++", 
#ifdef WIN32
        (COLOR_BTNFACE+1)
#endif / WIN32
#ifdef X11
        os::get_sys_color(os::COLOR_BTNFACE)
#endif // X11
      );

    } // detail

    namespace paint {

      void label (const draw::graphics& graph,
                  const win::window& win,
                  const std::string& text,
                  draw::text_origin origin) {
        using namespace gui::draw;
        gui::core::rectangle area = win.client_area();
        graph.text(draw::text_box(text, area, origin),
                   font::system(),
                   color::black());
#ifdef SHOW_TEXT_AREA
        graph.text(draw::bounding_box(text, area, origin), font::system(), color::black());
        graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
#endif // SHOW_TEXT_AREA
      }

    }

  } // win

} // gui

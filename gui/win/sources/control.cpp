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

#ifdef WIN32
#endif // WIN32

#ifdef X11
#endif // X11

  namespace layout {
  }

  namespace win {

    namespace detail {

    } // detail

#ifdef WIN32

    // --------------------------------------------------------------------------
    template<>
    draw::graphics get_param<0, draw::graphics>(const core::event& e) {
      return draw::graphics(e.id, (os::graphics)e.param_1);
    }

    // --------------------------------------------------------------------------
    void paint_caller::operator()(const core::event& e) {
      if (f) {
        PAINTSTRUCT ps;
        os::graphics id = BeginPaint(e.id, &ps);
        f(draw::graphics(e.id, id));
        EndPaint(e.id, &ps);
      }
    }

#endif // WIN32
#ifdef X11

    void paint_caller::operator()(const core::event& e) {
      if (callback) {
        if (!gc) {
          gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
        }
        draw::graphics g(e.xexpose.window, gc);
        callback(g);
        //        XFlushGC(e.xexpose.display, gc);
      }
    }

#endif // X11


  } // win

} // gui

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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"
#include "window_event_handler.h"
#include "graphics.h"


namespace gui {

  namespace win {

    namespace detail {
    }

    // --------------------------------------------------------------------------
    template<>
    draw::graphics get_param<0, draw::graphics>(const core::event& e);

#ifdef WIN32
    // --------------------------------------------------------------------------
    struct paint_caller : Params<draw::graphics>::caller<get_param<0, draw::graphics>> {
      typedef Params<draw::graphics>::caller<get_param<0, draw::graphics>> super;
      typedef super::function function;

      paint_caller(const function cb)
        :super(cb)
      {}

      template<class T>
      paint_caller(T* t, void(T::*callback_)(const draw::graphics&))
        : super(gui::core::bind_method(t, callback_))
      {}

      void operator()(const core::event& e);
    };

    // --------------------------------------------------------------------------
    typedef event_handler<WM_PAINT, paint_caller> paint_event;

#endif // WIN32

#ifdef X11

    typedef event_handler<Expose,
                          Params<draw::graphics>::
                          caller<get_param<0, draw::graphics>>>   paint_event;

#endif // X11

  } // win

} // gui

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
#include<ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"
#include "window_event_handler.h"
#include "graphics.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {
#ifdef WIN32
      const os::event_id SLIDER_MESSAGE = WM_USER + 1;
      const os::event_id SELECTION_CHANGE_MESSAGE = WM_USER + 2;
      const os::event_id SELECTION_COMMIT_MESSAGE = WM_USER + 3;
      const os::event_id HILITE_CHANGE_MESSAGE = WM_USER + 4;
      const os::event_id BN_CLICKED_MESSAGE = WM_USER + 5;
      const os::event_id BN_PUSHED_MESSAGE = WM_USER + 6;
      const os::event_id BN_UNPUSHED_MESSAGE = WM_USER + 7;
      const os::event_id BN_STATE_MESSAGE = WM_USER + 8;
      const os::event_id SCROLLBAR_MESSAGE = WM_USER + 9;
#endif //WIN32
#ifdef X11
      extern Atom SCROLLBAR_MESSAGE;
      extern Atom SELECTION_CHANGE_MESSAGE;
      extern Atom SELECTION_COMMIT_MESSAGE;
      extern Atom HILITE_CHANGE_MESSAGE;
      extern Atom SLIDER_MESSAGE;
      extern Atom BN_CLICKED_MESSAGE;
      extern Atom BN_PUSHED_MESSAGE;
      extern Atom BN_UNPUSHED_MESSAGE;
      extern Atom BN_STATE_MESSAGE;

      void init_control_messages ();
#endif // X11
    } // detail

    // --------------------------------------------------------------------------
    class no_erase_window_class : public window_class {
    public:
      no_erase_window_class ()
      {}

      no_erase_window_class (const window_class& rhs)
        :window_class(rhs)
      {}

      no_erase_window_class (const std::string& cls_name,
                             os::color background = color::white,
                             os::cursor_type cursor = window_class_defaults<os::system_platform>::cursor,
                             os::style style = window_class_defaults<os::system_platform>::style,
                             os::style ex_style = window_class_defaults<os::system_platform>::ex_style)
        :window_class(cls_name, background, cursor, style, ex_style)
      {}

      void prepare (window*, os::window) const override;

    protected:
      os::brush get_background_brush () const override;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    inline std::string convert_to_string(const T& t) {
      return ostreamfmt(t);
    }

    template<>
    inline std::string convert_to_string<std::string>(const std::string& t) {
      return t;
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
    typedef event_handler<WM_PAINT, 0, paint_caller> paint_event;

#endif // WIN32

#ifdef X11

    typedef event_handler<Expose, ExposureMask,
                          Params<draw::graphics>::
                          caller<get_param<0, draw::graphics>>>   paint_event;

#endif // X11

    namespace paint {
      void text_item (const std::string& text,
                      const draw::graphics&,
                      const core::rectangle& place,
                      const draw::brush& background,
                      bool selected,
                      text_origin origin = text_origin::vcenter_left);
    }

  } // win

} // gui

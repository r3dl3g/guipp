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
      const os::event_id BN_CLICKED_MESSAGE = WM_USER + 3;
      const os::event_id BN_PUSHED_MESSAGE = WM_USER + 4;
      const os::event_id BN_UNPUSHED_MESSAGE = WM_USER + 5;
      const os::event_id BN_STATE_MESSAGE = WM_USER + 6;
      const os::event_id SCROLLBAR_MESSAGE = WM_USER + 7;
#endif //WIN32
#ifdef X11
      extern Atom SCROLLBAR_MESSAGE;
      extern Atom SELECTION_CHANGE_MESSAGE;
      extern Atom SLIDER_MESSAGE;
      extern Atom BN_CLICKED_MESSAGE;
      extern Atom BN_PUSHED_MESSAGE;
      extern Atom BN_UNPUSHED_MESSAGE;
      extern Atom BN_STATE_MESSAGE;
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
                             os::cursor_type cursor = IF_WIN32(IDC_ARROW) IF_X11(0),
                             os::style class_style = IF_WIN32(CS_DBLCLKS) IF_X11(0),
                             os::style style = IF_X11(0) IF_WIN32(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP),
                             os::style ex_style = IF_X11(0) IF_WIN32(WS_EX_NOPARENTNOTIFY),
                             os::event_callback callback = nullptr)
        :window_class(cls_name, background, cursor, class_style, style, ex_style, callback)
      {}

      void prepare (window*) const override;

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
                      draw::text_origin origin = draw::vcenter_left);
    }

  } // win

} // gui

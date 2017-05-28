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
                             os::cursor_type cursor = window_class_defaults<>::cursor,
                             os::style style = window_class_defaults<>::style,
                             os::style ex_style = window_class_defaults<>::ex_style)
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

    enum class event_source {
      keyboard,
      mouse,
      logic
    };
#ifdef WIN32
    // --------------------------------------------------------------------------
    struct paint_caller : params<draw::graphics>::caller<get_param<0, draw::graphics>> {
      typedef params<draw::graphics>::caller<get_param<0, draw::graphics>> super;
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
    using paint_event = event_handler<WM_PAINT, 0, paint_caller>;


    using selection_changed_event = event_handler<detail::SELECTION_CHANGE_MESSAGE, 0,
                                                  params<event_source>::
                                                  caller<get_param<0, event_source>>>;

    using selection_commit_event = event_handler<detail::SELECTION_COMMIT_MESSAGE>;

    using hilite_changed_event = event_handler<detail::HILITE_CHANGE_MESSAGE, 0,
                                               params<bool>::caller<get_param<0, bool>>>;

#endif //WIN32
    // --------------------------------------------------------------------------
#ifdef X11
    using paint_event = event_handler<Expose, ExposureMask,
                          params<draw::graphics>::
                          caller<get_param<0, draw::graphics>>>;

    using selection_changed_event = event_handler<ClientMessage, 0,
                          params<event_source>::
                          caller<get_client_data<0, event_source>>, 0,
                          client_message_matcher<detail::SELECTION_CHANGE_MESSAGE>>;

    using selection_commit_event = event_handler<ClientMessage, 0,
                          params<>::caller<>, 0,
                          client_message_matcher<detail::SELECTION_COMMIT_MESSAGE>>;

    using hilite_changed_event = event_handler<ClientMessage, 0,
                          params<bool>::caller<get_client_data<0, bool>>, 0,
                          client_message_matcher<detail::HILITE_CHANGE_MESSAGE>>;
#endif // X11
    // --------------------------------------------------------------------------

    namespace paint {
      void text_item (const draw::graphics&,
                      const core::rectangle& place,
                      const draw::brush& background,
                      const std::string& text,
                      bool selected,
                      text_origin origin = text_origin::vcenter_left);
    }

  } // win

} // gui

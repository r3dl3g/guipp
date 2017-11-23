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
// Library includes
//
#include <gui/core/ostreamfmt.h>
#include <gui/win/window.h>
#include <gui/win/window_event_handler.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>


namespace gui {

  // --------------------------------------------------------------------------

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
      const os::event_id SELECTION_CANCEL_MESSAGE = WM_USER + 10;
      const os::event_id CONTENT_CHANGED_MESSAGE = WM_USER + 10;
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
      extern Atom SELECTION_CANCEL_MESSAGE;
      extern Atom CONTENT_CHANGED_MESSAGE;

      int init_control_messages ();
#endif // X11
    } // detail

    // --------------------------------------------------------------------------
    template<typename T,
             win::cursor_type C = window_class_defaults<>::cursor,
             os::style S = window_class_defaults<>::style,
             os::style ES = window_class_defaults<>::ex_style,
             os::style CS = window_class_defaults<>::class_style>
    using no_erase_window_class = window_class<T, color::transparent, C, S, ES, CS>;

    // --------------------------------------------------------------------------
    template<typename T>
    inline std::string convert_to_string (const T& t) {
      return ostreamfmt(t);
    }

    template<>
    inline std::string convert_to_string<std::string>(const std::string& t) {
      return t;
    }

    // --------------------------------------------------------------------------
    typedef std::string (text_source_fn)();
    typedef std::function<text_source_fn> text_source;

    // --------------------------------------------------------------------------
    struct const_text {
      const_text ()
      {}

      const_text (const std::string& text)
        : text(text)
      {}

      const std::string& operator() () const {
        return text;
      }

    private:
      std::string text;
    };

    // --------------------------------------------------------------------------
    enum class event_source {
      keyboard,
      mouse,
      logic
    };

#ifdef WIN32
    template<>
    event_source get_param<0, event_source>(const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    draw::graphics get_param<0, draw::graphics>(const core::event& e);

    // --------------------------------------------------------------------------
    struct paint_caller : core::params<draw::graphics>::getter<get_param<0, draw::graphics> > {
      typedef core::params<draw::graphics>::getter<get_param<0, draw::graphics> > super;
      typedef super::function function;

      paint_caller (const function cb)
        : super(cb)
      {}

      template<class T>
      paint_caller (T* t, void(T::*callback_)(const draw::graphics &))
        : super(gui::core::bind_method(t, callback_))
      {}

      void operator() (const core::event& e);
    };

    // --------------------------------------------------------------------------
    using paint_event = core::event_handler<WM_PAINT, 0, paint_caller>;


    using selection_changed_event = core::event_handler<detail::SELECTION_CHANGE_MESSAGE, 0,
                                                  core::params<event_source>::
                                                  getter<get_param<0, event_source> > >;

    using selection_commit_event = core::event_handler<detail::SELECTION_COMMIT_MESSAGE>;

    using selection_cancel_event = core::event_handler<detail::SELECTION_CANCEL_MESSAGE>;

    using hilite_changed_event = core::event_handler<detail::HILITE_CHANGE_MESSAGE, 0,
                                               core::params<bool>::getter<get_param<0, bool> > >;

    using content_changed_event = core::event_handler<detail::CONTENT_CHANGED_MESSAGE>;

#endif //WIN32
       // --------------------------------------------------------------------------
#ifdef X11
    // --------------------------------------------------------------------------
    draw::graphics get_draw_graphics (const core::event& e);

    using paint_event = core::event_handler<Expose, ExposureMask,
                                      core::params<draw::graphics>::
                                      getter<get_draw_graphics> >;

    using selection_changed_event = core::event_handler<ClientMessage, 0,
                                                  core::params<event_source>::
                                                  getter<get_client_data<0, event_source> >,
                                                  0,
                                                  client_message_matcher<detail::SELECTION_CHANGE_MESSAGE> >;

    using selection_commit_event = core::event_handler<ClientMessage, 0,
                                                 core::params<>::
                                                 getter<>,
                                                 0,
                                                 client_message_matcher<detail::SELECTION_COMMIT_MESSAGE> >;

    using selection_cancel_event = core::event_handler<ClientMessage, 0,
                                                 core::params<>::
                                                 getter<>,
                                                 0,
                                                 client_message_matcher<detail::SELECTION_CANCEL_MESSAGE> >;

    using hilite_changed_event = core::event_handler<ClientMessage, 0,
                                               core::params<bool>::
                                               getter<get_client_data<0, bool> >,
                                               0,
                                               client_message_matcher<detail::HILITE_CHANGE_MESSAGE> >;

    using content_changed_event = core::event_handler<ClientMessage, 0,
                                                core::params<>::
                                                getter<>,
                                                0,
                                                client_message_matcher<detail::CONTENT_CHANGED_MESSAGE> >;

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

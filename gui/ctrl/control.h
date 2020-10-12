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
#include <util/ostreamfmt.h>
#include <gui/win/window.h>
#include <gui/win/window_event_handler.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>
#include <gui++-ctrl-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace ctrl {

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
      win::cursor_type C = win::window_class_defaults<>::cursor,
      os::style S = win::window_class_defaults<>::style,
      os::style ES = win::window_class_defaults<>::ex_style,
      os::style CS = win::window_class_defaults<>::class_style>
      using no_erase_window_class = win::window_class<T, color::transparent, C, S, ES, CS>;

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

    // --------------------------------------------------------------------------
    enum class item_state {
      normal,
      disabled,
      hilited,
      selected
    };

#ifdef WIN32
  } // namespace ctrl

  namespace win {

    template<>
    GUIPP_CTRL_EXPORT ctrl::event_source get_param<0, ctrl::event_source>(const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT draw::graphics get_param<0, draw::graphics>(const core::event& e);

  } // namespace win

  namespace ctrl {
    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT paint_caller : core::params<draw::graphics>::getter<win::get_param<0, draw::graphics> > {
      typedef core::params<draw::graphics>::getter<win::get_param<0, draw::graphics> > super;
      typedef super::function function;

      paint_caller (const function cb)
        : super(cb)
      {}

      template<class T>
      paint_caller (T* t, void(T::*callback_)(const draw::graphics &))
        : super(util::bind_method(t, callback_))
      {}

      void operator() (const core::event& e);
    };

    // --------------------------------------------------------------------------
    using paint_event = core::event_handler<WM_PAINT, 0, paint_caller>;


    using selection_changed_event = core::event_handler<detail::SELECTION_CHANGE_MESSAGE, 0,
                                                  core::params<event_source>::
                                                  getter<win::get_param<0, event_source> > >;

    using selection_commit_event = core::event_handler<detail::SELECTION_COMMIT_MESSAGE>;

    using selection_cancel_event = core::event_handler<detail::SELECTION_CANCEL_MESSAGE>;

    using hilite_changed_event = core::event_handler<detail::HILITE_CHANGE_MESSAGE, 0,
                                               core::params<bool>::getter<win::get_param<0, bool> > >;

    using content_changed_event = core::event_handler<detail::CONTENT_CHANGED_MESSAGE>;

#endif //WIN32

    // --------------------------------------------------------------------------
    using paint_function = std::function<void(draw::graphics)>;

#ifdef X11
    // --------------------------------------------------------------------------

    using selection_changed_event = core::event_handler<ClientMessage, 0,
                                                        core::params<event_source>::
                                                        getter<win::get_client_data<0, event_source> >,
                                                        0,
                                                        win::event::functor<win::client_message_matcher<detail::SELECTION_CHANGE_MESSAGE>>>;

    using selection_commit_event = core::event_handler<ClientMessage, 0,
                                                       core::params<>::
                                                       getter<>,
                                                       0,
                                                       win::event::functor<win::client_message_matcher<detail::SELECTION_COMMIT_MESSAGE>>>;

    using selection_cancel_event = core::event_handler<ClientMessage, 0,
                                                       core::params<>::
                                                       getter<>,
                                                       0,
                                                       win::event::functor<win::client_message_matcher<detail::SELECTION_CANCEL_MESSAGE>>>;

    using hilite_changed_event = core::event_handler<ClientMessage, 0,
                                                     core::params<bool>::
                                                     getter<win::get_client_data<0, bool> >,
                                                     0,
                                                     win::event::functor<win::client_message_matcher<detail::HILITE_CHANGE_MESSAGE>>>;

    using content_changed_event = core::event_handler<ClientMessage, 0,
                                                      core::params<>::
                                                      getter<>,
                                                      0,
                                                      win::event::functor<win::client_message_matcher<detail::CONTENT_CHANGED_MESSAGE>>>;

#endif // X11
       // --------------------------------------------------------------------------

    namespace paint {

      GUIPP_CTRL_EXPORT void text_item (const draw::graphics&,
                      const core::rectangle& place,
                      const draw::brush& background,
                      const std::string& text,
                      item_state state,
                      text_origin_t origin_t = text_origin_t::vcenter_left);

    }

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT control : public win::window {
    public:
      typedef win::window super;

      void on_selection_changed (std::function<void(event_source)>&& f);
      void on_selection_commit (std::function<void()>&& f);
      void on_selection_cancel (std::function<void()>&& f);
      void on_hilite_changed (std::function<void(bool)>&& f);
      void on_content_changed (std::function<void()>&& f);

      void notify_content_changed () const;

    };

    // --------------------------------------------------------------------------
    template<os::color background = color::very_light_gray>
    class client_control : public control {
    public:
      typedef control super;
      typedef win::window_class<client_control, background> clazz;

      void create (const win::container& parent,
                   const core::rectangle& r = core::rectangle::def);
    };

    // --------------------------------------------------------------------------
    template<os::color B>
    inline void client_control<B>::create (const win::container& parent,
                                           const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

  } // ctrl

} // gui

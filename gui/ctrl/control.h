/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <gui/ctrl/item_state.h>
#include <gui/ctrl/gui++-ctrl-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {
#ifdef GUIPP_WIN
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
      const os::event_id CONTENT_CHANGED_MESSAGE = WM_USER + 11;
#endif //GUIPP_WIN
#ifdef GUIPP_X11
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
#endif // GUIPP_X11
#ifdef GUIPP_QT
      const os::event_id SLIDER_MESSAGE = QEvent::User + 1;
      const os::event_id SELECTION_CHANGE_MESSAGE = QEvent::User + 2;
      const os::event_id SELECTION_COMMIT_MESSAGE = QEvent::User + 3;
      const os::event_id HILITE_CHANGE_MESSAGE = QEvent::User + 4;
      const os::event_id BN_CLICKED_MESSAGE = QEvent::User + 5;
      const os::event_id BN_PUSHED_MESSAGE = QEvent::User + 6;
      const os::event_id BN_UNPUSHED_MESSAGE = QEvent::User + 7;
      const os::event_id BN_STATE_MESSAGE = QEvent::User + 8;
      const os::event_id SCROLLBAR_MESSAGE = QEvent::User + 9;
      const os::event_id SELECTION_CANCEL_MESSAGE = QEvent::User + 10;
      const os::event_id CONTENT_CHANGED_MESSAGE = QEvent::User + 11;
#endif // GUIPP_QT
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
      const_text () = default;

      explicit const_text (const std::string& text)
        : text(text)
      {}

      explicit const_text (std::string&& text)
        : text(std::move(text))
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

#ifdef GUIPP_WIN
  } // namespace ctrl

  namespace win {

    template<>
    GUIPP_CTRL_EXPORT ctrl::event_source get_param<0, ctrl::event_source>(const core::event& e);

  } // namespace win

  namespace ctrl {

    using selection_changed_event = core::event_handler<detail::SELECTION_CHANGE_MESSAGE, 0,
                                                        core::params<event_source>::
                                                        getter<win::get_param<0, event_source> > >;

    using selection_commit_event = core::event_handler<detail::SELECTION_COMMIT_MESSAGE>;

    using selection_cancel_event = core::event_handler<detail::SELECTION_CANCEL_MESSAGE>;

    using hilite_changed_event = core::event_handler<detail::HILITE_CHANGE_MESSAGE, 0,
                                                     core::params<bool>::getter<win::get_param<0, bool> > >;

    using content_changed_event = core::event_handler<detail::CONTENT_CHANGED_MESSAGE>;

#endif //GUIPP_WIN

    // --------------------------------------------------------------------------
    using paint_function = std::function<void(draw::graphics&)>;

#ifdef GUIPP_X11
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

#endif // GUIPP_X11
       // --------------------------------------------------------------------------
#ifdef GUIPP_QT
    GUIPP_CTRL_EXPORT event_source get_event_source (const core::event&);
    GUIPP_CTRL_EXPORT bool get_hilite_changed (const core::event&);

    using selection_commit_event = core::event_handler<detail::SELECTION_COMMIT_MESSAGE>;
    using selection_cancel_event = core::event_handler<detail::SELECTION_CANCEL_MESSAGE>;
    using content_changed_event = core::event_handler<detail::CONTENT_CHANGED_MESSAGE>;

    using selection_changed_event = core::event_handler<detail::SELECTION_CHANGE_MESSAGE, 0,
                                                        core::params<event_source>::
                                                        getter<get_event_source>>;

    using hilite_changed_event = core::event_handler<detail::HILITE_CHANGE_MESSAGE, 0,
                                                     core::params<bool>::
                                                     getter<get_hilite_changed>>;

#endif // GUIPP_QT

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT control : public win::window {
    public:
      typedef win::window super;

      void on_selection_changed (std::function<void(event_source)>&& f);
      void on_selection_commit (std::function<void()>&& f);
      void on_selection_cancel (std::function<void()>&& f);
      void on_hilite_changed (std::function<void(bool)>&& f);
      void on_content_changed (std::function<void()>&& f);

      void notify_selection_changed (event_source);
      void notify_selection_commit ();
      void notify_selection_cancel ();
      void notify_hilite_changed (bool);  // true if hilite is visible, false for no hilite
      void notify_content_changed ();

    };

    // --------------------------------------------------------------------------
    template<os::color B = color::very_light_gray>
    class client_control : public control {
    public:
      typedef control super;
      typedef win::window_class<client_control, B> clazz;

      inline void create (win::container& parent,
                          const core::rectangle& r = core::rectangle::def) {
        super::create(clazz::get(), parent, r);
      }
    };

  } // ctrl

} // gui

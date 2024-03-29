/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     basic control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>
#include <gui/ctrl/tooltip.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {
#ifdef GUIPP_X11
      Atom SCROLLBAR_MESSAGE = 0;
      Atom SELECTION_CHANGE_MESSAGE = 0;
      Atom SELECTION_COMMIT_MESSAGE = 0;
      Atom HILITE_CHANGE_MESSAGE = 0;
      Atom SLIDER_MESSAGE = 0;
      Atom BN_CLICKED_MESSAGE = 0;
      Atom BN_PUSHED_MESSAGE = 0;
      Atom BN_UNPUSHED_MESSAGE = 0;
      Atom BN_STATE_MESSAGE = 0;
      Atom SELECTION_CANCEL_MESSAGE = 0;
      Atom CONTENT_CHANGED_MESSAGE = 0;
#endif // GUIPP_X11

      int init_control_messages () {
#ifdef GUIPP_X11
        core::x11::init_atom(SCROLLBAR_MESSAGE, "SCROLLBAR_MESSAGE");
        core::x11::init_atom(SELECTION_CHANGE_MESSAGE, "SELECTION_CHANGE_MESSAGE");
        core::x11::init_atom(SELECTION_COMMIT_MESSAGE, "SELECTION_COMMIT_MESSAGE");
        core::x11::init_atom(HILITE_CHANGE_MESSAGE, "HILITE_CHANGE_MESSAGE");
        core::x11::init_atom(SLIDER_MESSAGE, "SLIDER_MESSAGE");
        core::x11::init_atom(BN_CLICKED_MESSAGE, "BN_CLICKED_MESSAGE");
        core::x11::init_atom(BN_PUSHED_MESSAGE, "BN_PUSHED_MESSAGE");
        core::x11::init_atom(BN_UNPUSHED_MESSAGE, "BN_UNPUSHED_MESSAGE");
        core::x11::init_atom(BN_STATE_MESSAGE, "BN_STATE_MESSAGE");
        core::x11::init_atom(SELECTION_CANCEL_MESSAGE, "SELECTION_CANCEL_MESSAGE");
        core::x11::init_atom(CONTENT_CHANGED_MESSAGE, "CONTENT_CHANGED_MESSAGE");
#endif // GUIPP_X11
        return 1;
      }

    } // detail

#ifdef GUIPP_WIN
  } // namespace ctrl

  namespace win {

    // --------------------------------------------------------------------------
    template<>
    ctrl::event_source get_param<0, ctrl::event_source> (const core::event& e) {
      return ctrl::event_source(e.wParam);
    }

    template<>
    float get_param<0, float>(const core::event& e) {
      return *(float*)e.wParam;
    }

  } // namespace win

  namespace ctrl {

#endif // GUIPP_WIN

#ifdef GUIPP_QT
    event_source get_event_source (const core::event& e) {
      return static_cast<event_source>(e.cast<win::QClientEvent>().l1());
    }

    bool get_hilite_changed (const core::event& e) {
      return e.cast<win::QClientEvent>().l1() != 0;
    }

#endif // GUIPP_QT

    // --------------------------------------------------------------------------
    void control::on_selection_changed (selection_changed_event::function&& f) {
      on<selection_changed_event>(std::move(f));
    }

    void control::on_selection_commit (selection_commit_event::function&& f) {
      on<selection_commit_event>(std::move(f));
    }

    void control::on_selection_cancel (selection_cancel_event::function&& f) {
      on<selection_cancel_event>(std::move(f));
    }

    void control::on_hilite_changed (hilite_changed_event::function&& f) {
      on<hilite_changed_event>(std::move(f));
    }

    void control::on_content_changed (content_changed_event::function&& f) {
      on<content_changed_event>(std::move(f));
    }

    void control::notify_selection_changed (event_source src) {
      notify_event(detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(src));
    }

    void control::notify_selection_commit () {
      notify_event(detail::SELECTION_COMMIT_MESSAGE);
    }

    void control::notify_selection_cancel () {
      notify_event(detail::SELECTION_CANCEL_MESSAGE);
    }

    void control::notify_hilite_changed (bool b) {
      notify_event(detail::HILITE_CHANGE_MESSAGE, b);
    }

    void control::notify_content_changed () {
      notify_event(detail::CONTENT_CHANGED_MESSAGE);
    }

    void control::add_tooltip (core::text_source t) {
      on_mouse_enter([&, t] {
        tooltip::get().set(t, this, absolute_geometry());
      });
      on_mouse_leave([&] {
        tooltip::get().clear();
      });
    }

    // --------------------------------------------------------------------------
    colored_control::colored_control (os::color fg, os::color bg)
      : foreground(fg)
    {
      set_background(bg);
    }

    void colored_control::set_foreground (os::color c) {
      foreground = c;
    }

    os::color colored_control::get_foreground () const {
      return foreground;
    }

    // --------------------------------------------------------------------------
    client_control::client_control (os::color bg) {
      set_background(bg);
    }

    void client_control::create (win::container& parent,
                                 const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

    // --------------------------------------------------------------------------
  } // ctrl

} // gui

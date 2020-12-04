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
// Library includes
//
#include <gui/draw/bitmap.h>
#include <gui/ctrl/control.h>


namespace gui {

  namespace layout {}

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {
#ifdef X11
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
#endif // X11

      int init_control_messages () {
#ifdef X11
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
#endif // X11
        return 1;
      }

    } // detail

#ifdef WIN32
  } // namespace ctrl

  namespace win {

    // --------------------------------------------------------------------------
    template<>
    draw::graphics get_param<0, draw::graphics> (const core::event& e) {
      return draw::graphics(e.id, (os::graphics)e.wParam);
    }

    // --------------------------------------------------------------------------
    template<>
    ctrl::event_source get_param<0, ctrl::event_source> (const core::event& e) {
      return ctrl::event_source(e.wParam);
    }

  } // namespace win

  namespace ctrl {

    // --------------------------------------------------------------------------
    void paint_caller::operator() (const core::event& e) {
      if (f) {
        PAINTSTRUCT ps;
        os::graphics id = BeginPaint(e.id, &ps);
        f(draw::graphics(e.id, id));
        EndPaint(e.id, &ps);
      }
    }

#endif // WIN32

#ifdef QT_WIDGETS_LIB
    event_source get_event_source (const core::event& e) {
      return static_cast<event_source>(dynamic_cast<const win::QClientEvent&>(e).l1());
    }

    bool get_hilite_changed (const core::event& e) {
      return dynamic_cast<const win::QClientEvent&>(e).l1() != 0;
    }

#endif // QT_WIDGETS_LIB

    namespace paint {

      // --------------------------------------------------------------------------
      void text_item (const draw::graphics& g,
                      const core::rectangle& place,
                      const draw::brush& background,
                      const std::string& text,
                      item_state state,
                      text_origin_t origin) {
        using namespace draw;
        g.fill(rectangle(place), item_state::selected == state ? color::highLightColor() : background);
        os::color col = color::windowTextColor();
        switch (state) {
          case item_state::selected: col = color::highLightTextColor(); break;
          case item_state::disabled: col = color::disabledTextColor(); break;
        }
        g.text(text_box(text, place, origin), font::system(), col);
      }

    } // namespace paint

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

    void control::notify_content_changed () const {
      notify_event(detail::CONTENT_CHANGED_MESSAGE);
    }

  } // ctrl

} // gui

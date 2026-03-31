/**
 * @copyright (c) 2025-2025 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     helper to debug windows messages
 *
 * @license   MIT license. See accompanying file LICENSE.
 */


// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/enable_drag.h"


namespace gui {

  namespace win {

    enable_drag::enable_drag (gui::win::window& w)
      : win(w)
      , drag_win(w)
      , at_drag(false)
    {
      init();
    }

    enable_drag::enable_drag (gui::win::window& w, gui::win::window& drag)
      : win(w)
      , drag_win(drag)
      , at_drag(false)
    {
      init();
    }

    void enable_drag::init () {
      win.on_left_btn_down(util::bind_method(this, &enable_drag::down));
      win.on_left_btn_up(util::bind_method(this, &enable_drag::up));
      win.on_mouse_move(util::bind_method(this, &enable_drag::move));
    }

    void enable_drag::down (gui::os::key_state, const gui::core::native_point& pt) {
      at_drag = true;
      mouse_offset = gui::core::global::scale_from_native(pt);
      win.capture_pointer();
      logging::trace() << "Mouse down at " << pt << " mouse offset: " << mouse_offset;
    }

    void enable_drag::up (gui::os::key_state, const gui::core::native_point& pt) {
      win.uncapture_pointer();
      at_drag = false;
      logging::trace() << "Mouse up at " << pt;
    }

    void enable_drag::move (gui::os::key_state, const gui::core::native_point& pt) {
      if (at_drag) {
        auto current = drag_win.get_overlapped_window().surface_to_screen(pt);
        auto window_pos = current - mouse_offset;
        logging::trace() << "Mouse move to " << pt << " screen pos: " << current << " window: " << window_pos;
        drag_win.position(window_pos);
      }
    }

  } // win

} // gui

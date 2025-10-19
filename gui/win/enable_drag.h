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
 * @brief     helper to draw windows by mouse
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include "gui/win/overlapped_window.h"


namespace gui {

  namespace win {

    struct GUIPP_WIN_EXPORT enable_drag {
      enable_drag (gui::win::window& w);
      enable_drag (gui::win::window& w, gui::win::window& drag);

    private:
      void init ();
      void down (gui::os::key_state, const gui::core::native_point& pt);
      void up (gui::os::key_state, const gui::core::native_point& pt);
      void move (gui::os::key_state, const gui::core::native_point& pt);

    private:
      gui::win::window& win;
      gui::win::window& drag_win;
      gui::core::point mouse_offset;
      bool at_drag;

    };

  } // namespace win

} // namespace gui

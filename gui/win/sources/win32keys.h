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
* @brief     C++ API: win32 key definitions
*
* @file
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
#include "guidefs.h"


namespace gui {

  namespace win {

    namespace state {
      const os::key_state left_button = MK_LBUTTON;
      const os::key_state middle_button = MK_MBUTTON;
      const os::key_state right_button = MK_RBUTTON;

      const os::key_state control = MK_CONTROL;
      const os::key_state shift = MK_SHIFT;
      const os::key_state alt = MK_MENU;
      const os::key_state system = MK_SYTEM;

      const os::key_state num_lock = MK_NUM_LOCK;
      const os::key_state scroll_lock = MK_SCROLL_LOCK;
      const os::key_state caps_lock = MK_CAPS_LOCK;
    }

    namespace keys {
      const os::key_symbol left = VK_LEFT;
      const os::key_symbol right = VK_RIGHT;
      const os::key_symbol up = VK_UP;
      const os::key_symbol down = VK_DOWN;

      const os::key_symbol page_up = VK_PRIOR;
      const os::key_symbol page_down = VK_NEXT;
      const os::key_symbol home = VK_HOME;
      const os::key_symbol end = VK_END;

      namespace numpad {
        const os::key_symbol left = VK_LEFT | 0xFF00;
        const os::key_symbol right = VK_RIGHT | 0xFF00;
        const os::key_symbol up = VK_UP | 0xFF00;
        const os::key_symbol down = VK_DOWN | 0xFF00;

        const os::key_symbol page_up = VK_PRIOR | 0xFF00;
        const os::key_symbol page_down = VK_NEXT | 0xFF00;
        const os::key_symbol home = VK_HOME | 0xFF00;
        const os::key_symbol end = VK_END | 0xFF00;

        const os::key_symbol del = VK_DELETE | 0xFF00;
      }

      const os::key_symbol del = VK_DELETE;
      const os::key_symbol insert = VK_INSERT;
      const os::key_symbol escape = VK_ESCAPE;
      const os::key_symbol enter = VK_RETURN;
      const os::key_symbol space = VK_SPACE;
      const os::key_symbol clear = 0xFFFF;

      const os::key_symbol back_space = VK_BACK;
      const os::key_symbol tab = VK_TAB;
      const os::key_symbol print = VK_PRINT;

      const os::key_symbol f1 = VK_F1;
      const os::key_symbol f2 = VK_F2;
      const os::key_symbol f3 = VK_F3;
      const os::key_symbol f4 = VK_F4;
      const os::key_symbol f5 = VK_F5;
      const os::key_symbol f6 = VK_F6;
      const os::key_symbol f7 = VK_F7;
      const os::key_symbol f8 = VK_F8;
      const os::key_symbol f9 = VK_F9;
      const os::key_symbol f10 = VK_F10;
      const os::key_symbol f11 = VK_F11;
      const os::key_symbol f12 = VK_F12;
      const os::key_symbol f13 = VK_F13;
      const os::key_symbol f14 = VK_F14;
      const os::key_symbol f15 = VK_F15;
      const os::key_symbol f16 = VK_F16;
      const os::key_symbol f17 = VK_F17;
      const os::key_symbol f18 = VK_F18;
      const os::key_symbol f19 = VK_F19;
      const os::key_symbol f20 = VK_F20;
      const os::key_symbol f21 = VK_F21;
      const os::key_symbol f22 = VK_F22;
      const os::key_symbol f23 = VK_F23;
      const os::key_symbol f24 = VK_F24;

      const os::key_symbol control = VK_CONTROL;
      const os::key_symbol alt = VK_MENU;
      const os::key_symbol shift = VK_SHIFT;
      const os::key_symbol system = VK_LWIN;

      const os::key_symbol num_lock = VK_NUMLOCK;
      const os::key_symbol scroll_lock = VK_SCROLL;
      const os::key_symbol caps_lock = VK_CAPITAL;

    }

  } // win

} // gui

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
* @brief     C++ API: x11 key definitions
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
      const os::key_state left_button = Button1Mask;
      const os::key_state middle_button = Button2Mask;
      const os::key_state right_button = Button3Mask;

      const os::key_state control = ControlMask;
      const os::key_state shift = ShiftMask;
      const os::key_state alt = Mod1Mask;
      const os::key_state system = Mod4Mask;

      const os::key_state num_lock = Mod2Mask;
      const os::key_state scroll_lock = Mod3Mask;
      const os::key_state caps_lock = LockMask;
    }

    namespace keys {
      const os::key_symbol left = XK_Left;
      const os::key_symbol right = XK_Right;
      const os::key_symbol up = XK_Up;
      const os::key_symbol down = XK_Down;

      const os::key_symbol page_up = XK_Page_Up;
      const os::key_symbol page_down = XK_Page_Down;
      const os::key_symbol home = XK_Home;
      const os::key_symbol end = XK_End;

      namespace numpad {
        const os::key_symbol left = XK_KP_Left;
        const os::key_symbol right = XK_KP_Right;
        const os::key_symbol up = XK_KP_Up;
        const os::key_symbol down = XK_KP_Down;

        const os::key_symbol page_up = XK_KP_Page_Up;
        const os::key_symbol page_down = XK_KP_Page_Down;
        const os::key_symbol home = XK_KP_Home;
        const os::key_symbol end = XK_KP_End;

        const os::key_symbol del = XK_KP_Delete;
      }

      const os::key_symbol del = XK_Delete;
      const os::key_symbol insert = XK_Insert;
      const os::key_symbol escape = XK_Escape;
      const os::key_symbol enter = XK_Return;
      const os::key_symbol space = XK_space;
      const os::key_symbol clear = XK_Clear;

      const os::key_symbol back_space = XK_BackSpace;
      const os::key_symbol tab = XK_Tab;
      const os::key_symbol print = XK_Print;

      const os::key_symbol f1 = XK_F1;
      const os::key_symbol f2 = XK_F2;
      const os::key_symbol f3 = XK_F3;
      const os::key_symbol f4 = XK_F4;
      const os::key_symbol f5 = XK_F5;
      const os::key_symbol f6 = XK_F6;
      const os::key_symbol f7 = XK_F7;
      const os::key_symbol f8 = XK_F8;
      const os::key_symbol f9 = XK_F9;
      const os::key_symbol f10 = XK_F10;
      const os::key_symbol f11 = XK_F11;
      const os::key_symbol f12 = XK_F12;
      const os::key_symbol f13 = XK_F13;
      const os::key_symbol f14 = XK_F14;
      const os::key_symbol f15 = XK_F15;
      const os::key_symbol f16 = XK_F16;
      const os::key_symbol f17 = XK_F17;
      const os::key_symbol f18 = XK_F18;
      const os::key_symbol f19 = XK_F19;
      const os::key_symbol f20 = XK_F20;
      const os::key_symbol f21 = XK_F21;
      const os::key_symbol f22 = XK_F22;
      const os::key_symbol f23 = XK_F23;
      const os::key_symbol f24 = XK_F24;

      const os::key_symbol control = XK_Control_L;
      const os::key_symbol alt = XK_Alt_L;
      const os::key_symbol shift = XK_Shift_L;
      const os::key_symbol system = XK_Super_L;

      const os::key_symbol num_lock = XK_Num_Lock;
      const os::key_symbol scroll_lock = XK_Scroll_Lock;
      const os::key_symbol caps_lock = XK_Caps_Lock;

    }

  } // win

} // gui

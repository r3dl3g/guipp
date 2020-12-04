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
// Library includes
//
#include <gui/core/guidefs.h>


namespace gui {

  namespace win {

    namespace state {
      const os::key_state none = Qt::Key_unknown;

      const os::key_state left_button = Qt::LeftButton;
      const os::key_state middle_button = Qt::MiddleButton;
      const os::key_state right_button = Qt::RightButton;

      const os::key_state control = Qt::CTRL;
      const os::key_state shift = Qt::SHIFT;
      const os::key_state alt = Qt::ALT;
      const os::key_state system = Qt::META;

      const os::key_state num_lock = Qt::KeypadModifier;
      const os::key_state scroll_lock = Qt::Key_unknown;
      const os::key_state caps_lock = Qt::Key_unknown;
    }

    namespace keys {
      const os::key_symbol left = Qt::Key_Left;
      const os::key_symbol right = Qt::Key_Right;
      const os::key_symbol up = Qt::Key_Up;
      const os::key_symbol down = Qt::Key_Down;

      const os::key_symbol page_up = Qt::Key_PageUp;
      const os::key_symbol page_down = Qt::Key_PageDown;
      const os::key_symbol home = Qt::Key_Home;
      const os::key_symbol end = Qt::Key_End;

      namespace numpad {
        const os::key_symbol left = Qt::Key_Left | Qt::KeypadModifier;
        const os::key_symbol right = Qt::Key_Right | Qt::KeypadModifier;
        const os::key_symbol up = Qt::Key_Up | Qt::KeypadModifier;
        const os::key_symbol down = Qt::Key_Down | Qt::KeypadModifier;

        const os::key_symbol page_up = Qt::Key_PageUp | Qt::KeypadModifier;
        const os::key_symbol page_down = Qt::Key_PageDown | Qt::KeypadModifier;
        const os::key_symbol home = Qt::Key_Home | Qt::KeypadModifier;
        const os::key_symbol end = Qt::Key_End | Qt::KeypadModifier;

        const os::key_symbol del = Qt::Key_Delete | Qt::KeypadModifier;
      }

      const os::key_symbol del = Qt::Key_Delete;
      const os::key_symbol insert = Qt::Key_Insert;
      const os::key_symbol escape = Qt::Key_Escape;
      const os::key_symbol enter = Qt::Key_Return;
      const os::key_symbol space = Qt::Key_Space;
      const os::key_symbol clear = Qt::Key_Clear;

      const os::key_symbol back_space = Qt::Key_Backspace;
      const os::key_symbol tab = Qt::Key_Tab;
      const os::key_symbol print = Qt::Key_Print;

      const os::key_symbol f1 = Qt::Key_F1;
      const os::key_symbol f2 = Qt::Key_F2;
      const os::key_symbol f3 = Qt::Key_F3;
      const os::key_symbol f4 = Qt::Key_F4;
      const os::key_symbol f5 = Qt::Key_F5;
      const os::key_symbol f6 = Qt::Key_F6;
      const os::key_symbol f7 = Qt::Key_F7;
      const os::key_symbol f8 = Qt::Key_F8;
      const os::key_symbol f9 = Qt::Key_F9;
      const os::key_symbol f10 = Qt::Key_F10;
      const os::key_symbol f11 = Qt::Key_F11;
      const os::key_symbol f12 = Qt::Key_F12;
      const os::key_symbol f13 = Qt::Key_F13;
      const os::key_symbol f14 = Qt::Key_F14;
      const os::key_symbol f15 = Qt::Key_F15;
      const os::key_symbol f16 = Qt::Key_F16;
      const os::key_symbol f17 = Qt::Key_F17;
      const os::key_symbol f18 = Qt::Key_F18;
      const os::key_symbol f19 = Qt::Key_F19;
      const os::key_symbol f20 = Qt::Key_F20;
      const os::key_symbol f21 = Qt::Key_F21;
      const os::key_symbol f22 = Qt::Key_F22;
      const os::key_symbol f23 = Qt::Key_F23;
      const os::key_symbol f24 = Qt::Key_F24;

      const os::key_symbol control = Qt::Key_Control;
      const os::key_symbol alt = Qt::Key_Alt;
      const os::key_symbol shift = Qt::Key_Shift;
      const os::key_symbol system = Qt::Key_Super_L;

      const os::key_symbol num_lock = Qt::Key_NumLock;
      const os::key_symbol scroll_lock = Qt::Key_ScrollLock;
      const os::key_symbol caps_lock = Qt::Key_CapsLock;

      const os::key_symbol a = Qt::Key_A;
      const os::key_symbol b = Qt::Key_B;
      const os::key_symbol c = Qt::Key_C;
      const os::key_symbol d = Qt::Key_D;
      const os::key_symbol e = Qt::Key_E;
      const os::key_symbol f = Qt::Key_F;
      const os::key_symbol g = Qt::Key_G;
      const os::key_symbol h = Qt::Key_H;
      const os::key_symbol i = Qt::Key_I;
      const os::key_symbol j = Qt::Key_J;
      const os::key_symbol k = Qt::Key_K;
      const os::key_symbol l = Qt::Key_L;
      const os::key_symbol m = Qt::Key_M;
      const os::key_symbol n = Qt::Key_N;
      const os::key_symbol o = Qt::Key_O;
      const os::key_symbol p = Qt::Key_P;
      const os::key_symbol q = Qt::Key_Q;
      const os::key_symbol r = Qt::Key_R;
      const os::key_symbol s = Qt::Key_S;
      const os::key_symbol t = Qt::Key_T;
      const os::key_symbol u = Qt::Key_U;
      const os::key_symbol v = Qt::Key_V;
      const os::key_symbol w = Qt::Key_W;
      const os::key_symbol x = Qt::Key_X;
      const os::key_symbol y = Qt::Key_Y;
      const os::key_symbol z = Qt::Key_Z;
    }

  } // namespace win

} // namespace gui

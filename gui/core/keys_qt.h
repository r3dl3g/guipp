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
 * @brief     Qt specific key definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

#ifndef GUIPP_QT
# error Include keys.h instead keys_qt.h!
#endif // GUIPP_QT


namespace gui {

  namespace core {

    namespace state {
      const gui::os::key_state none = Qt::Key_unknown;

      const gui::os::key_state left_button = Qt::LeftButton;
      const gui::os::key_state middle_button = Qt::MiddleButton;
      const gui::os::key_state right_button = Qt::RightButton;

      const gui::os::key_state control = Qt::CTRL;
      const gui::os::key_state shift = Qt::SHIFT;
      const gui::os::key_state alt = Qt::ALT;
      const gui::os::key_state system = Qt::META;

      const gui::os::key_state num_lock = Qt::KeypadModifier;
      const gui::os::key_state scroll_lock = Qt::Key_unknown;
      const gui::os::key_state caps_lock = Qt::Key_unknown;
    }

    namespace keys {
      const gui::os::key_symbol left = Qt::Key_Left;
      const gui::os::key_symbol right = Qt::Key_Right;
      const gui::os::key_symbol up = Qt::Key_Up;
      const gui::os::key_symbol down = Qt::Key_Down;

      const gui::os::key_symbol page_up = Qt::Key_PageUp;
      const gui::os::key_symbol page_down = Qt::Key_PageDown;
      const gui::os::key_symbol home = Qt::Key_Home;
      const gui::os::key_symbol end = Qt::Key_End;

      namespace numpad {
        const gui::os::key_symbol left = Qt::Key_Left | Qt::KeypadModifier;
        const gui::os::key_symbol right = Qt::Key_Right | Qt::KeypadModifier;
        const gui::os::key_symbol up = Qt::Key_Up | Qt::KeypadModifier;
        const gui::os::key_symbol down = Qt::Key_Down | Qt::KeypadModifier;

        const gui::os::key_symbol page_up = Qt::Key_PageUp | Qt::KeypadModifier;
        const gui::os::key_symbol page_down = Qt::Key_PageDown | Qt::KeypadModifier;
        const gui::os::key_symbol home = Qt::Key_Home | Qt::KeypadModifier;
        const gui::os::key_symbol end = Qt::Key_End | Qt::KeypadModifier;

        const gui::os::key_symbol del = Qt::Key_Delete | Qt::KeypadModifier;
      }

      const gui::os::key_symbol del = Qt::Key_Delete;
      const gui::os::key_symbol insert = Qt::Key_Insert;
      const gui::os::key_symbol escape = Qt::Key_Escape;
      const gui::os::key_symbol enter = Qt::Key_Return;
      const gui::os::key_symbol space = Qt::Key_Space;
      const gui::os::key_symbol clear = Qt::Key_Clear;

      const gui::os::key_symbol back_space = Qt::Key_Backspace;
      const gui::os::key_symbol tab = Qt::Key_Tab;
      const gui::os::key_symbol print = Qt::Key_Print;

      const gui::os::key_symbol f1 = Qt::Key_F1;
      const gui::os::key_symbol f2 = Qt::Key_F2;
      const gui::os::key_symbol f3 = Qt::Key_F3;
      const gui::os::key_symbol f4 = Qt::Key_F4;
      const gui::os::key_symbol f5 = Qt::Key_F5;
      const gui::os::key_symbol f6 = Qt::Key_F6;
      const gui::os::key_symbol f7 = Qt::Key_F7;
      const gui::os::key_symbol f8 = Qt::Key_F8;
      const gui::os::key_symbol f9 = Qt::Key_F9;
      const gui::os::key_symbol f10 = Qt::Key_F10;
      const gui::os::key_symbol f11 = Qt::Key_F11;
      const gui::os::key_symbol f12 = Qt::Key_F12;
      const gui::os::key_symbol f13 = Qt::Key_F13;
      const gui::os::key_symbol f14 = Qt::Key_F14;
      const gui::os::key_symbol f15 = Qt::Key_F15;
      const gui::os::key_symbol f16 = Qt::Key_F16;
      const gui::os::key_symbol f17 = Qt::Key_F17;
      const gui::os::key_symbol f18 = Qt::Key_F18;
      const gui::os::key_symbol f19 = Qt::Key_F19;
      const gui::os::key_symbol f20 = Qt::Key_F20;
      const gui::os::key_symbol f21 = Qt::Key_F21;
      const gui::os::key_symbol f22 = Qt::Key_F22;
      const gui::os::key_symbol f23 = Qt::Key_F23;
      const gui::os::key_symbol f24 = Qt::Key_F24;

      const gui::os::key_symbol control = Qt::Key_Control;
      const gui::os::key_symbol alt = Qt::Key_Alt;
      const gui::os::key_symbol shift = Qt::Key_Shift;
      const gui::os::key_symbol system = Qt::Key_Super_L;

      const gui::os::key_symbol num_lock = Qt::Key_NumLock;
      const gui::os::key_symbol scroll_lock = Qt::Key_ScrollLock;
      const gui::os::key_symbol caps_lock = Qt::Key_CapsLock;

      const gui::os::key_symbol a = Qt::Key_A;
      const gui::os::key_symbol b = Qt::Key_B;
      const gui::os::key_symbol c = Qt::Key_C;
      const gui::os::key_symbol d = Qt::Key_D;
      const gui::os::key_symbol e = Qt::Key_E;
      const gui::os::key_symbol f = Qt::Key_F;
      const gui::os::key_symbol g = Qt::Key_G;
      const gui::os::key_symbol h = Qt::Key_H;
      const gui::os::key_symbol i = Qt::Key_I;
      const gui::os::key_symbol j = Qt::Key_J;
      const gui::os::key_symbol k = Qt::Key_K;
      const gui::os::key_symbol l = Qt::Key_L;
      const gui::os::key_symbol m = Qt::Key_M;
      const gui::os::key_symbol n = Qt::Key_N;
      const gui::os::key_symbol o = Qt::Key_O;
      const gui::os::key_symbol p = Qt::Key_P;
      const gui::os::key_symbol q = Qt::Key_Q;
      const gui::os::key_symbol r = Qt::Key_R;
      const gui::os::key_symbol s = Qt::Key_S;
      const gui::os::key_symbol t = Qt::Key_T;
      const gui::os::key_symbol u = Qt::Key_U;
      const gui::os::key_symbol v = Qt::Key_V;
      const gui::os::key_symbol w = Qt::Key_W;
      const gui::os::key_symbol x = Qt::Key_X;
      const gui::os::key_symbol y = Qt::Key_Y;
      const gui::os::key_symbol z = Qt::Key_Z;
    }

  } // namespace core

} // namespace gui

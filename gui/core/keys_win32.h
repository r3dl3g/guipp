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
 * @brief     win32 specific key definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

#ifndef GUIPP_WIN
# error Include keys.h instead keys_win32.h!
#endif //GUIPP_WIN


namespace gui {

  namespace core {

    namespace state {
      const gui::os::key_state none = 0;

      const gui::os::key_state left_button = MK_LBUTTON;
      const gui::os::key_state middle_button = MK_MBUTTON;
      const gui::os::key_state right_button = MK_RBUTTON;

      const gui::os::key_state control = MK_CONTROL;
      const gui::os::key_state shift = MK_SHIFT;
      const gui::os::key_state alt = MK_MENU;
      const gui::os::key_state system = MK_SYTEM;

      const gui::os::key_state num_lock = MK_NUM_LOCK;
      const gui::os::key_state scroll_lock = MK_SCROLL_LOCK;
      const gui::os::key_state caps_lock = MK_CAPS_LOCK;
    }

    namespace keys {
      const gui::os::key_symbol left = VK_LEFT;
      const gui::os::key_symbol right = VK_RIGHT;
      const gui::os::key_symbol up = VK_UP;
      const gui::os::key_symbol down = VK_DOWN;

      const gui::os::key_symbol page_up = VK_PRIOR;
      const gui::os::key_symbol page_down = VK_NEXT;
      const gui::os::key_symbol home = VK_HOME;
      const gui::os::key_symbol end = VK_END;

      namespace numpad {
        const gui::os::key_symbol left = VK_LEFT | 0xFF00;
        const gui::os::key_symbol right = VK_RIGHT | 0xFF00;
        const gui::os::key_symbol up = VK_UP | 0xFF00;
        const gui::os::key_symbol down = VK_DOWN | 0xFF00;

        const gui::os::key_symbol page_up = VK_PRIOR | 0xFF00;
        const gui::os::key_symbol page_down = VK_NEXT | 0xFF00;
        const gui::os::key_symbol home = VK_HOME | 0xFF00;
        const gui::os::key_symbol end = VK_END | 0xFF00;

        const gui::os::key_symbol del = VK_DELETE | 0xFF00;
      }

      const gui::os::key_symbol del = VK_DELETE;
      const gui::os::key_symbol insert = VK_INSERT;
      const gui::os::key_symbol escape = VK_ESCAPE;
      const gui::os::key_symbol enter = VK_RETURN;
      const gui::os::key_symbol space = VK_SPACE;
      const gui::os::key_symbol clear = 0xFFFF;

      const gui::os::key_symbol back_space = VK_BACK;
      const gui::os::key_symbol tab = VK_TAB;
      const gui::os::key_symbol print = VK_PRINT;

      const gui::os::key_symbol f1 = VK_F1;
      const gui::os::key_symbol f2 = VK_F2;
      const gui::os::key_symbol f3 = VK_F3;
      const gui::os::key_symbol f4 = VK_F4;
      const gui::os::key_symbol f5 = VK_F5;
      const gui::os::key_symbol f6 = VK_F6;
      const gui::os::key_symbol f7 = VK_F7;
      const gui::os::key_symbol f8 = VK_F8;
      const gui::os::key_symbol f9 = VK_F9;
      const gui::os::key_symbol f10 = VK_F10;
      const gui::os::key_symbol f11 = VK_F11;
      const gui::os::key_symbol f12 = VK_F12;
      const gui::os::key_symbol f13 = VK_F13;
      const gui::os::key_symbol f14 = VK_F14;
      const gui::os::key_symbol f15 = VK_F15;
      const gui::os::key_symbol f16 = VK_F16;
      const gui::os::key_symbol f17 = VK_F17;
      const gui::os::key_symbol f18 = VK_F18;
      const gui::os::key_symbol f19 = VK_F19;
      const gui::os::key_symbol f20 = VK_F20;
      const gui::os::key_symbol f21 = VK_F21;
      const gui::os::key_symbol f22 = VK_F22;
      const gui::os::key_symbol f23 = VK_F23;
      const gui::os::key_symbol f24 = VK_F24;

      const gui::os::key_symbol control = VK_CONTROL;
      const gui::os::key_symbol alt = VK_MENU;
      const gui::os::key_symbol shift = VK_SHIFT;
      const gui::os::key_symbol system = VK_LWIN;

      const gui::os::key_symbol num_lock = VK_NUMLOCK;
      const gui::os::key_symbol scroll_lock = VK_SCROLL;
      const gui::os::key_symbol caps_lock = VK_CAPITAL;

      const gui::os::key_symbol a = 'A';
      const gui::os::key_symbol b = 'B';
      const gui::os::key_symbol c = 'C';
      const gui::os::key_symbol d = 'D';
      const gui::os::key_symbol e = 'E';
      const gui::os::key_symbol f = 'F';
      const gui::os::key_symbol g = 'G';
      const gui::os::key_symbol h = 'H';
      const gui::os::key_symbol i = 'I';
      const gui::os::key_symbol j = 'J';
      const gui::os::key_symbol k = 'K';
      const gui::os::key_symbol l = 'L';
      const gui::os::key_symbol m = 'M';
      const gui::os::key_symbol n = 'N';
      const gui::os::key_symbol o = 'O';
      const gui::os::key_symbol p = 'P';
      const gui::os::key_symbol q = 'Q';
      const gui::os::key_symbol r = 'R';
      const gui::os::key_symbol s = 'S';
      const gui::os::key_symbol t = 'T';
      const gui::os::key_symbol u = 'U';
      const gui::os::key_symbol v = 'V';
      const gui::os::key_symbol w = 'W';
      const gui::os::key_symbol x = 'X';
      const gui::os::key_symbol y = 'Y';
      const gui::os::key_symbol z = 'Z';

    }

  } // namespace core

} // namespace gui

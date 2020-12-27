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

#ifndef GUIPP_X11
# error Include keys.h instead keys_x11.h!
#endif // GUIPP_X11


namespace gui {

  namespace core {

    namespace state {
      const gui::os::key_state none = 0;

      const gui::os::key_state left_button = Button1Mask;
      const gui::os::key_state middle_button = Button2Mask;
      const gui::os::key_state right_button = Button3Mask;

      const gui::os::key_state control = ControlMask;
      const gui::os::key_state shift = ShiftMask;
      const gui::os::key_state alt = Mod1Mask;
      const gui::os::key_state system = Mod4Mask;

      const gui::os::key_state num_lock = Mod2Mask;
      const gui::os::key_state scroll_lock = Mod3Mask;
      const gui::os::key_state caps_lock = LockMask;
    }

    namespace keys {
      const gui::os::key_symbol left = XK_Left;
      const gui::os::key_symbol right = XK_Right;
      const gui::os::key_symbol up = XK_Up;
      const gui::os::key_symbol down = XK_Down;

      const gui::os::key_symbol page_up = XK_Page_Up;
      const gui::os::key_symbol page_down = XK_Page_Down;
      const gui::os::key_symbol home = XK_Home;
      const gui::os::key_symbol end = XK_End;

      namespace numpad {
        const gui::os::key_symbol left = XK_KP_Left;
        const gui::os::key_symbol right = XK_KP_Right;
        const gui::os::key_symbol up = XK_KP_Up;
        const gui::os::key_symbol down = XK_KP_Down;

        const gui::os::key_symbol page_up = XK_KP_Page_Up;
        const gui::os::key_symbol page_down = XK_KP_Page_Down;
        const gui::os::key_symbol home = XK_KP_Home;
        const gui::os::key_symbol end = XK_KP_End;

        const gui::os::key_symbol del = XK_KP_Delete;
      }

      const gui::os::key_symbol del = XK_Delete;
      const gui::os::key_symbol insert = XK_Insert;
      const gui::os::key_symbol escape = XK_Escape;
      const gui::os::key_symbol enter = XK_Return;
      const gui::os::key_symbol space = XK_space;
      const gui::os::key_symbol clear = XK_Clear;

      const gui::os::key_symbol back_space = XK_BackSpace;
      const gui::os::key_symbol tab = XK_Tab;
      const gui::os::key_symbol print = XK_Print;

      const gui::os::key_symbol f1 = XK_F1;
      const gui::os::key_symbol f2 = XK_F2;
      const gui::os::key_symbol f3 = XK_F3;
      const gui::os::key_symbol f4 = XK_F4;
      const gui::os::key_symbol f5 = XK_F5;
      const gui::os::key_symbol f6 = XK_F6;
      const gui::os::key_symbol f7 = XK_F7;
      const gui::os::key_symbol f8 = XK_F8;
      const gui::os::key_symbol f9 = XK_F9;
      const gui::os::key_symbol f10 = XK_F10;
      const gui::os::key_symbol f11 = XK_F11;
      const gui::os::key_symbol f12 = XK_F12;
      const gui::os::key_symbol f13 = XK_F13;
      const gui::os::key_symbol f14 = XK_F14;
      const gui::os::key_symbol f15 = XK_F15;
      const gui::os::key_symbol f16 = XK_F16;
      const gui::os::key_symbol f17 = XK_F17;
      const gui::os::key_symbol f18 = XK_F18;
      const gui::os::key_symbol f19 = XK_F19;
      const gui::os::key_symbol f20 = XK_F20;
      const gui::os::key_symbol f21 = XK_F21;
      const gui::os::key_symbol f22 = XK_F22;
      const gui::os::key_symbol f23 = XK_F23;
      const gui::os::key_symbol f24 = XK_F24;

      const gui::os::key_symbol control = XK_Control_L;
      const gui::os::key_symbol alt = XK_Alt_L;
      const gui::os::key_symbol shift = XK_Shift_L;
      const gui::os::key_symbol system = XK_Super_L;

      const gui::os::key_symbol num_lock = XK_Num_Lock;
      const gui::os::key_symbol scroll_lock = XK_Scroll_Lock;
      const gui::os::key_symbol caps_lock = XK_Caps_Lock;

      const gui::os::key_symbol a = XK_a;
      const gui::os::key_symbol b = XK_b;
      const gui::os::key_symbol c = XK_c;
      const gui::os::key_symbol d = XK_d;
      const gui::os::key_symbol e = XK_e;
      const gui::os::key_symbol f = XK_f;
      const gui::os::key_symbol g = XK_g;
      const gui::os::key_symbol h = XK_h;
      const gui::os::key_symbol i = XK_i;
      const gui::os::key_symbol j = XK_j;
      const gui::os::key_symbol k = XK_k;
      const gui::os::key_symbol l = XK_l;
      const gui::os::key_symbol m = XK_m;
      const gui::os::key_symbol n = XK_n;
      const gui::os::key_symbol o = XK_o;
      const gui::os::key_symbol p = XK_p;
      const gui::os::key_symbol q = XK_q;
      const gui::os::key_symbol r = XK_r;
      const gui::os::key_symbol s = XK_s;
      const gui::os::key_symbol t = XK_t;
      const gui::os::key_symbol u = XK_u;
      const gui::os::key_symbol v = XK_v;
      const gui::os::key_symbol w = XK_w;
      const gui::os::key_symbol x = XK_x;
      const gui::os::key_symbol y = XK_y;
      const gui::os::key_symbol z = XK_z;
    }

  } // namespace core

} // namespace gui

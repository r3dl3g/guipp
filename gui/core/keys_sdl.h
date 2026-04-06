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
 * @brief     X11 specific key definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

#ifndef GUIPP_SDL
# error Include keys.h instead keys_sdl.h!
#endif // GUIPP_X11


namespace gui {

  namespace core {

    namespace state {
      const gui::os::key_state none = KMOD_NONE;

      const gui::os::key_state left_button = static_cast<gui::os::key_state>(1);
      const gui::os::key_state middle_button = static_cast<gui::os::key_state>(2);
      const gui::os::key_state right_button = static_cast<gui::os::key_state>(3);

      const gui::os::key_state control = KMOD_CTRL;
      const gui::os::key_state shift = KMOD_SHIFT;
      const gui::os::key_state alt = KMOD_ALT;
      const gui::os::key_state system = KMOD_GUI;

      const gui::os::key_state num_lock = KMOD_NUM;
      const gui::os::key_state scroll_lock = KMOD_SCROLL;
      const gui::os::key_state caps_lock = KMOD_CAPS;
    }

    namespace keys {
      const gui::os::key_symbol left = SDLK_LEFT;
      const gui::os::key_symbol right = SDLK_RIGHT;
      const gui::os::key_symbol up = SDLK_UP;
      const gui::os::key_symbol down = SDLK_DOWN;

      const gui::os::key_symbol page_up = SDLK_PAGEUP;
      const gui::os::key_symbol page_down = SDLK_PAGEDOWN;
      const gui::os::key_symbol home = SDLK_HOME;
      const gui::os::key_symbol end = SDLK_END;

      namespace numpad {
        const gui::os::key_symbol left = SDLK_KP_4;
        const gui::os::key_symbol right = SDLK_KP_6;
        const gui::os::key_symbol up = SDLK_KP_8;
        const gui::os::key_symbol down = SDLK_KP_2;

        const gui::os::key_symbol page_up = SDLK_KP_9;
        const gui::os::key_symbol page_down = SDLK_KP_3;
        const gui::os::key_symbol home = SDLK_KP_7;
        const gui::os::key_symbol end = SDLK_KP_1;

        const gui::os::key_symbol del = SDLK_KP_BACKSPACE;
      }

      const gui::os::key_symbol del = SDLK_DELETE;
      const gui::os::key_symbol insert = SDLK_INSERT;
      const gui::os::key_symbol escape = SDLK_ESCAPE;
      const gui::os::key_symbol enter = SDLK_RETURN;
      const gui::os::key_symbol space = SDLK_SPACE;
      const gui::os::key_symbol clear = SDLK_CLEAR;

      const gui::os::key_symbol back_space = SDLK_BACKSPACE;
      const gui::os::key_symbol tab = SDLK_TAB;
      const gui::os::key_symbol print = SDLK_PRINTSCREEN;

      const gui::os::key_symbol f1 =  SDLK_F1;
      const gui::os::key_symbol f2 =  SDLK_F2;
      const gui::os::key_symbol f3 =  SDLK_F3;
      const gui::os::key_symbol f4 =  SDLK_F4;
      const gui::os::key_symbol f5 =  SDLK_F5;
      const gui::os::key_symbol f6 =  SDLK_F6;
      const gui::os::key_symbol f7 =  SDLK_F7;
      const gui::os::key_symbol f8 =  SDLK_F8;
      const gui::os::key_symbol f9 =  SDLK_F9;
      const gui::os::key_symbol f10 = SDLK_F10;
      const gui::os::key_symbol f11 = SDLK_F11;
      const gui::os::key_symbol f12 = SDLK_F12;
      const gui::os::key_symbol f13 = SDLK_F13;
      const gui::os::key_symbol f14 = SDLK_F14;
      const gui::os::key_symbol f15 = SDLK_F15;
      const gui::os::key_symbol f16 = SDLK_F16;
      const gui::os::key_symbol f17 = SDLK_F17;
      const gui::os::key_symbol f18 = SDLK_F18;
      const gui::os::key_symbol f19 = SDLK_F19;
      const gui::os::key_symbol f20 = SDLK_F20;
      const gui::os::key_symbol f21 = SDLK_F21;
      const gui::os::key_symbol f22 = SDLK_F22;
      const gui::os::key_symbol f23 = SDLK_F23;
      const gui::os::key_symbol f24 = SDLK_F24;

      const gui::os::key_symbol control = SDLK_LCTRL;
      const gui::os::key_symbol alt = SDLK_LALT;
      const gui::os::key_symbol shift = SDLK_LSHIFT;
      const gui::os::key_symbol system = SDLK_LGUI;

      const gui::os::key_symbol num_lock = SDLK_NUMLOCKCLEAR;
      const gui::os::key_symbol scroll_lock = SDLK_SCROLLLOCK;
      const gui::os::key_symbol caps_lock = SDLK_CAPSLOCK;

      const gui::os::key_symbol a = SDLK_a;
      const gui::os::key_symbol b = SDLK_b;
      const gui::os::key_symbol c = SDLK_c;
      const gui::os::key_symbol d = SDLK_d;
      const gui::os::key_symbol e = SDLK_e;
      const gui::os::key_symbol f = SDLK_f;
      const gui::os::key_symbol g = SDLK_g;
      const gui::os::key_symbol h = SDLK_h;
      const gui::os::key_symbol i = SDLK_i;
      const gui::os::key_symbol j = SDLK_j;
      const gui::os::key_symbol k = SDLK_k;
      const gui::os::key_symbol l = SDLK_l;
      const gui::os::key_symbol m = SDLK_m;
      const gui::os::key_symbol n = SDLK_n;
      const gui::os::key_symbol o = SDLK_o;
      const gui::os::key_symbol p = SDLK_p;
      const gui::os::key_symbol q = SDLK_q;
      const gui::os::key_symbol r = SDLK_r;
      const gui::os::key_symbol s = SDLK_s;
      const gui::os::key_symbol t = SDLK_t;
      const gui::os::key_symbol u = SDLK_u;
      const gui::os::key_symbol v = SDLK_v;
      const gui::os::key_symbol w = SDLK_w;
      const gui::os::key_symbol x = SDLK_x;
      const gui::os::key_symbol y = SDLK_y;
      const gui::os::key_symbol z = SDLK_z;

      const gui::os::key_symbol plus =      SDLK_PLUS;
      const gui::os::key_symbol minus =     SDLK_MINUS;
      const gui::os::key_symbol asterisk =  SDLK_ASTERISK;
      const gui::os::key_symbol comma =     SDLK_COMMA;
      const gui::os::key_symbol period =    SDLK_PERIOD;
      const gui::os::key_symbol slash =     SDLK_SLASH;
      const gui::os::key_symbol colon =     SDLK_COLON;
      const gui::os::key_symbol semicolon = SDLK_SEMICOLON;
      const gui::os::key_symbol less =      SDLK_LESS;
      const gui::os::key_symbol equal =     SDLK_EQUALS;
      const gui::os::key_symbol greater =   SDLK_GREATER;
      const gui::os::key_symbol question =  SDLK_QUESTION;
      const gui::os::key_symbol at =        SDLK_AT;

    }

  } // namespace core

} // namespace gui

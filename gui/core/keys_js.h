/**
 * @copyright (c) 2026-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     browser specific key definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

#ifndef GUIPP_JS
# error Include keys.h instead keys_js.h!
#endif //GUIPP_WIN


namespace gui {

  namespace core {

    namespace state {
      const gui::os::key_state none = 0;

      const gui::os::key_state left_button = 0x01;
      const gui::os::key_state middle_button = 0x02;
      const gui::os::key_state right_button = 0x04;

      const gui::os::key_state control = 0x10;
      const gui::os::key_state shift = 0x20;
      const gui::os::key_state alt = 0x40;
      const gui::os::key_state system = 0x80;

      const gui::os::key_state num_lock = 0x100;
      const gui::os::key_state scroll_lock = 0x200;
      const gui::os::key_state caps_lock = 0x400;
    }

    namespace keys {
      const gui::os::key_symbol home =        0xff50;
      const gui::os::key_symbol left =        0xff51;
      const gui::os::key_symbol up =          0xff52;
      const gui::os::key_symbol right =       0xff53;
      const gui::os::key_symbol down =        0xff54;
      const gui::os::key_symbol page_up =     0xff55;
      const gui::os::key_symbol page_down =   0xff56;
      const gui::os::key_symbol end =         0xff57;

      namespace numpad {  
        const gui::os::key_symbol home =      0xff95;
        const gui::os::key_symbol left =      0xff96;
        const gui::os::key_symbol up =        0xff97;
        const gui::os::key_symbol right =     0xff98;
        const gui::os::key_symbol down =      0xff99;
        const gui::os::key_symbol page_up =   0xff9a;
        const gui::os::key_symbol page_down = 0xff9b;
        const gui::os::key_symbol end =       0xff9c;

        const gui::os::key_symbol del =       0xff9f;
      }

      const gui::os::key_symbol del =         0xffff;
      const gui::os::key_symbol insert =      0xff63;
      const gui::os::key_symbol escape =      0x1b;
      const gui::os::key_symbol enter =       0x0d;
      const gui::os::key_symbol space =       0x20;
      const gui::os::key_symbol clear =       0x0b;

      const gui::os::key_symbol back_space =  0x08;
      const gui::os::key_symbol tab =         0x09;
      const gui::os::key_symbol print =       0xff61;

      const gui::os::key_symbol f1 =          0xffbe;
      const gui::os::key_symbol f2 =          0xffbf;
      const gui::os::key_symbol f3 =          0xffc0;
      const gui::os::key_symbol f4 =          0xffc1;
      const gui::os::key_symbol f5 =          0xffc2;
      const gui::os::key_symbol f6 =          0xffc3;
      const gui::os::key_symbol f7 =          0xffc4;
      const gui::os::key_symbol f8 =          0xffc5;
      const gui::os::key_symbol f9 =          0xffc6;
      const gui::os::key_symbol f10 =         0xffc7;
      const gui::os::key_symbol f11 =         0xffc8;
      const gui::os::key_symbol f12 =         0xffc9;
      const gui::os::key_symbol f13 =         0xffca;
      const gui::os::key_symbol f14 =         0xffcb;
      const gui::os::key_symbol f15 =         0xffcc;
      const gui::os::key_symbol f16 =         0xffcd;
      const gui::os::key_symbol f17 =         0xffce;
      const gui::os::key_symbol f18 =         0xffcf;
      const gui::os::key_symbol f19 =         0xffd0;
      const gui::os::key_symbol f20 =         0xffd1;
      const gui::os::key_symbol f21 =         0xffd2;
      const gui::os::key_symbol f22 =         0xffd3;
      const gui::os::key_symbol f23 =         0xffd4;
      const gui::os::key_symbol f24 =         0xffd5;

      const gui::os::key_symbol control =     0xffe3;
      const gui::os::key_symbol alt =         0xffe9;
      const gui::os::key_symbol shift =       0xffe1;
      const gui::os::key_symbol system =      0xffe7;

      const gui::os::key_symbol num_lock =    0xff7f;
      const gui::os::key_symbol scroll_lock = 0xffe6;
      const gui::os::key_symbol caps_lock =   0xffe5;

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

      const gui::os::key_symbol plus =      '+';
      const gui::os::key_symbol minus =     '-';
      const gui::os::key_symbol asterisk =  '*';
      const gui::os::key_symbol comma =     ',';
      const gui::os::key_symbol period =    '.';
      const gui::os::key_symbol slash =     '/';
      const gui::os::key_symbol colon =     ':';
      const gui::os::key_symbol semicolon = ';';
      const gui::os::key_symbol less =      '<';
      const gui::os::key_symbol equal =     '=';
      const gui::os::key_symbol greater =   '>';
      const gui::os::key_symbol question =  '?';
      const gui::os::key_symbol at =        '@';


    }

  } // namespace core

} // namespace gui

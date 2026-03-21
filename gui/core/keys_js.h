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
      const gui::os::key_symbol left = 0x51;
      const gui::os::key_symbol right = 0x53;
      const gui::os::key_symbol up = 0x52;
      const gui::os::key_symbol down = 0x54;

      const gui::os::key_symbol page_up = 0x55;
      const gui::os::key_symbol page_down = 0x56;
      const gui::os::key_symbol home = 0x50;
      const gui::os::key_symbol end = 0x57;

      namespace numpad {
        const gui::os::key_symbol left = 0xFF51;
        const gui::os::key_symbol right = 0xFF53;
        const gui::os::key_symbol up = 0xFF52;
        const gui::os::key_symbol down = 0xFF54;

        const gui::os::key_symbol page_up = 0xFF55;
        const gui::os::key_symbol page_down = 0xFF65;
        const gui::os::key_symbol home = 0xFF50;
        const gui::os::key_symbol end = 0xFF57;

        const gui::os::key_symbol del = 0xFF9F;
      }

      const gui::os::key_symbol del = 0xff;
      const gui::os::key_symbol insert = 0x63;
      const gui::os::key_symbol escape = 0x1b;
      const gui::os::key_symbol enter = 0x0d;
      const gui::os::key_symbol space = 0x20;
      const gui::os::key_symbol clear = 0x0b;

      const gui::os::key_symbol back_space = 0x08;
      const gui::os::key_symbol tab = 0x09;
      const gui::os::key_symbol print = 0x61;

      const gui::os::key_symbol f1 = 0xbe;
      const gui::os::key_symbol f2 = 0xbf;
      const gui::os::key_symbol f3 = 0xc0;
      const gui::os::key_symbol f4 = 0xc1;
      const gui::os::key_symbol f5 = 0xc2;
      const gui::os::key_symbol f6 = 0xc3;
      const gui::os::key_symbol f7 = 0xc4;
      const gui::os::key_symbol f8 = 0xc5;
      const gui::os::key_symbol f9 = 0xc6;
      const gui::os::key_symbol f10 = 0xc7;
      const gui::os::key_symbol f11 = 0xc8;
      const gui::os::key_symbol f12 = 0xc9;
      const gui::os::key_symbol f13 = 0xca;
      const gui::os::key_symbol f14 = 0xcb;
      const gui::os::key_symbol f15 = 0xcc;
      const gui::os::key_symbol f16 = 0xcd;
      const gui::os::key_symbol f17 = 0xce;
      const gui::os::key_symbol f18 = 0xcf;
      const gui::os::key_symbol f19 = 0xd0;
      const gui::os::key_symbol f20 = 0xd1;
      const gui::os::key_symbol f21 = 0xd2;
      const gui::os::key_symbol f22 = 0xd3;
      const gui::os::key_symbol f23 = 0xd4;
      const gui::os::key_symbol f24 = 0xd5;

      const gui::os::key_symbol control = 0xe3;
      const gui::os::key_symbol alt = 0xe9;
      const gui::os::key_symbol shift = 0xe1;
      const gui::os::key_symbol system = 0xeb;

      const gui::os::key_symbol num_lock = 0x7f;
      const gui::os::key_symbol scroll_lock = 0x14;
      const gui::os::key_symbol caps_lock = 0xe5;

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

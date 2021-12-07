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
 * @brief     mouse cursor definition
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace win {

    inline cursor::operator bool () const {
      return type != cursor_type::none;
    }

    inline bool cursor::operator== (const cursor& rhs) const {
      return type == rhs.type;
    }

    inline bool cursor::operator!= (const cursor& rhs) const {
      return type != rhs.type;
    }

  } // namespace win

} // namespace gui

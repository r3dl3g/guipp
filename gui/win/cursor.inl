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
 * @brief     C++ API: cursor definition
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

    inline cursor::cursor ()
      : type(0)
      , id(0)
    {}

    inline cursor::operator bool () const {
      return type != 0;
    }

    inline cursor::cursor (os::cursor_type t)
      : type(t)
      , id(0)
    {}

  } // namespace win

} // namespace gui

/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: bitmap definition
 *
 * @file
 */

#pragma once


namespace gui {

  namespace draw {

    inline core::uint32_size bitmap_info::size () const {
      return {width, height};
    }

    template<typename T>
    core::basic_size<T> bitmap_info::size () const {
        return {static_cast<T>(width), static_cast<T>(height)};
      }

    inline byte bitmap_info::depth () const {
      return static_cast<byte>(bits_per_pixel);
    }

    inline std::size_t bitmap_info::mem_size () const {
      return static_cast<size_t>(bytes_per_line) * static_cast<size_t>(height);
    }

  } // namespace draw

} // namespace gui

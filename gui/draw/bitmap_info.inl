/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

    inline core::native_size bitmap_info::size () const {
      return {width, height};
    }

    inline core::size bitmap_info::scaled_size () const {
        return core::global::scale_from_native(size());
      }

    inline byte bitmap_info::depth () const {
      return get_color_depth(pixel_format);
    }

    inline byte bitmap_info::bits_per_pixel () const {
      return get_bits_per_pixel(pixel_format);
    }

    inline std::size_t bitmap_info::mem_size () const {
      return static_cast<size_t>(bytes_per_line) * static_cast<size_t>(height);
    }

  } // namespace draw

} // namespace gui

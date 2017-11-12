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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>


namespace gui {

  namespace draw {

    struct bitmap_info {
      bitmap_info ();
      bitmap_info (uint32_t w, uint32_t h, BPP bpp);
      bitmap_info (uint32_t w, uint32_t h, uint32_t bpl, BPP bpp);

      uint32_t width;
      uint32_t height;
      uint32_t bytes_per_line;
      BPP bits_per_pixel;

      core::size size () const;
      byte depth () const;
      std::size_t mem_size () const;

    };

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    inline core::size bitmap_info::size () const {
      return {static_cast<core::size::type>(width), static_cast<core::size::type>(height)};
    }

    inline byte bitmap_info::depth () const {
      return static_cast<byte>(bits_per_pixel);
    }

    inline std::size_t bitmap_info::mem_size () const {
      return static_cast<size_t>(bytes_per_line) * static_cast<size_t>(height);
    }

  }

}
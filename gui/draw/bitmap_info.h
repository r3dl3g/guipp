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

    struct GUIPP_EXPORT bitmap_info {
      bitmap_info ();
      bitmap_info (const core::uint32_size&, BPP bpp);
      bitmap_info (uint32_t w, uint32_t h, BPP bpp);
      bitmap_info (uint32_t w, uint32_t h, uint32_t bpl, BPP bpp);

      uint32_t width;
      uint32_t height;
      uint32_t bytes_per_line;
      BPP bits_per_pixel;

      core::uint32_size size () const;

      template<typename T>
      core::basic_size<T> size () const;

      byte depth () const;
      std::size_t mem_size () const;

      bool operator== (const bitmap_info&) const;

    };

  } //namespace draw

} // namespace gui

#include <gui/draw/bitmap_info.inl>

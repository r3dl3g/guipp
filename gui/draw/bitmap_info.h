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
 * @brief     bitmap information
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>
#include <gui/draw/gui++-draw-export.h>


namespace gui {

  namespace draw {

    struct GUIPP_DRAW_EXPORT bitmap_info {
      bitmap_info ();
      bitmap_info (const core::native_size&, pixel_format_t px_fmt);
      bitmap_info (uint32_t w, uint32_t h, pixel_format_t px_fmt);
      bitmap_info (uint32_t w, uint32_t h, uint32_t bpl, pixel_format_t px_fmt);

      uint32_t width;
      uint32_t height;
      pixel_format_t pixel_format;
      uint32_t bytes_per_line;

      core::native_size size () const;

      core::size scaled_size () const;

      byte depth () const;
      byte bits_per_pixel () const;
      std::size_t mem_size () const;

      bool operator== (const bitmap_info&) const;

#ifdef GUIPP_QT

      static pixel_format_t convert (QImage::Format f);
      static QImage::Format convert (pixel_format_t f);

#endif // GUIPP_QT

    };

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const bitmap_info&);

    GUIPP_DRAW_EXPORT uint32_t bitmap_calc_bytes_per_line (uint32_t w, pixel_format_t px_fmt);

  } //namespace draw

} // namespace gui

#include <gui/draw/bitmap_info.inl>

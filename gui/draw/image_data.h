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
 * @brief     direct access image data
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

 // --------------------------------------------------------------------------
 //
 // Common includes
 //
#include <algorithm>
 
 // --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/pixel.h"
#include "gui/core/array_wrapper.h"
#include "gui/core/color.h"
#include "gui/draw/bitmap_info.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace draw {

    // --------------------------------------------------------------------------
    struct image_data_base {
      inline explicit image_data_base (const bitmap_info& info)
        : info(info)
      {}

      inline const bitmap_info& get_info () const {
        return info;
      }

      inline uint32_t width () const {
        return info.width;
      }

      inline uint32_t height () const {
        return info.height;
      }

    private:
      bitmap_info info;
    };

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    struct image_data : public image_data_base {
      using super = image_data_base;
      using pixel_type = typename pixel::BPP2Pixel<T>::pixel;
      using raw_type = core::array_wrapper<byte>;
      using row_type = core::array_wrapper<pixel_type>;
      using index_type = typename core::array_wrapper<pixel_type>::index_type;
      using const_index_type = typename core::array_wrapper<pixel_type>::const_index_type;
      using const_row_type = core::array_wrapper<pixel_type const>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      image_data (raw_type data, const bitmap_info& info);

      row_type row (uint32_t y);
      const const_row_type row (uint32_t y) const;

      index_type pixel (uint32_t x, uint32_t y);
      const const_index_type pixel (uint32_t x, uint32_t y) const;

      raw_type& raw_data ();
      const raw_type& raw_data () const;

      image_data& operator= (const image_data&);

    private:
      raw_type data;

    };

  } // namespace draw

} // namespace gui

#include "image_data.inl"

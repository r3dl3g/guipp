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


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline image_data<T>::image_data (raw_type data, const bitmap_info& info)
      : super(info)
      , data(data)
    {}

    template<pixel_format_t T>
    inline auto image_data<T>::row (uint32_t y) -> row_type {
      byte* row = data.data(y * get_info().bytes_per_line, get_info().bytes_per_line);
      using raw_t = typename row_type::raw_type;
      return row_type(reinterpret_cast<raw_t*>(row), width());
    }

    template<pixel_format_t T>
    inline auto image_data<T>::pixel (uint32_t x, uint32_t y) -> index_type {
      return row(y)[x];
    }

    template<pixel_format_t T>
    inline auto image_data<T>::raw_data () -> raw_type {
      return data;
    }

    template<pixel_format_t T>
    inline auto image_data<T>::row (uint32_t y) const -> const const_row_type {
      const byte* row = data.data(y * get_info().bytes_per_line, get_info().bytes_per_line);
      using raw_t = typename row_type::raw_type;
      return const_row_type(const_cast<raw_t*>(reinterpret_cast<const raw_t*>(row)), width());
    }

    template<pixel_format_t T>
    inline auto image_data<T>::pixel (uint32_t x, uint32_t y) const -> const const_index_type {
      return row(y)[x];
    }

    template<pixel_format_t T>
    inline auto image_data<T>::raw_data () const -> const_raw_type {
      return const_raw_type(data.data(0, get_info().mem_size()), get_info().mem_size());
    }

    template<pixel_format_t T>
    template<pixel_format_t S>
    image_data<T>& image_data<T>::copy_from (const image_data<S>& rhs) {
      if (get_info() == rhs.get_info()) {
        // copy 1:1
        data.copy_from(rhs.raw_data(), get_info().mem_size());
      } else {
        // copy row by row and possibly convert it
        auto h = std::min(height(), rhs.height());
        auto w = std::min(width(), rhs.width());
        for (uint_fast32_t y = 0; y < h; ++y) {
          auto in = rhs.row(y);
          auto out = row(y);
          for (uint_fast32_t x = 0; x < w; ++x) {
            out[x] = in[x];
          }
        }
      }
      return *this;
    }

    template<pixel_format_t T>
    template<pixel_format_t S>
    image_data<T>& image_data<T>::operator= (const image_data<S>& rhs) {
      return copy_from(rhs);
    }

    // --------------------------------------------------------------------------

  } //namespace draw

} // namespace gui

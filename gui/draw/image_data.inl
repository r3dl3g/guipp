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
    inline auto image_data<T>::raw_data () -> raw_type& {
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
    inline auto image_data<T>::raw_data () const -> const raw_type& {
      return data;
    }

    template<pixel_format_t T>
    image_data<T>& image_data<T>::operator= (const image_data<T>& rhs) {
      if (get_info() == rhs.get_info()) {
        // copy 1:1
        data.copy_from(rhs.raw_data(), get_info().mem_size());
      } else {
        // copy row:row
        auto rows = std::min(height(), rhs.height());
        auto length = std::min(get_info().bytes_per_line, rhs.get_info().bytes_per_line);
        for (int y = 0; y < rows; ++y) {
          raw_data().sub(y, length).copy_from(rhs.raw_data().sub(y, length), length);
        }
      }
      return *this;
    }

    // --------------------------------------------------------------------------

  } //namespace draw

} // namespace gui

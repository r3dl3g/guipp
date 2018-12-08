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


namespace basepp {

  template<>
  struct array_wrapper<gui::draw::bw_pixel> : public bit_array_wrapper<gui::draw::bw_pixel> {
    typedef bit_array_wrapper<gui::draw::bw_pixel> super;
    typedef super::type type;

    inline array_wrapper (type* data, size_t size)
      : super(data, size)
    {}

    inline array_wrapper (std::vector<type>& data)
      : super(data)
    {}
  };

  template<>
  struct array_wrapper<gui::draw::bw_pixel const> : public bit_array_wrapper<gui::draw::bw_pixel const> {
    typedef bit_array_wrapper<gui::draw::bw_pixel const> super;
    typedef super::type type;

    inline array_wrapper (const type* data, size_t size)
      : super(data, size)
    {}

    inline array_wrapper (const std::vector<type>& data)
      : super(data)
    {}
  };

} // namespace basepp

namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    inline gray_pixel gray_pixel::operator= (bw_pixel bw) {
      value = basepp::system_bw_bits::value[static_cast<bool>(bw)];
      return *this;
    }

    inline gray_pixel gray_pixel::operator= (rgb_pixel rgb) {
      *this = rgb.get_gray();
      return *this;
    }

    inline gray_pixel gray_pixel::operator= (rgba_pixel rgba) {
      *this = rgba.get_gray();
      return *this;
    }

    inline bw_pixel gray_pixel::get_bw () const {
      return value == 0 ? bw_pixel::black : bw_pixel::white;
    }

    // --------------------------------------------------------------------------
    inline void rgb_pixel::operator= (bw_pixel bw) {
      red = green = blue = basepp::system_bw_bits::value[static_cast<bool>(bw)];
    }

    inline void rgb_pixel::operator= (gray_pixel gray) {
      red = green = blue = gray.value;
    }
    inline void rgb_pixel::operator= (rgba_pixel rgba) {
      *this = static_cast<rgb_pixel>(rgba);
    }

    inline gray_pixel rgb_pixel::get_gray () const {
      return {static_cast<byte>((static_cast<uint16_t>(red) +
                                static_cast<uint16_t>(green) +
                                static_cast<uint16_t>(blue)) / 3)};
    }

    inline bw_pixel rgb_pixel::get_bw () const {
      return get_gray().get_bw();
    }

    // --------------------------------------------------------------------------
    inline rgba_pixel::rgba_pixel ()
    {}

    inline rgba_pixel::rgba_pixel (byte r, byte g, byte b, byte a)
      : rgb_pixel{r, g, b}
      , alpha(a)
    {}

    inline rgba_pixel::rgba_pixel (byte r, byte g, byte b)
      : rgb_pixel{r, g, b}
      , alpha{0}
    {}

    inline void rgba_pixel::operator= (bw_pixel bw) {
      rgb_pixel::operator=(bw);
      alpha = 0;
    }

    inline void rgba_pixel::operator= (gray_pixel gray) {
      rgb_pixel::operator=(gray);
      alpha = 0;
    }

    inline void rgba_pixel::operator= (rgb_pixel rgb) {
      rgb_pixel::operator=(rgb);
      alpha = 0;
    }

    // --------------------------------------------------------------------------
    inline bw_pixel operator* (bw_pixel p, float f) {
      return f == 0 ? bw_pixel::black : p;
    }

    inline byte pixel_mul (byte p, float f) {
      return static_cast<byte>(std::min<int>(0xff, static_cast<int>(p * f)));
    }

    inline gray_pixel operator* (gray_pixel p, float f) {
      return {pixel_mul(p.value, f)};
    }

    inline rgb_pixel operator* (rgb_pixel p, float f) {
      return {pixel_mul(p.red, f), pixel_mul(p.green, f), pixel_mul(p.blue, f)};
    }

    inline rgba_pixel operator* (rgba_pixel p, float f) {
      return {pixel_mul(p.red, f), pixel_mul(p.green, f), pixel_mul(p.blue, f), p.alpha};
    }

    // --------------------------------------------------------------------------
    template<BPP T>
    inline image_data<T>::image_data (raw_type data, const bitmap_info& info)
      : data(data)
      , info(info)
    {}

    template<BPP T>
    inline auto image_data<T>::row (uint32_t y) -> row_type {
      byte* row = data.data(y * info.bytes_per_line, info.bytes_per_line);
      using raw_type = typename row_type::type;
      return row_type(reinterpret_cast<raw_type*>(row), info.width);
    }

    template<BPP T>
    inline auto image_data<T>::pixel (uint32_t x, uint32_t y) -> pixel_type& {
      return row(y)[x];
    }

    template<BPP T>
    inline const bitmap_info& image_data<T>::get_info () const {
      return info;
    }

    template<BPP T>
    inline auto image_data<T>::raw_data () -> raw_type& {
      return data;
    }

    // --------------------------------------------------------------------------
    template<BPP T>
    inline const_image_data<T>::const_image_data (raw_type data, const bitmap_info& info)
      : data(data)
      , info(info)
    {}

    template<BPP T>
    inline auto const_image_data<T>::row (uint32_t y) const -> const row_type {
      const byte* row = data.data(y * info.bytes_per_line, info.bytes_per_line);
      using raw_type = const typename row_type::type;
      return row_type(reinterpret_cast<raw_type*>(row), info.width);
    }

    template<BPP T>
    inline auto const_image_data<T>::pixel (uint32_t x, uint32_t y) const -> pixel_type {
      return row(y)[x];
    }

    template<BPP T>
    inline const bitmap_info& const_image_data<T>::get_info () const {
      return info;
    }

    template<BPP T>
    inline auto const_image_data<T>::raw_data () const -> const raw_type& {
      return data;
    }

    // --------------------------------------------------------------------------

    template<BPP T>
    image_data<T>& image_data<T>::operator= (const image_data<T>& rhs) {
      if (info == rhs.get_info()) {
        // copy 1:1
        data.copy_from(rhs.raw_data(), info.mem_size());
      } else {
        // copy row:row
        auto rows = std::min(info.height, rhs.get_info().height);
        auto length = std::min(info.bytes_per_line, rhs.get_info().bytes_per_line);
        for (int y = 0; y < rows; ++y) {
          raw_data().sub(y, length).copy_from(rhs.raw_data().sub(y, length), length);
        }
      }
      return *this;
    }

    template<BPP T>
    image_data<T>& image_data<T>::operator= (const const_image_data<T>& rhs) {
      if (info == rhs.get_info()) {
        // copy 1:1
        data.copy_from(rhs.raw_data(), info.mem_size());
      } else {
        // copy row:row
        auto rows = std::min(info.height, rhs.get_info().height);
        auto length = std::min(info.bytes_per_line, rhs.get_info().bytes_per_line);
        for (int y = 0; y < rows; ++y) {
          raw_data().sub(y, length).copy_from(rhs.raw_data().sub(y, length), length);
        }
      }
      return *this;
    }

  } //namespace draw

} // namespace gui

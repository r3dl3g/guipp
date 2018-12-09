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

    inline gray_pixel gray_pixel::operator= (rgb_pixel p) {
      operator=(p.get_gray());
      return *this;
    }

    inline gray_pixel gray_pixel::operator= (rgba_pixel p) {
      operator=(p.get_gray());
      return *this;
    }

    inline gray_pixel gray_pixel::operator= (bgr_pixel p) {
      operator=(p.get_gray());
      return *this;
    }

    inline gray_pixel gray_pixel::operator= (bgra_pixel p) {
      operator=(p.get_gray());
      return *this;
    }

    inline gray_pixel gray_pixel::operator= (argb_pixel p) {
      operator=(p.get_gray());
      return *this;
    }

    inline gray_pixel gray_pixel::operator= (abgr_pixel p) {
      operator=(p.get_gray());
      return *this;
    }

    inline bw_pixel gray_pixel::get_bw () const {
      return value == 0 ? bw_pixel::black : bw_pixel::white;
    }

    // --------------------------------------------------------------------------
    inline void rgb_pixel::operator= (bw_pixel bw) {
      red = green = blue = basepp::system_bw_bits::value[static_cast<bool>(bw)];
    }

    inline void rgb_pixel::operator= (gray_pixel p) {
      red = green = blue = p.value;
    }

    inline void rgb_pixel::operator= (rgba_pixel P) {
      *this = static_cast<rgb_pixel>(P);
    }

    inline void rgb_pixel::operator= (bgr_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
    }

    inline void rgb_pixel::operator= (bgra_pixel P) {
      operator=(static_cast<bgr_pixel>(P));
    }

    inline void rgb_pixel::operator= (argb_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
    }

    inline void rgb_pixel::operator= (abgr_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
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

    inline void rgba_pixel::operator= (gray_pixel p) {
      rgb_pixel::operator=(p);
      alpha = 0;
    }

    inline void rgba_pixel::operator= (rgb_pixel p) {
      rgb_pixel::operator=(p);
      alpha = 0;
    }

    inline void rgba_pixel::operator= (bgr_pixel p) {
      rgb_pixel::operator=(p);
      alpha = 0;
    }

    inline void rgba_pixel::operator= (bgra_pixel p) {
      rgb_pixel::operator=(p);
      alpha = p.alpha;
    }

    inline void rgba_pixel::operator= (argb_pixel p) {
      rgb_pixel::operator=(p);
      alpha = p.alpha;
    }

    inline void rgba_pixel::operator= (abgr_pixel p) {
      rgb_pixel::operator=(p);
      alpha = p.alpha;
    }

    // --------------------------------------------------------------------------
    inline void bgr_pixel::operator= (bw_pixel bw) {
      red = green = blue = basepp::system_bw_bits::value[static_cast<bool>(bw)];
    }

    inline void bgr_pixel::operator= (gray_pixel p) {
      red = green = blue = p.value;
    }

    inline void bgr_pixel::operator= (rgb_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
    }

    inline void bgr_pixel::operator= (rgba_pixel P) {
      operator=(static_cast<rgb_pixel>(P));
    }

    inline void bgr_pixel::operator= (bgra_pixel P) {
      operator=(static_cast<bgr_pixel>(P));
    }

    inline void bgr_pixel::operator= (argb_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
    }

    inline void bgr_pixel::operator= (abgr_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
    }


    inline gray_pixel bgr_pixel::get_gray () const {
      return {static_cast<byte>((static_cast<uint16_t>(red) +
                                static_cast<uint16_t>(green) +
                                static_cast<uint16_t>(blue)) / 3)};
    }

    inline bw_pixel bgr_pixel::get_bw () const {
      return get_gray().get_bw();
    }


    // --------------------------------------------------------------------------
    inline bgra_pixel::bgra_pixel ()
    {}

    inline bgra_pixel::bgra_pixel (byte b, byte g, byte r, byte a)
      : bgr_pixel{b, g, r}
      , alpha(a)
    {}

    inline bgra_pixel::bgra_pixel (byte b, byte g, byte r)
      : bgr_pixel{b, g, r}
      , alpha{0}
    {}

    inline void bgra_pixel::operator= (bw_pixel bw) {
      bgr_pixel::operator=(bw);
      alpha = 0;
    }

    inline void bgra_pixel::operator= (gray_pixel p) {
      bgr_pixel::operator=(p);
      alpha = 0;
    }

    inline void bgra_pixel::operator= (rgb_pixel p) {
      bgr_pixel::operator=(p);
      alpha = 0;
    }

    inline void bgra_pixel::operator= (rgba_pixel p) {
      bgr_pixel::operator=(p);
      alpha = p.alpha;
    }

    inline void bgra_pixel::operator= (bgr_pixel p) {
      bgr_pixel::operator=(p);
      alpha = 0;
    }

    inline void bgra_pixel::operator= (argb_pixel p) {
      bgr_pixel::operator=(p);
      alpha = p.alpha;
    }

    inline void bgra_pixel::operator= (abgr_pixel p) {
      bgr_pixel::operator=(p);
      alpha = p.alpha;
    }

    // --------------------------------------------------------------------------
    inline void argb_pixel::operator= (bw_pixel p) {
      red = green = blue = basepp::system_bw_bits::value[static_cast<bool>(p)];
      alpha = 0;
    }

    inline void argb_pixel::operator= (gray_pixel p) {
      red = green = blue = p.value;
      alpha = 0;
    }

    inline void argb_pixel::operator= (rgb_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = 0;
    }

    inline void argb_pixel::operator= (rgba_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = p.alpha;
    }

    inline void argb_pixel::operator= (bgr_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = 0;
    }

    inline void argb_pixel::operator= (bgra_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = p.alpha;
    }

    inline void argb_pixel::operator= (abgr_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = p.alpha;
    }

    inline gray_pixel argb_pixel::get_gray () const {
      return {static_cast<byte>((static_cast<uint16_t>(red) +
                                static_cast<uint16_t>(green) +
                                static_cast<uint16_t>(blue)) / 3)};
    }

    inline bw_pixel argb_pixel::get_bw () const {
      return get_gray().get_bw();
    }

    // --------------------------------------------------------------------------
    inline void abgr_pixel::operator= (bw_pixel p) {
      red = green = blue = basepp::system_bw_bits::value[static_cast<bool>(p)];
      alpha = 0;
    }

    inline void abgr_pixel::operator= (gray_pixel p) {
      red = green = blue = p.value;
      alpha = 0;
    }

    inline void abgr_pixel::operator= (rgb_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = 0;
    }

    inline void abgr_pixel::operator= (rgba_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = p.alpha;
    }

    inline void abgr_pixel::operator= (bgr_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = 0;
    }

    inline void abgr_pixel::operator= (bgra_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = p.alpha;
    }

    inline void abgr_pixel::operator= (argb_pixel p) {
      red = p.red;
      green = p.green;
      blue = p.blue;
      alpha = p.alpha;
    }

    inline gray_pixel abgr_pixel::get_gray () const {
      return {static_cast<byte>((static_cast<uint16_t>(red) +
                                static_cast<uint16_t>(green) +
                                static_cast<uint16_t>(blue)) / 3)};
    }

    inline bw_pixel abgr_pixel::get_bw () const {
      return get_gray().get_bw();
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

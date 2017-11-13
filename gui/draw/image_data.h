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
 // Common includes
 //
#include <algorithm>
 
 // --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/array_wrapper.h>
#include <gui/draw/bitmap_info.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    enum class bw_pixel : bool {
      black = false,
      white = true
    };

  }

  namespace core {

    template<>
    struct array_wrapper<draw::bw_pixel> : public bool_array_wrapper<draw::bw_pixel> {
      typedef bool_array_wrapper<draw::bw_pixel> super;

      inline array_wrapper (byte* data, size_t size)
        : super(data, size)
      {}

      inline array_wrapper (std::vector<type>& data)
        : super(data)
      {}
    };

    template<>
    struct array_wrapper<draw::bw_pixel const> : public bool_array_wrapper<draw::bw_pixel const> {
      typedef bool_array_wrapper<draw::bw_pixel const> super;

      inline array_wrapper (const byte* data, size_t size)
        : super(data, size)
      {}

      inline array_wrapper (const std::vector<type>& data)
        : super(data)
      {}
    };

  }

  namespace draw {

#pragma pack(push, 1)

    // --------------------------------------------------------------------------
    struct rgb_pixel;
    struct rgba_pixel;

    // --------------------------------------------------------------------------
    struct gray_pixel {
      byte value;

      gray_pixel operator= (bw_pixel bw);
      gray_pixel operator= (rgb_pixel rgb);
      gray_pixel operator= (rgba_pixel rgba);

      bw_pixel get_bw () const;
    };

    // --------------------------------------------------------------------------
    struct rgb_pixel {
      gray_pixel red;
      gray_pixel green;
      gray_pixel blue;

      void operator= (bw_pixel bw);
      void operator= (gray_pixel gray);
      void operator= (rgba_pixel rgba);

      gray_pixel get_gray () const;
      bw_pixel get_bw () const;
    };

    // --------------------------------------------------------------------------
    struct rgba_pixel : public rgb_pixel {
      rgba_pixel ()
      {}

      rgba_pixel (gray_pixel r, gray_pixel g, gray_pixel b, gray_pixel a)
        : rgb_pixel{r, g, b}
        , alpha(a)
      {}

      rgba_pixel (gray_pixel r, gray_pixel g, gray_pixel b)
        : rgb_pixel{r, g, b}
        , alpha{0}
      {}

      void operator= (bw_pixel bw);
      void operator= (gray_pixel gray);
      void operator= (rgb_pixel rgb);

      gray_pixel alpha;
    };
#pragma pack(pop)

    // --------------------------------------------------------------------------
    inline gray_pixel gray_pixel::operator= (bw_pixel bw) {
      value = system_bw_bits::adapt(static_cast<bool>(bw) ? 0xff : 0);
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
      return value == system_bw_bits::value[1] ? bw_pixel::white : bw_pixel::black;
    }

    // --------------------------------------------------------------------------
    inline void rgb_pixel::operator= (bw_pixel bw) {
      red = green = blue = bw;
    }

    inline void rgb_pixel::operator= (gray_pixel gray) {
      red = green = blue = gray;
    }
    inline void rgb_pixel::operator= (rgba_pixel rgba) {
      *this = static_cast<rgb_pixel>(rgba);
    }

    inline gray_pixel rgb_pixel::get_gray () const {
      return {static_cast<byte>((static_cast<uint16_t>(red.value) +
                                static_cast<uint16_t>(green.value) +
                                static_cast<uint16_t>(blue.value)) / 3)};
    }

    inline bw_pixel rgb_pixel::get_bw () const {
      return get_gray().get_bw();
    }

    // --------------------------------------------------------------------------
    inline void rgba_pixel::operator= (bw_pixel bw) {
      rgb_pixel::operator=(bw);
      alpha.value = 0;
    }

    inline void rgba_pixel::operator= (gray_pixel gray) {
      rgb_pixel::operator=(gray);
      alpha.value = 0;
    }

    inline void rgba_pixel::operator= (rgb_pixel rgb) {
      rgb_pixel::operator=(rgb);
      alpha.value = 0;
    }

    // --------------------------------------------------------------------------
    inline bw_pixel operator* (bw_pixel p, float f) {
      return f == 0 ? bw_pixel::black : p;
    }

    inline gray_pixel operator* (gray_pixel p, float f) {
      return {static_cast<byte>(std::min<int>(0xff, static_cast<int>(p.value * f)))};
    }

    inline rgb_pixel operator* (rgb_pixel p, float f) {
      return {p.red * f, p.green * f, p.blue * f};
    }

    inline rgba_pixel operator* (rgba_pixel p, float f) {
      return {p.red * f, p.green * f, p.blue * f, p.alpha};
    }

    // --------------------------------------------------------------------------
    template<BPP T> struct BPP2Pixel {};
    template<> struct BPP2Pixel<BPP::BW>    { using pixel = bw_pixel; };
    template<> struct BPP2Pixel<BPP::GRAY>  { using pixel = gray_pixel ; };
    template<> struct BPP2Pixel<BPP::RGB>   { using pixel = rgb_pixel; };
    template<> struct BPP2Pixel<BPP::RGBA>  { using pixel = rgba_pixel; };

    // --------------------------------------------------------------------------
    template<BPP T>
    using image_row = core::array_wrapper<typename BPP2Pixel<T>::pixel>;

    // --------------------------------------------------------------------------
    template<BPP T>
    struct image_data {
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = core::array_wrapper<byte>;
      using row_type = core::array_wrapper<pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      inline image_data (raw_type data, const bitmap_info& info)
        : data(data)
        , info(info)
      {}

      inline row_type row (uint32_t y) {
        byte* row = data.data(y * info.bytes_per_line, info.bytes_per_line);
        using raw_type = typename row_type::type;
        return row_type(reinterpret_cast<raw_type*>(row), info.width);
      }

      pixel_type& pixel (uint32_t x, uint32_t y) {
        return row(y)[x];
      }

    private:
      raw_type data;
      bitmap_info info;
    };

    // --------------------------------------------------------------------------
    template<BPP T>
    using const_image_row = core::array_wrapper<const typename BPP2Pixel<T>::pixel>;

    // --------------------------------------------------------------------------
    template<BPP T>
    struct const_image_data {
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = core::array_wrapper<const byte>;
      using row_type = core::array_wrapper<const pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      inline const_image_data (raw_type data, const bitmap_info& info)
        : data(data)
        , info(info)
      {}

      inline const row_type row (uint32_t y) const {
        const byte* row = data.data(y * info.bytes_per_line, info.bytes_per_line);
        using raw_type = const typename row_type::type;
        return row_type(reinterpret_cast<raw_type*>(row), info.width);
      }

      pixel_type pixel (uint32_t x, uint32_t y) const {
        return row(y)[x];
      }

    private:
      raw_type data;
      bitmap_info info;
    };

  } //namespace draw

} // namespace gui

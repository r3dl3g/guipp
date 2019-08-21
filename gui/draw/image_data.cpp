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

 // --------------------------------------------------------------------------
 //
 // Common includes
 //
 
 // --------------------------------------------------------------------------
//
// Library includes
//
#include "image_data.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace pixel {

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const bw_pixel& px) {
      out << static_cast<bool>(px);
      return out;
    }

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const gray_pixel& px) {
      out << static_cast<int>(px.value);
      return out;
    }

    constexpr gray_pixel color<gray_pixel>::black;
    constexpr gray_pixel color<gray_pixel>::white;

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const rgb_pixel& px) {
      out << "r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue);
      return out;
    }

    constexpr rgb_pixel color<rgb_pixel>::black;
    constexpr rgb_pixel color<rgb_pixel>::white;

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const rgba_pixel& px) {
      out << "r:" << static_cast<int>(px.red)
        << ", g:" << static_cast<int>(px.green)
        << ", b:" << static_cast<int>(px.blue)
        << ", a:" << static_cast<int>(px.alpha);
      return out;
    }

    constexpr rgba_pixel color<rgba_pixel>::black;
    constexpr rgba_pixel color<rgba_pixel>::white;

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const bgr_pixel& px) {
      out << "b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red);
      return out;
    }

    constexpr bgr_pixel color<bgr_pixel>::black;
    constexpr bgr_pixel color<bgr_pixel>::white;

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const bgra_pixel& px) {
      out << "b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red)
          << ", a:" << static_cast<int>(px.alpha);
      return out;
    }

    constexpr bgra_pixel color<bgra_pixel>::black;
    constexpr bgra_pixel color<bgra_pixel>::white;

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const argb_pixel& px) {
      out << "a:" << static_cast<int>(px.alpha)
          << ", r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue);
      return out;
    }

    constexpr argb_pixel color<argb_pixel>::black;
    constexpr argb_pixel color<argb_pixel>::white;

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const abgr_pixel& px) {
      out << "a:" << static_cast<int>(px.alpha)
          << ", b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red);
      return out;
    }

    constexpr abgr_pixel color<abgr_pixel>::black;
    constexpr abgr_pixel color<abgr_pixel>::white;

    // --------------------------------------------------------------------------
    byte get_alpha (bw_pixel) {
      return 0;//IF_WIN32_ELSE(0, 255);
    }

    byte get_alpha (gray_pixel) {
      return 0;//IF_WIN32_ELSE(0, 255);
    }

    byte get_alpha (rgb_pixel) {
      return 0;//IF_WIN32_ELSE(0, 255);
    }

    byte get_alpha (bgr_pixel) {
      return 0;//IF_WIN32_ELSE(0, 255);
    }

  } // namespace pixel

  namespace draw {

    // --------------------------------------------------------------------------
//    template<>
//    image_data<PixelFormat::BW>::image_data (raw_type data, const bitmap_info& info)
//      : data(data)
//      , info(info)
//    {}

//    template<>
//    auto image_data<PixelFormat::BW>::row (uint32_t y) -> row_type {
//      byte* row = data.data(y * info.bytes_per_line, info.bytes_per_line);
//      using raw_type = typename row_type::type;
//      return row_type(reinterpret_cast<raw_type*>(row), info.width);
//    }

//    template<>
//    auto image_data<PixelFormat::BW>::pixel (uint32_t x, uint32_t y) -> pixel_type {
//      return row(y)[x];
//    }

//    template<>
//    const bitmap_info& image_data<PixelFormat::BW>::get_info () const {
//      return info;
//    }

//    template<>
//    auto image_data<PixelFormat::BW>::raw_data () -> raw_type& {
//      return data;
//    }

  } // namespace draw

} // namespace gui
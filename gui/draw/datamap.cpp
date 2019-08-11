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
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/datamap.h>


namespace gui {

  namespace draw {

    basic_datamap::basic_datamap (const blob& data, const bitmap_info& info)
      : data(data)
      , info(info)
    {}

    basic_datamap::basic_datamap (blob&& data, bitmap_info&& info)
      : data(std::move(data))
      , info(std::move(info))
    {}

    bool basic_datamap::is_valid () const {
      return data.size() > 0;
    }

    basic_datamap::operator bool () const {
      return is_valid();
    }

    const bitmap_info& basic_datamap::get_info () const {
      return info;
    }

    bitmap_info& basic_datamap::get_info () {
      return info;
    }

    core::size basic_datamap::size () const {
      return info.size<core::size::type>();
    }

    byte basic_datamap::depth () const {
      return info.depth();
    }

    PixelFormat basic_datamap::pixel_format () const {
      return info.pixel_format;
    }

    void basic_datamap::clear () {
      data.clear();
    }

    core::basic_rectangle<uint32_t> checked_area (const bitmap_info& bmi, const core::rectangle& area) {
      const uint32_t x = std::max<uint32_t>(0, std::min<uint32_t>(bmi.width, basepp::roundup<uint32_t>(area.x())));
      const uint32_t y = std::max<uint32_t>(0, std::min<uint32_t>(bmi.height, basepp::roundup<uint32_t>(area.y())));
      const uint32_t w = std::min<uint32_t>(bmi.width - x, basepp::roundup<uint32_t>(area.width()));
      const uint32_t h = std::min<uint32_t>(bmi.height - y, basepp::roundup<uint32_t>(area.height()));
      return {x, y, w, h};
    }

    core::basic_rectangle<uint32_t> checked_area (const bitmap_info& bmi, const core::point& pt, const core::uint32_size& sz) {
      const uint32_t x = std::max<uint32_t>(0, std::min<uint32_t>(bmi.width, basepp::roundup<uint32_t>(pt.x())));
      const uint32_t y = std::max<uint32_t>(0, std::min<uint32_t>(bmi.height, basepp::roundup<uint32_t>(pt.y())));
      const uint32_t w = std::min<uint32_t>(bmi.width - x, sz.width());
      const uint32_t h = std::min<uint32_t>(bmi.height - y, sz.height());
      return {x, y, w, h};
    }

  } // namespace draw

} // namespace gui

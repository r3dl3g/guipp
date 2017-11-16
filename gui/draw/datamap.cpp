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

    void basic_datamap::create (const blob& data, const bitmap_info& bmi) {
      this->data = data;
      info = bmi;
    }

    BPP basic_datamap::bits_per_pixel () const {
      return info.bits_per_pixel;
    }

    void basic_datamap::clear () {
      data.clear();
    }

    core::basic_rectangle<uint32_t> checked_area (const bitmap_info& bmi, const core::rectangle& area) {
      const uint32_t x = std::max<uint32_t>(0, std::min<uint32_t>(bmi.width, roundup<uint32_t>(area.x())));
      const uint32_t y = std::max<uint32_t>(0, std::min<uint32_t>(bmi.height, roundup<uint32_t>(area.y())));
      const uint32_t w = std::min<uint32_t>(bmi.width - x, roundup<uint32_t>(area.width()));
      const uint32_t h = std::min<uint32_t>(bmi.height - y, roundup<uint32_t>(area.height()));
      return {x, y, w, h};
    }

    core::basic_rectangle<uint32_t> checked_area (const bitmap_info& bmi, const core::point& pt, const core::uint32_size& sz) {
      const uint32_t x = std::max<uint32_t>(0, std::min<uint32_t>(bmi.width, roundup<uint32_t>(pt.x())));
      const uint32_t y = std::max<uint32_t>(0, std::min<uint32_t>(bmi.height, roundup<uint32_t>(pt.y())));
      const uint32_t w = std::min<uint32_t>(bmi.width - x, sz.width());
      const uint32_t h = std::min<uint32_t>(bmi.height - y, sz.height());
      return {x, y, w, h};
    }

  } // namespace draw

} // namespace gui

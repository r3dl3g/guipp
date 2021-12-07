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
 * @brief     direct access image_data
 *
 * @license   MIT license. See accompanying file LICENSE.
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
      : info(info)
      , data(data)
    {}

    basic_datamap::basic_datamap (blob&& data, bitmap_info&& info)
      : info(std::move(info))
      , data(std::move(data))
    {}

    bool basic_datamap::is_valid () const {
      return !data.empty();
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

    const blob& basic_datamap::get_raw () const {
      return data;
    }

    core::native_size basic_datamap::native_size () const {
      return info.size();
    }

    core::size basic_datamap::scaled_size () const {
      return info.scaled_size();
    }

    byte basic_datamap::depth () const {
      return info.depth();
    }

    pixel_format_t basic_datamap::pixel_format () const {
      return info.pixel_format;
    }

    void basic_datamap::clear () {
      data.clear();
    }

    core::native_rect checked_area (const bitmap_info& bmi, const core::native_rect& area) {
      const int32_t x = std::max<int32_t>(0, std::min<int32_t>(bmi.width, area.x()));
      const int32_t y = std::max<int32_t>(0, std::min<int32_t>(bmi.height, area.y()));
      const uint32_t w = std::min<uint32_t>(bmi.width - x, area.width());
      const uint32_t h = std::min<uint32_t>(bmi.height - y, area.height());
      return {x, y, w, h};
    }

    core::native_rect checked_area (const bitmap_info& bmi, const core::native_point& pt, const core::native_size& sz) {
      const int32_t x = std::max<int32_t>(0, std::min<int32_t>(bmi.width, pt.x()));
      const int32_t y = std::max<int32_t>(0, std::min<int32_t>(bmi.height, pt.y()));
      const uint32_t w = std::min<uint32_t>(bmi.width - x, sz.width());
      const uint32_t h = std::min<uint32_t>(bmi.height - y, sz.height());
      return {x, y, w, h};
    }

  } // namespace draw

} // namespace gui

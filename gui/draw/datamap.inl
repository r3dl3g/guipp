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
// Library includes
//
#include <gui/draw/converter.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    template<BPP T>
    inline datamap<T>::datamap ()
    {}

    template<BPP T>
    inline datamap<T>::datamap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    template<BPP T>
    inline datamap<T>::datamap (const core::uint32_size& sz) {
      create(sz);
    }

    template<BPP T>
    inline datamap<T>::datamap (const const_image_data<T>& data) {
      create(data);
    }

    template<BPP T>
    inline datamap<T>::datamap (const core::size& sz) {
      create(sz);
    }

    template<BPP T>
    template<BPP S>
    inline datamap<T>::datamap (const datamap<S>& src) {
      const bitmap_info& bmi = src.get_info();

      const auto w = bmi.width;
      const auto h = bmi.height;

      info = {w, h, T};
      data.resize(info.mem_size());

      convert::bpp::convert<S, T>(src.get_raw(), get_raw(), w, h);
    }

    template<BPP T>
    const datamap<T> basic_datamap::convert () const {
      const bitmap_info& bmi = get_info();

      if (T == bmi.bits_per_pixel) {
        return datamap<T>(get_raw<T>());
      } else {
        const auto w = bmi.width;
        const auto h = bmi.height;

        datamap<T> dest(w, h);
        switch (bmi.bits_per_pixel) {
          case BPP::BW:   convert::bpp::convert<BPP::BW, T>(get_raw<BPP::BW>(), dest.get_raw(), w, h); break;
          case BPP::GRAY: convert::bpp::convert<BPP::GRAY, T>(get_raw<BPP::GRAY>(), dest.get_raw(), w, h); break;
          case BPP::RGB:  convert::bpp::convert<BPP::RGB, T>(get_raw<BPP::RGB>(), dest.get_raw(), w, h); break;
          case BPP::RGBA: convert::bpp::convert<BPP::RGBA, T>(get_raw<BPP::RGBA>(), dest.get_raw(), w, h); break;
        }
        return dest;
      }
    }

    template<BPP T>
    inline void datamap<T>::create (uint32_t w, uint32_t h) {
      info = {w, h, T};
      data.resize(info.mem_size());
    }

    template<BPP T>
    inline void datamap<T>::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    template<BPP T>
    inline void datamap<T>::create (const core::uint32_size& sz) {
      create(sz.width(), sz.height());
    }

    template<BPP T>
    inline void datamap<T>::create (const const_image_data<T>& rhs) {
      const bitmap_info& bmi = rhs.get_info();
      info = bitmap_info(bmi.width, bmi.height, T);
      auto sz = info.mem_size();
      data.resize(sz);
      image_data<T> lhs(data, info);
      lhs = rhs;
    }

    template<BPP T>
    inline void datamap<T>::copy_from (const datamap& src_img,
                                       const core::rectangle& src_rect,
                                       const core::point& dest_pt) {
      bitmap_info src_bmi = src_img.get_info();

      auto src = checked_area(src_bmi, src_rect);
      auto dest = checked_area(info, dest_pt, src.size());

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::copy::sub<T>(src_img.get_raw(), get_raw(),
                            src.position(), dest);
    }

    template<BPP T>
    inline void datamap<T>::crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
      datamap bmp(w, h);
      bmp.copy_from(*this, core::rectangle(x, y, w, h), core::point::zero);
      std::swap(data, bmp.data);
      std::swap(info, bmp.info);
    }

    template<BPP T>
    inline void datamap<T>::stretch_from (const datamap& src) {
      stretch_from(src, core::rectangle(src.size()), core::rectangle(size()));
    }

    template<BPP T>
    inline void datamap<T>::stretch_from (const datamap& src_img,
                                          const core::rectangle& src_rect,
                                          const core::rectangle& dest_rect) {
      bitmap_info src_bmi = src_img.get_info();

      auto src = checked_area(src_bmi, src_rect);
      auto dest = checked_area(info, dest_rect);

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::stretch::sub<T>(src_img.get_raw(), get_raw(), src, dest);
    }

    template<BPP T>
    inline void datamap<T>::adjust_brightness (float f) {
      convert::brightness::adjust<T>(get_raw(), info.width, info.height, f);
    }

    template<BPP T>
    inline auto datamap<T>::sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const -> datamap {
      datamap bmp(w, h);
      bmp.copy_from(*this, 
                    core::rectangle(core::point::type(x), core::point::type(y), core::size::type(w), core::size::type(h)),
                    core::point::zero);
      return bmp;
    }

    template<BPP T>
    inline auto datamap<T>::brightness (float f) const -> datamap {
      datamap bmp = *this;
      bitmap_info& bmi = bmp.info;
      convert::brightness::adjust<T>(bmp.get_raw(), bmi.width, bmi.height, f);
      return bmp;
    }

    template<BPP T>
    inline void datamap<T>::invert () {
      for (auto& c : data) {
        c = ~c;
      }
    }

  }

}

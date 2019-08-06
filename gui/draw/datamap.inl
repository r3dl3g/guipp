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
    template<PixelFormat T>
    inline datamap<T>::datamap ()
    {}

    template<PixelFormat T>
    inline datamap<T>::datamap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    template<PixelFormat T>
    inline datamap<T>::datamap (const core::uint32_size& sz) {
      create(sz);
    }

    template<PixelFormat T>
    inline datamap<T>::datamap (const const_image_data<T>& data) {
      create(data);
    }

    template<PixelFormat T>
    inline datamap<T>::datamap (const core::size& sz) {
      create(sz);
    }

    template<PixelFormat T>
    template<PixelFormat S>
    inline datamap<T>::datamap (const datamap<S>& src)
      : datamap(src.get_data())
    {}

    template<PixelFormat T>
    template<PixelFormat S>
    inline datamap<T>::datamap (const const_image_data<S>& src) {
      const bitmap_info& bmi = src.get_info();

      const auto w = bmi.width;
      const auto h = bmi.height;

      info = {w, h, T};
      data.resize(info.mem_size());

      convert::format::convert<S, T>(src, get_data(), w, h);
    }

    template<PixelFormat T>
    inline datamap<T>::datamap (const blob& data, const bitmap_info& bmi) {
      super::create(data, bmi);
    }

    template<PixelFormat T>
    const datamap<T> basic_datamap::convert () const {
      const bitmap_info& bmi = get_info();

      if (T == bmi.pixel_format) {
        return datamap<T>(get_data<T>());
      } else {
        const auto w = bmi.width;
        const auto h = bmi.height;

        datamap<T> dest(w, h);
        switch (bmi.pixel_format) {
          case PixelFormat::BW:   convert::format::convert<PixelFormat::BW,   T>(get_data<PixelFormat::BW>(),   dest.get_data(), w, h); break;
          case PixelFormat::GRAY: convert::format::convert<PixelFormat::GRAY, T>(get_data<PixelFormat::GRAY>(), dest.get_data(), w, h); break;
          case PixelFormat::RGB:  convert::format::convert<PixelFormat::RGB,  T>(get_data<PixelFormat::RGB>(),  dest.get_data(), w, h); break;
          case PixelFormat::RGBA: convert::format::convert<PixelFormat::RGBA, T>(get_data<PixelFormat::RGBA>(), dest.get_data(), w, h); break;
          case PixelFormat::BGR:  convert::format::convert<PixelFormat::BGR,  T>(get_data<PixelFormat::BGR>(),  dest.get_data(), w, h); break;
          case PixelFormat::BGRA: convert::format::convert<PixelFormat::BGRA, T>(get_data<PixelFormat::BGRA>(), dest.get_data(), w, h); break;
          case PixelFormat::ARGB: convert::format::convert<PixelFormat::ARGB, T>(get_data<PixelFormat::ARGB>(), dest.get_data(), w, h); break;
          case PixelFormat::ABGR: convert::format::convert<PixelFormat::ABGR, T>(get_data<PixelFormat::ABGR>(), dest.get_data(), w, h); break;
        }
        return dest;
      }
    }

    template<PixelFormat T>
    inline void datamap<T>::create (uint32_t w, uint32_t h) {
      info = {w, h, T};
      data.resize(info.mem_size());
    }

    template<PixelFormat T>
    inline void datamap<T>::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    template<PixelFormat T>
    inline void datamap<T>::create (const core::uint32_size& sz) {
      create(sz.width(), sz.height());
    }

    template<PixelFormat T>
    inline void datamap<T>::create (const const_image_data<T>& rhs) {
      const bitmap_info& bmi = rhs.get_info();
      info = bitmap_info(bmi.width, bmi.height, T);
      auto sz = info.mem_size();
      data.resize(sz);
      memcpy(data.data(), rhs.raw_data().data(0, sz), sz);
//      data.assign(raw, raw + sz);
    }

    template<PixelFormat T>
    inline void datamap<T>::copy_from (const datamap& src_img,
                                       const core::rectangle& src_rect,
                                       const core::point& dest_pt) {
      bitmap_info src_bmi = src_img.get_info();

      auto src = checked_area(src_bmi, src_rect);
      auto dest = checked_area(info, dest_pt, src.size());

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::copy::sub<T>(src_img.get_data(), get_data(),
                            src.position(), dest);
    }

    template<PixelFormat T>
    inline void datamap<T>::crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
      datamap bmp(w, h);
      bmp.copy_from(*this, core::rectangle(x, y, w, h), core::point::zero);
      std::swap(data, bmp.data);
      std::swap(info, bmp.info);
    }

    template<PixelFormat T>
    inline void datamap<T>::stretch_from (const datamap& src) {
      stretch_from(src, core::rectangle(src.size()), core::rectangle(size()));
    }

    template<PixelFormat T>
    inline void datamap<T>::stretch_from (const datamap& src_img,
                                          const core::rectangle& src_rect,
                                          const core::rectangle& dest_rect) {
      bitmap_info src_bmi = src_img.get_info();

      auto src = checked_area(src_bmi, src_rect);
      auto dest = checked_area(info, dest_rect);

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::stretch::sub<T>(src_img.get_data(), get_data(), src, dest);
    }

    template<PixelFormat T>
    inline void datamap<T>::adjust_brightness (float f) {
      convert::brightness::adjust<T>(get_data(), info.width, info.height, f);
    }

    template<PixelFormat T>
    inline auto datamap<T>::sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const -> datamap {
      datamap bmp(w, h);
      bmp.copy_from(*this, 
                    core::rectangle(core::point::type(x), core::point::type(y), core::size::type(w), core::size::type(h)),
                    core::point::zero);
      return bmp;
    }

    template<PixelFormat T>
    inline auto datamap<T>::brightness (float f) const -> datamap {
      datamap bmp = *this;
      bitmap_info& bmi = bmp.info;
      convert::brightness::adjust<T>(bmp.get_data(), bmi.width, bmi.height, f);
      return bmp;
    }

    template<PixelFormat T>
    inline void datamap<T>::invert () {
      for (auto& c : data) {
        c = ~c;
      }
    }

    template<PixelFormat T>
    inline void datamap<T>::fill (const pixel_type& c) {
      convert::fill::fill<T>(get_data(), info.width, info.height, c);
    }

  }

}

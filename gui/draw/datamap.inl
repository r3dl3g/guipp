/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    template<pixel_format_t S>
    datamap<S> basic_datamap::convert () const {
      const bitmap_info& bmi = get_info();

      if (S == bmi.pixel_format) {
        return datamap<S>(const_reinterpret<S>());
      } else {
        const auto w = bmi.width;
        const auto h = bmi.height;

        datamap<S> dest(w, h);
        switch (bmi.pixel_format) {
          case pixel_format_t::BW:   convert::format::convert<pixel_format_t::BW,   S>(const_reinterpret<pixel_format_t::BW>(),   dest.get_data(), w, h); break;
          case pixel_format_t::GRAY: convert::format::convert<pixel_format_t::GRAY, S>(const_reinterpret<pixel_format_t::GRAY>(), dest.get_data(), w, h); break;
          case pixel_format_t::RGB:  convert::format::convert<pixel_format_t::RGB,  S>(const_reinterpret<pixel_format_t::RGB>(),  dest.get_data(), w, h); break;
          case pixel_format_t::RGBA: convert::format::convert<pixel_format_t::RGBA, S>(const_reinterpret<pixel_format_t::RGBA>(), dest.get_data(), w, h); break;
          case pixel_format_t::BGR:  convert::format::convert<pixel_format_t::BGR,  S>(const_reinterpret<pixel_format_t::BGR>(),  dest.get_data(), w, h); break;
          case pixel_format_t::BGRA: convert::format::convert<pixel_format_t::BGRA, S>(const_reinterpret<pixel_format_t::BGRA>(), dest.get_data(), w, h); break;
          case pixel_format_t::ARGB: convert::format::convert<pixel_format_t::ARGB, S>(const_reinterpret<pixel_format_t::ARGB>(), dest.get_data(), w, h); break;
          case pixel_format_t::ABGR: convert::format::convert<pixel_format_t::ABGR, S>(const_reinterpret<pixel_format_t::ABGR>(), dest.get_data(), w, h); break;
          default: break;
        }
        return dest;
      }
    }

    template<pixel_format_t T>
    inline datamap<T>::datamap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    template<pixel_format_t T>
    inline datamap<T>::datamap (const core::native_size& sz) {
      create(sz);
    }

    template<pixel_format_t T>
    inline datamap<T>::datamap (const const_image_data<T>& data) {
      create(data);
    }

    template<pixel_format_t T>
    inline datamap<T>::datamap (const core::size& sz) {
      create(sz);
    }

    template<pixel_format_t T>
    template<pixel_format_t S>
    inline datamap<T>::datamap (const datamap<S>& src)
      : datamap(src.get_data())
    {}

    template<pixel_format_t T>
    template<pixel_format_t S>
    inline datamap<T>::datamap (const const_image_data<S>& src) {
      const bitmap_info& bmi = src.get_info();

      const auto w = bmi.width;
      const auto h = bmi.height;

      info = {w, h, T};
      data.resize(info.mem_size());

      convert::format::convert<S, T>(src, get_data(), w, h);
    }

    template<pixel_format_t T>
    inline datamap<T>::datamap (const blob& data, const bitmap_info& bmi)
      : super(data, bmi)
    {}

    template<pixel_format_t T>
    inline datamap<T>::datamap (blob&& data, bitmap_info&& bmi)
      : super(std::move(data), std::move(bmi))
    {}

    template<pixel_format_t T>
    inline void datamap<T>::create (uint32_t w, uint32_t h) {
      info = {w, h, T};
      data.resize(info.mem_size());
    }

    template<pixel_format_t T>
    inline void datamap<T>::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    template<pixel_format_t T>
    inline void datamap<T>::create (const core::native_size& sz) {
      create(sz.width(), sz.height());
    }

    template<pixel_format_t T>
    inline void datamap<T>::create (const const_image_data<T>& rhs) {
      const bitmap_info& bmi = rhs.get_info();
      info = bitmap_info(bmi.width, bmi.height, T);
      const auto sz = info.mem_size();
      data.resize(sz);
      if (sz == bmi.mem_size()) {
        memcpy(data.data(), rhs.raw_data().data(0, sz), sz);
      } else {
        byte* in = data.data();
        const byte* out = rhs.raw_data().data(0, bmi.mem_size());
        const auto bytes_per_line = std::min(info.bytes_per_line, bmi.bytes_per_line);
        for(uint32_t i = 0; i < bmi.height; ++i) {
          memcpy(in, out, bytes_per_line);
          in += info.bytes_per_line;
          out += bmi.bytes_per_line;
        }
      }
    }

    template<pixel_format_t T>
    inline void datamap<T>::copy_from (const datamap& src_img,
                                       const core::native_rect& src_rect,
                                       const core::native_point& dest_pt) {
      bitmap_info src_bmi = src_img.get_info();

      auto src = checked_area(src_bmi, src_rect);
      auto dest = checked_area(info, dest_pt, src.size());

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::copy::sub<T>(src_img.get_data(), get_data(), src.position(), dest);
    }

    template<pixel_format_t T>
    inline void datamap<T>::crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
      datamap bmp(w, h);
      bmp.copy_from(*this, core::rectangle(x, y, w, h), core::point::zero);
      std::swap(data, bmp.data);
      std::swap(info, bmp.info);
    }

    template<pixel_format_t T>
    template<convert::interpolation I>
    inline void datamap<T>::stretch_from (const datamap& src) {
      stretch_from<I>(src, core::native_rect(src.native_size()), core::native_rect(native_size()));
    }

    template<pixel_format_t T>
    template<convert::interpolation I>
    inline void datamap<T>::stretch_from (const datamap& src_img,
                                          const core::native_rect& src_rect,
                                          const core::native_rect& dest_rect) {
      bitmap_info src_bmi = src_img.get_info();

      auto src = checked_area(src_bmi, src_rect);
      auto dest = checked_area(info, dest_rect);

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::stretch<T, I>::sub(src_img.get_data(), get_data(), src, dest);
    }

    template<pixel_format_t T>
    inline void datamap<T>::adjust_brightness (float f) {
      convert::brightness::adjust<T>(get_data(), info.width, info.height, f);
    }

    template<pixel_format_t T>
    inline auto datamap<T>::sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const -> datamap {
      datamap bmp(w, h);
      bmp.copy_from(*this, 
                    core::native_rect(x, y, w, h),
                    core::native_point::zero);
      return bmp;
    }

    template<pixel_format_t T>
    inline auto datamap<T>::brightness (double f) const -> datamap {
      datamap bmp = *this;
      bitmap_info& bmi = bmp.info;
      convert::brightness::adjust<T>(bmp.get_data(), bmi.width, bmi.height, f);
      return bmp;
    }

    template<pixel_format_t T>
    inline void datamap<T>::invert () {
      for (auto& c : data) {
        c = ~c;
      }
    }

    template<pixel_format_t T>
    inline void datamap<T>::fill (const pixel_type& c) {
      convert::fill::fill<T>(get_data(), info.width, info.height, c);
    }

    template<pixel_format_t T>
    inline datamap<pixel_format_t::BW> datamap<T>::get_mask (pixel::gray limit) const {
      datamap<pixel_format_t::BW> img(info.size());
      convert::format::mask<T, pixel_format_t::BW>(get_data(), img.get_data(), info.width, info.height, limit);
      return img;
    }

  }

}

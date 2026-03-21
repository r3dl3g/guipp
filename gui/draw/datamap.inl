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
        return datamap<S>(reinterpret<S>());
      } else {
        const auto w = bmi.width;
        const auto h = bmi.height;

        datamap<S> dest(w, h);
        switch (bmi.pixel_format) {
          case pixel_format_t::BW:   convert::format::convert<pixel_format_t::BW,   S>(reinterpret<pixel_format_t::BW>(),   dest.get_data(), w, h); break;
          case pixel_format_t::GRAY: convert::format::convert<pixel_format_t::GRAY, S>(reinterpret<pixel_format_t::GRAY>(), dest.get_data(), w, h); break;
          case pixel_format_t::RGB:  convert::format::convert<pixel_format_t::RGB,  S>(reinterpret<pixel_format_t::RGB>(),  dest.get_data(), w, h); break;
          case pixel_format_t::RGBA: convert::format::convert<pixel_format_t::RGBA, S>(reinterpret<pixel_format_t::RGBA>(), dest.get_data(), w, h); break;
          case pixel_format_t::BGR:  convert::format::convert<pixel_format_t::BGR,  S>(reinterpret<pixel_format_t::BGR>(),  dest.get_data(), w, h); break;
          case pixel_format_t::BGRA: convert::format::convert<pixel_format_t::BGRA, S>(reinterpret<pixel_format_t::BGRA>(), dest.get_data(), w, h); break;
          case pixel_format_t::ARGB: convert::format::convert<pixel_format_t::ARGB, S>(reinterpret<pixel_format_t::ARGB>(), dest.get_data(), w, h); break;
          case pixel_format_t::ABGR: convert::format::convert<pixel_format_t::ABGR, S>(reinterpret<pixel_format_t::ABGR>(), dest.get_data(), w, h); break;
          default: break;
        }
        return dest;
      }
    }

    template<pixel_format_t S>
    const image_data<S> basic_datamap::reinterpret () const {
      return image_data<S>(core::array_wrapper<byte>(const_cast<byte*>(data()), size()), info);
    }

    template<pixel_format_t S>
    image_data<S> basic_datamap::reinterpret () {
      return image_data<S>(core::array_wrapper<byte>(data(), size()), info);
    }
    // --------------------------------------------------------------------------

    template<pixel_format_t T>
    inline datamap<T>::datamap () {

    }

    template<pixel_format_t T>
    inline datamap<T>::datamap (datamap&& rhs)
      : super(std::move(rhs))
    {}

    template<pixel_format_t T>
    inline datamap<T>::datamap (const datamap& rhs)
      : super(rhs)
    {}

    template<pixel_format_t T>
    inline datamap<T>& datamap<T>::operator= (const datamap& rhs) {
      super::operator=(rhs);
      return *this;
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
    inline datamap<T>::datamap (const image_data<T>& data) {
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
    inline datamap<T>::datamap (const image_data<S>& src) {
      const bitmap_info& bmi = src.get_info();

      const auto w = bmi.width;
      const auto h = bmi.height;

      prepare({w, h, T});

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
      prepare({w, h, T});
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
    inline void datamap<T>::create (const image_data<T>& rhs) {
      const bitmap_info& bmi = rhs.get_info();
      
      prepare({bmi.width, bmi.height, T});
      
      const auto& info = get_info();
      const auto sz = info.mem_size();

      if (sz == bmi.mem_size()) {
        assign(rhs.raw_data().data(0, sz), sz);
      } else {
        auto data = access();
        byte* in = data.data(0, sz);

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
      auto dest = checked_area(get_info(), dest_pt, src.size());

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::copy::sub<T>(src_img.get_data(), get_data(), src.position(), dest);
    }

    template<pixel_format_t T>
    inline void datamap<T>::crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
      datamap bmp(w, h);
      bmp.copy_from(*this, core::rectangle(x, y, w, h), core::point::zero);
      swap(bmp);
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
      auto dest = checked_area(get_info(), dest_rect);

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::stretch<T, I>::sub(src_img.get_data(), get_data(), src, dest);
    }

    template<pixel_format_t T>
    inline void datamap<T>::adjust_brightness (float f) {
      convert::brightness::adjust<T>(get_data(), get_info().width, get_info().height, f);
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
      const bitmap_info& bmi = bmp.get_info();
      convert::brightness::adjust<T>(bmp.get_data(), bmi.width, bmi.height, f);
      return bmp;
    }

    template<pixel_format_t T>
    inline void datamap<T>::invert () {
      const auto sz = get_info().mem_size();
      auto data = access();
      byte* in = data.data(0, sz);
      const byte* end = in + sz;
      while (in < end) {
        *in = ~(*in);
        ++in;
      }
    }

    template<pixel_format_t T>
    inline void datamap<T>::fill (const pixel_type& c) {
      const bitmap_info& bmi = get_info();
      convert::fill::fill<T>(get_data(), bmi.width, bmi.height, c);
    }

    template<pixel_format_t T>
    inline datamap<pixel_format_t::BW> datamap<T>::get_mask (pixel::gray limit) const {
      const bitmap_info& bmi = get_info();
      datamap<pixel_format_t::BW> img(bmi.size());
      convert::format::mask<T, pixel_format_t::BW>(get_data(), img.get_data(), bmi.width, bmi.height, limit);
      if (!core::os::bitmap_bit_white) {
        img.invert();
      }
      return img;
    }

  }

}

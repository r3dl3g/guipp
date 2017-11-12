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
    inline datamap<T>::datamap (uint32_t w, uint32_t h, const blob& data) {
      create(data, {w, h, T});
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
      convert::bpp::convert<S, T>(src.get_data(), data, w, h, bmi.bytes_per_line, info.bytes_per_line);
    }

    template<BPP T>
    inline datamap<T>::datamap (const basic_datamap& src) {
      const bitmap_info& bmi = src.get_info();
      const auto w = bmi.width;
      const auto h = bmi.height;
      info = {w, h, T};

      data.resize(info.mem_size());
      switch (bmi.bits_per_pixel) {
        case BPP::BW:   convert::bpp::convert<BPP::BW, T>(src.get_data(), data, w, h, bmi.bytes_per_line, info.bytes_per_line); break;
        case BPP::GRAY: convert::bpp::convert<BPP::GRAY, T>(src.get_data(), data, w, h, bmi.bytes_per_line, info.bytes_per_line); break;
        case BPP::RGB:  convert::bpp::convert<BPP::RGB, T>(src.get_data(), data, w, h, bmi.bytes_per_line, info.bytes_per_line); break;
        case BPP::RGBA: convert::bpp::convert<BPP::RGBA, T>(src.get_data(), data, w, h, bmi.bytes_per_line, info.bytes_per_line); break;
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
    inline void datamap<T>::create (uint32_t w, uint32_t h, const blob& data) {
      info = {w, h, T};
      this->data.resize(info.mem_size());
      memcpy(this->data.data(), data.data(), std::min(this->data.size(), data.size()));
    }

    template<BPP T>
    inline void datamap<T>::copy_from (const datamap& src_img,
                                       const core::rectangle& src_rect,
                                       const core::point& dest_pt) {
      const blob &src_data = src_img.get_data();
      bitmap_info src_bmi = src_img.get_info();

      const uint32_t src_x0 = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.width, roundup<uint32_t>(src_rect.x())));
      const uint32_t src_y0 = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.height, roundup<uint32_t>(src_rect.y())));
      const uint32_t src_w = std::min<uint32_t>(src_bmi.width - src_x0, roundup<uint32_t>(src_rect.width()));
      const uint32_t src_h = std::min<uint32_t>(src_bmi.height - src_y0, roundup<uint32_t>(src_rect.height()));

      const uint32_t dest_x0 = std::max<uint32_t>(0, std::min<uint32_t>(info.width, roundup<uint32_t>(dest_pt.x())));
      const uint32_t dest_y0 = std::max<uint32_t>(0, std::min<uint32_t>(info.height, roundup<uint32_t>(dest_pt.y())));
      const uint32_t dest_w = std::min<uint32_t>(info.width - dest_x0, src_w);
      const uint32_t dest_h = std::min<uint32_t>(info.height - dest_y0, src_h);

      const uint32_t src_bpl = src_bmi.bytes_per_line;
      const uint32_t dest_bpl = info.bytes_per_line;

      if ((dest_w < 1) ||(dest_h < 1)) {
        return;
      }

      convert::copy::sub<T>(src_data, src_bpl, data, dest_bpl, src_x0, src_y0, dest_x0, dest_y0, dest_w, dest_h);
    }

    template<BPP T>
    inline void datamap<T>::crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const {
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
      const blob &src_data = src_img.get_data();
      bitmap_info src_bmi = src_img.get_info();

      const uint32_t src_x0 = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.width, roundup<uint32_t>(src_rect.x())));
      const uint32_t src_y0 = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.height, roundup<uint32_t>(src_rect.y())));
      const uint32_t src_w = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.width - src_x0, roundup<uint32_t>(src_rect.width())));
      const uint32_t src_h = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.height - src_y0, roundup<uint32_t>(src_rect.height())));

      const uint32_t dest_x0 = std::max<uint32_t>(0, std::min<uint32_t>(info.width, roundup<uint32_t>(dest_rect.x())));
      const uint32_t dest_y0 = std::max<uint32_t>(0, std::min<uint32_t>(info.height, roundup<uint32_t>(dest_rect.y())));
      const uint32_t dest_w = std::min<uint32_t>(info.width - dest_x0, roundup<uint32_t>(dest_rect.width()));
      const uint32_t dest_h = std::min<uint32_t>(info.height - dest_y0, roundup<uint32_t>(dest_rect.height()));

      if ((dest_w < 1) ||(dest_h < 1)) {
        return;
      }

      const uint32_t src_bpl = src_bmi.bytes_per_line;
      const uint32_t dest_bpl = info.bytes_per_line;

      convert::stretch::sub<BPP::BW>(src_data, src_bpl, data, dest_bpl, src_x0, src_y0, src_w, src_h, dest_x0, dest_y0, dest_w, dest_h);
    }

    template<BPP T>
    inline void datamap<T>::adjust_brightness (float f) {
      convert::brightness::adjust<T>(data, info.width, info.height, info.bytes_per_line, f);
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
      blob& data = bmp.data;
      bitmap_info& bmi = bmp.info;
      convert::brightness::adjust<T>(data, bmi.width, bmi.height, bmi.bytes_per_line, f);
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

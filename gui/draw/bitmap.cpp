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
 * @brief     monochrome and coloured fast bitmaps
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <limits>
#include <algorithm>
#include <map>
#include <util/ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/graphics.h"
#include "gui/draw/pen.h"
#include "gui/draw/use.h"


namespace gui {

  namespace native {

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data = nullptr);
    void free_bitmap (os::bitmap& id);
    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi);
    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi);
    draw::bitmap_info bitmap_get_info (const os::bitmap& id);
    void copy_bitmap (draw::basic_map& lhs, const draw::basic_map& rhs);

  } // namespace native

  namespace draw {

    // --------------------------------------------------------------------------
    basic_map& basic_map::operator= (basic_map&& rhs) {
      if (&rhs != this) {
        clear();
        if (rhs.is_valid()) {
          std::swap(id, rhs.id);
        }
      }
      return *this;
    }    

    basic_map& basic_map::operator= (const basic_map& rhs) {
      if (&rhs != this) {
        clear();
        if (rhs) {
          native::copy_bitmap(*this, rhs);
        }
      }
      return *this;
    }

    bool basic_map::is_valid () const {
#ifdef GUIPP_QT
      return id && !id->isNull();
#elif GUIPP_JS
      return !(id.isUndefined() || id.isNull());
#else
      return id != 0;
#endif
    }

    basic_map::operator const os::drawable () const {
#ifdef GUIPP_QT
      return id;
#else
      return get_os_bitmap();
#endif
    }

    void basic_map::clear () {
      if (is_valid()) {
        native::free_bitmap(get_os_bitmap());
        set_os_bitmap(IF_JS_ELSE(emscripten::val::undefined(), 0));
      }
    }

    bitmap_info basic_map::get_info () const {
      return native::bitmap_get_info(get_os_bitmap());
    }

    core::native_size basic_map::native_size () const {
      return get_info().size();
    }

    byte basic_map::depth () const {
      return get_info().depth();
    }

    pixel_format_t basic_map::pixel_format () const {
      return get_info().pixel_format;
    }

    void basic_map::create (const bitmap_info& rhs) {
      if (is_valid()) {
        bitmap_info bmi = get_info();
        if (bmi == rhs) {
          return;
        }
      }
      clear();
      set_os_bitmap(native::create_bitmap(rhs));
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    uint32_t basic_map::calc_bytes_per_line (uint32_t w, pixel_format_t px_fmt) {
      return draw::bitmap_info(w, 0, px_fmt).bytes_per_line;
    }

    // --------------------------------------------------------------------------
    void bitmap::create (uint32_t w, uint32_t h) {
      super::create({w, h, pixel_format_t::BW});
      if (!is_valid()) {
        throw std::runtime_error("create bitmap failed");
      }
    }

    void bitmap::create (const core::native_size& sz) {
      create(sz.width(), sz.height());
    }

    void bitmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    void bitmap::copy_from (const bitmap& src_img,
                            const core::native_rect& src_rect,
                            const core::native_point& dest_pt) {
      bwmap dest;
      dest.copy_from(src_img.get(), src_rect, dest_pt);
      operator=(dest);
    }

    void bitmap::invert () {
      bwmap data = get();
      data.invert();
      operator=(data);
    }

    void bitmap::operator= (const bwmap& rhs) {
      if (rhs) {
        const auto& bmi = rhs.get_info();
        create(bmi.size());
        if (core::os::bitmap_bit_white) {
          const auto raw = rhs.get_data();
          native::bitmap_put_data(get_os_bitmap(), raw.raw_data().data(0, bmi.mem_size()), bmi);
        } else {
          bwmap data = rhs;
          data.invert();
          const auto raw = data.get_data();
          native::bitmap_put_data(get_os_bitmap(), raw.raw_data().data(0, bmi.mem_size()), bmi);
        }
      } else {
        clear();
      }
    }

    bwmap bitmap::get () const {
      blob data;
      bitmap_info bmi;
      native::bitmap_get_data(get_os_bitmap(), data, bmi);
      bwmap bmp(std::move(data), std::move(bmi));
      if (!core::os::bitmap_bit_white) {
        bmp.invert();
      }
      return bmp;
    }

    bitmap::operator bwmap () const {
      return get();
    }

    // --------------------------------------------------------------------------

    void pixmap::create (uint32_t w, uint32_t h) {
#if GUIPP_WIN
      clear();
      HDC dc = GetDC(NULL);
      set_os_bitmap(CreateCompatibleBitmap(dc, w, h));
      ReleaseDC(NULL, dc);
#elif GUIPP_X11 || GUIPP_JS
      pixel_format_t px_fmt = core::global::get_device_pixel_format();
      super::create({w, h, px_fmt});
#elif GUIPP_QT
      set_os_bitmap(new  QPixmap(w, h));
#endif //GUIPP_QT
      if (!is_valid()) {
        throw std::runtime_error("create pixmap failed");
      }
    }

    void pixmap::create (const core::native_size& sz) {
      create(sz.width(), sz.height());
    }

    void pixmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    void pixmap::copy_from (const pixmap& src_img,
                            const core::native_rect& src_rect,
                            const core::native_point& dest_pt) {
      graphics g(*this);
      g.copy_from(src_img, src_rect, dest_pt);
    }

    void pixmap::invert () {
#ifdef GUIPP_QT
      QImage tmp = get_os_bitmap()->toImage();
      tmp.invertPixels();
      *(get_os_bitmap()) = QPixmap::fromImage(std::move(tmp));
#else
      switch (pixel_format()) {
        case pixel_format_t::BW:   invert<pixel_format_t::BW>();   break;
        case pixel_format_t::GRAY: invert<pixel_format_t::GRAY>(); break;
        case pixel_format_t::RGB:  invert<pixel_format_t::RGB>();  break;
        case pixel_format_t::RGBA: invert<pixel_format_t::RGBA>(); break;
        case pixel_format_t::ARGB: invert<pixel_format_t::ARGB>();  break;
        case pixel_format_t::BGR:  invert<pixel_format_t::BGR>();  break;
        case pixel_format_t::BGRA: invert<pixel_format_t::BGRA>();  break;
        case pixel_format_t::ABGR: invert<pixel_format_t::ABGR>();  break;
        default:  break;
      }
#endif //GUIPP_QT
    }

    void pixmap::put_raw (cbyteptr data, const draw::bitmap_info& bmi) {
      if (is_valid()) {
        native::bitmap_put_data(get_os_bitmap(), data, bmi);
      }
    }

    void pixmap::get_raw (blob& data, draw::bitmap_info& bmi) const {
      if (is_valid()) {
        native::bitmap_get_data(get_os_bitmap(), data, bmi);
      }
    }

    bwmap pixmap::get_mask (pixel::gray limit) const {
      return get<pixel_format_t::GRAY>().get_mask(limit);
    }

    byte pixmap::default_depth () {
      static byte depth =
#ifdef GUIPP_X11
          core::global::get_device_depth();
#elif GUIPP_QT
          static_cast<byte>(QPixmap(1, 1).depth());
#else
          core::global::get_device_depth();
#endif
      return depth;
    }

    pixel_format_t pixmap::default_pixel_format () {
      return core::global::get_device_pixel_format();
    }

    void pixmap::operator= (const pixmap& rhs) {
      super::operator=(rhs);
    }

    // --------------------------------------------------------------------------
    masked_bitmap::masked_bitmap (const pixmap& img, const bitmap& msk)
      : image(img)
      , mask(msk)
    {
      if (mask) {
        graphics g(image);
        Use<pen> use(g, color::white);
        g.copy_from(mask, core::native_rect(image.native_size()), core::native_point::zero, copy_mode::bit_and);
      }
    }

    masked_bitmap::masked_bitmap (pixmap&& img, bitmap&& msk)
      : image(std::move(img))
      , mask(std::move(msk))
    {
      if (mask) {
        graphics g(image);
        Use<pen> use(g, color::white);
        g.copy_from(mask, core::native_rect(image.native_size()), core::native_point::zero, copy_mode::bit_and);
      }
    }

    masked_bitmap& masked_bitmap::operator= (const masked_bitmap& rhs) {
      if (&rhs != this) {
        image = rhs.image;
        mask = rhs.mask;
      }
      return *this;
    }

    masked_bitmap& masked_bitmap::operator= (masked_bitmap&& rhs) {
      if (&rhs != this) {
        image = std::move(rhs.image);
        mask = std::move(rhs.mask);
      }
      return *this;
    }

    masked_bitmap& masked_bitmap::operator= (const pixmap& rhs) {
      image = rhs;
      if (image.is_valid()) {
        mask = image.get_mask({0x3f});
      }
      return *this;
    }

    masked_bitmap& masked_bitmap::operator= (pixmap&& rhs) {
      image = std::move(rhs);
      if (image.is_valid()) {
        mask = image.get_mask({0x3f});
      }
      return *this;
    }

    masked_bitmap& masked_bitmap::operator= (const bitmap& rhs) {
      mask = rhs;
      image.clear();
      return *this;
    }

    masked_bitmap& masked_bitmap::operator= (bitmap&& rhs) {
      mask = std::move(rhs);
      image.clear();
      return *this;
    }

    // --------------------------------------------------------------------------
  }

}

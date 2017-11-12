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


namespace gui {

  namespace draw {

    inline basic_map::basic_map ()
      : id(0)
    {}

    inline basic_map::basic_map (os::bitmap id)
      : id(id)
    {}

    inline basic_map::~basic_map () {
      clear();
    }

    inline basic_map::basic_map (const basic_map& rhs)
      : id(0) {
      operator= (rhs);
    }

    inline basic_map::basic_map (basic_map&& rhs)
      : id(0) {
      operator= (std::move(rhs));
    }

    inline void basic_map::set_id (os::bitmap rhs) {
      id = rhs;
    }

    inline bool basic_map::is_valid () const {
      return id != 0;
    }

    inline basic_map::operator bool () const {
      return is_valid();
    }

    inline os::bitmap basic_map::get_id () const {
      return id;
    }

    inline basic_map::operator os::drawable() const {
      return get_id();
    }

    inline void basic_map::create (const core::size& sz, BPP bpp) {
      create(sz.os_width(), sz.os_height(), bpp);
    }

    // --------------------------------------------------------------------------
    inline bitmap::bitmap ()
    {}

    inline bitmap::bitmap (const bwmap& rhs) {
      operator= (rhs);
    }

    inline bitmap::bitmap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    inline bitmap::bitmap (const core::size& sz) {
      create(sz);
    }

    inline void bitmap::operator= (const bwmap& rhs) {
      if (rhs) {
        create(rhs.size());
        put_data(rhs.get_data(), rhs.get_info());
      } else {
        clear();
      }
    }

    inline void bitmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    // --------------------------------------------------------------------------
    inline pixmap::pixmap ()
    {}

    inline pixmap::pixmap (const basic_datamap& rhs) {
      operator= (rhs);
    }

    inline pixmap::pixmap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    inline pixmap::pixmap (const core::size& sz) {
      create(sz);
    }

    inline void pixmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    template<BPP T>
    datamap<T> pixmap::get () const {
      if (bits_per_pixel() == T) {
        datamap<T> bmp(size());
        get_data(bmp.get_data(), bmp.get_info());
        return bmp;
      } else {
        switch (bits_per_pixel()) {
          case BPP::BW: {
            bwmap t(size());
            get_data(t.get_data(), t.get_info());
            return datamap<T>(t);
          }
          case BPP::GRAY: {
            graymap t(size());
            get_data(t.get_data(), t.get_info());
            return datamap<T>(t);
          }
          case BPP::RGB: {
            rgbmap t(size());
            get_data(t.get_data(), t.get_info());
            return datamap<T>(t);
          }
          case BPP::RGBA: {
            rgbamap t(size());
            get_data(t.get_data(), t.get_info());
            return datamap<T>(t);
          }
          default:
            return datamap<T>();
        }
      }
    }

    template<BPP T>
    pixmap::operator datamap<T> () const {
      return get<T>();
    }

    // --------------------------------------------------------------------------
    inline masked_bitmap::masked_bitmap ()
    {}

    inline masked_bitmap::masked_bitmap (const masked_bitmap& rhs)
      : image(rhs.image)
      , mask(rhs.mask)
    {}

    inline masked_bitmap::masked_bitmap (masked_bitmap&& rhs) {
      operator= (std::move(rhs));
    }

    inline masked_bitmap::masked_bitmap (const pixmap& rhs) {
      operator= (rhs);
    }

    inline masked_bitmap::masked_bitmap (pixmap&& rhs) {
      operator= (std::move(rhs));
    }

    inline masked_bitmap::masked_bitmap (const pixmap& image, const bitmap& mask)
      : image(image)
      , mask(mask)
    {}

    inline masked_bitmap::masked_bitmap (pixmap&& img, bitmap&& msk) {
      std::swap(image, img);
      std::swap(mask, msk);
    }

    inline core::size masked_bitmap::size () const {
      return image.size();
    }

    inline bool masked_bitmap::is_valid () const {
      return image.is_valid();
    }

    inline masked_bitmap::operator bool () const {
      return image.is_valid();
    }

  } // namespace draw

} // namespace gui

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

    // --------------------------------------------------------------------------
    inline bitmap::bitmap ()
    {}

    inline bitmap::bitmap (const bwmap& rhs) {
      operator= (rhs);
    }

    inline bitmap::bitmap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    inline bitmap::bitmap (const core::uint32_size& sz) {
      create(sz);
    }

    inline bitmap::bitmap (const core::size& sz) {
      create(sz);
    }

    inline void bitmap::create (const core::uint32_size& sz) {
      create(sz.width(), sz.height());
    }

    inline void bitmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    // --------------------------------------------------------------------------
    inline pixmap::pixmap ()
    {}

    template<PixelFormat T>
    inline pixmap::pixmap (const datamap<T>& rhs) {
      operator=(rhs);
    }

    template<PixelFormat T>
    inline pixmap::pixmap (const const_image_data<T>& rhs) {
      const auto& bmi = rhs.get_info();
      create(bmi.size());
      if (pixel_format() == bmi.pixel_format) {
        put(rhs.raw_data().data(0, bmi.mem_size()), bmi);
      } else {
        switch (bmi.pixel_format()) {
          case PixelFormat::BW:   put(bwmap(rhs)); break;
          case PixelFormat::GRAY: put(graymap(rhs)); break;
          case PixelFormat::RGB:  put(rgbmap(rhs));  break;
          case PixelFormat::RGBA: put(rgbamap(rhs)); break;
          case PixelFormat::BGR:  put(bgrmap(rhs));  break;
          case PixelFormat::BGRA: put(bgramap(rhs));  break;
          case PixelFormat::ARGB: put(argbmap(rhs));  break;
          case PixelFormat::ABGR: put(abgrmap(rhs));  break;
        }
      }
    }

    inline pixmap::pixmap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    inline pixmap::pixmap (const core::uint32_size& sz) {
      create(sz);
    }

    inline pixmap::pixmap (const core::size& sz) {
      create(sz);
    }

    inline void pixmap::create (const core::uint32_size& sz) {
      create(sz.width(), sz.height());
    }

    inline void pixmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    template<PixelFormat T>
    void pixmap::invert () {
      datamap<T> data = get<T>();
      data.invert();
      operator=(data);
    }

    template<PixelFormat T>
    void pixmap::put (const datamap<T>& rhs) {
      const auto& bmi = rhs.get_info();
      const auto raw = rhs.get_data();
      put(raw.raw_data().data(0, bmi.mem_size()), bmi);
    }

    template<PixelFormat T>
    void pixmap::operator= (const datamap<T>& rhs) {
      if (rhs) {
        const auto& bmi = rhs.get_info();
        create(bmi.size());
        if (pixel_format() == bmi.pixel_format) {
          put(rhs);
        } else {
          switch (pixel_format()) {
            case PixelFormat::BW:   put(bwmap(rhs)); break;
            case PixelFormat::GRAY: put(graymap(rhs)); break;
            case PixelFormat::RGB:  put(rgbmap(rhs));  break;
            case PixelFormat::RGBA: put(rgbamap(rhs)); break;
            case PixelFormat::BGR:  put(bgrmap(rhs));  break;
            case PixelFormat::BGRA: put(bgramap(rhs));  break;
            case PixelFormat::ARGB: put(argbmap(rhs));  break;
            case PixelFormat::ABGR: put(abgrmap(rhs));  break;
          }
        }
      } else {
        clear();
      }
    }

    template<PixelFormat T>
    datamap<T> pixmap::get () const {
      blob data;
      bitmap_info bmi;
      get(data, bmi);

      if (bmi.pixel_format == T) {
        return datamap<T>(std::move(data), std::move(bmi));
      } else {
        switch (bmi.pixel_format) {
          case PixelFormat::BW:   return datamap<PixelFormat::BW>(std::move(data), std::move(bmi)).convert<T>();
          case PixelFormat::GRAY: return datamap<PixelFormat::GRAY>(std::move(data), std::move(bmi)).convert<T>();
          case PixelFormat::RGB:  return datamap<PixelFormat::RGB>(std::move(data), std::move(bmi)).convert<T>();
          case PixelFormat::RGBA: return datamap<PixelFormat::RGBA>(std::move(data), std::move(bmi)).convert<T>();
          case PixelFormat::BGR:  return datamap<PixelFormat::BGR>(std::move(data), std::move(bmi)).convert<T>();
          case PixelFormat::BGRA: return datamap<PixelFormat::BGRA>(std::move(data), std::move(bmi)).convert<T>();
          case PixelFormat::ARGB: return datamap<PixelFormat::ARGB>(std::move(data), std::move(bmi)).convert<T>();
          case PixelFormat::ABGR: return datamap<PixelFormat::ABGR>(std::move(data), std::move(bmi)).convert<T>();
          default:                return datamap<T>();
        }
      }
    }

    template<PixelFormat T>
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

    inline masked_bitmap::masked_bitmap (const bitmap& rhs) {
      operator= (rhs);
    }

    inline masked_bitmap::masked_bitmap (bitmap&& rhs) {
      operator= (std::move(rhs));
    }

    inline core::size masked_bitmap::size () const {
      return mask.size();
    }

    inline bool masked_bitmap::is_valid () const {
      return mask.is_valid();
    }

    inline masked_bitmap::operator bool () const {
      return is_valid();
    }

  } // namespace draw

} // namespace gui

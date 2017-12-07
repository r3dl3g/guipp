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

    template<BPP T>
    inline pixmap::pixmap (const datamap<T>& rhs) {
      operator=(rhs);
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

    template<BPP T>
    void pixmap::invert () {
      datamap<T> data = get<T>();
      data.invert();
      operator=(data);
    }

    template<BPP T>
    void pixmap::put (const datamap<T>& rhs) {
      const auto& bmi = rhs.get_info();
      const auto raw = rhs.get_raw();
      put(raw.raw_data().data(0, bmi.mem_size()), bmi);
    }

    template<BPP T>
    void pixmap::operator= (const datamap<T>& rhs) {
      if (rhs) {
        const auto& bmi = rhs.get_info();
        create(bmi.size());
        if (bits_per_pixel() == rhs.bits_per_pixel()) {
          put(rhs);
        } else {
          switch (bits_per_pixel()) {
            case BPP::BW:   put(bwmap(rhs)); break;
            case BPP::GRAY: put(graymap(rhs)); break;
            case BPP::RGB:  put(rgbmap(rhs));  break;
            case BPP::RGBA: put(rgbamap(rhs)); break;
          }
        }
      } else {
        clear();
      }
    }

    template<BPP T>
    datamap<T> pixmap::get () const {
      blob data;
      bitmap_info bmi;
      get(data, bmi);
      const auto img = basepp::array_wrapper<const byte>(data);

      if (bmi.bits_per_pixel == T) {
        return datamap<T>(const_image_data<T>(img, bmi));
      } else {
        switch (bits_per_pixel()) {
          case BPP::BW:   return datamap<T>(bwmap(const_image_data<BPP::BW>(img, bmi)));
          case BPP::GRAY: return datamap<T>(graymap(const_image_data<BPP::GRAY>(img, bmi)));
          case BPP::RGB:  return datamap<T>(rgbmap(const_image_data<BPP::RGB>(img, bmi)));
          case BPP::RGBA: return datamap<T>(rgbamap(const_image_data<BPP::RGBA>(img, bmi)));
          default:        return datamap<T>();
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

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

#pragma once


namespace gui {

  namespace draw {

    inline basic_map::basic_map ()
      : id()
    {}

    inline basic_map::basic_map (os::bitmap id)
      : id(id)
    {}

    inline basic_map::~basic_map () {
      clear();
    }

    inline basic_map::basic_map (const basic_map& rhs)
      : id() {
      operator= (rhs);
    }

    inline basic_map::basic_map (basic_map&& rhs) noexcept
      : id() {
      operator= (std::move(rhs));
    }

    inline void basic_map::set_os_bitmap (os::bitmap rhs) {
      id = rhs;
    }

    inline basic_map::operator bool () const {
      return is_valid();
    }

    inline os::bitmap& basic_map::get_os_bitmap () {
      return id;
    }

    inline const os::bitmap& basic_map::get_os_bitmap () const {
      return id;
    }

    inline core::size basic_map::scaled_size () const {
      return get_info().scaled_size();
    }

    // --------------------------------------------------------------------------
    inline bitmap::bitmap (const bwmap& rhs) {
      operator= (rhs);
    }

    inline bitmap::bitmap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    inline bitmap::bitmap (const core::native_size& sz) {
      create(sz);
    }

    inline bitmap::bitmap (const core::size& sz) {
      create(sz);
    }

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline pixmap::pixmap (const datamap<T>& rhs) {
      operator=(rhs);
    }

    template<pixel_format_t T>
    inline pixmap::pixmap (const const_image_data<T>& rhs) {
      put(rhs);
    }

    inline pixmap::pixmap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    inline pixmap::pixmap (const core::native_size& sz) {
      create(sz);
    }

    inline pixmap::pixmap (const core::size& sz) {
      create(sz);
    }

    template<pixel_format_t T>
    void pixmap::invert () {
      datamap<T> data = get<T>();
      data.invert();
      operator=(data);
    }

    template<pixel_format_t T>
    void pixmap::put (const datamap<T>& rhs) {
      if (rhs) {
        put(rhs.get_data());
      } else {
        clear();
      }
    }

    template<pixel_format_t T>
    void pixmap::put (const const_image_data<T>& rhs) {
      const auto& bmi = rhs.get_info();
      create(bmi.size());
      if (pixel_format() == T) {
        put_raw(rhs.raw_data().data(0, bmi.mem_size()), bmi);
      } else {
        switch (pixel_format()) {
          case pixel_format_t::BW:   put(datamap<pixel_format_t::BW>  (rhs)); break;
          case pixel_format_t::GRAY: put(datamap<pixel_format_t::GRAY>(rhs)); break;
          case pixel_format_t::RGB:  put(datamap<pixel_format_t::RGB >(rhs)); break;
          case pixel_format_t::RGBA: put(datamap<pixel_format_t::RGBA>(rhs)); break;
          case pixel_format_t::BGR:  put(datamap<pixel_format_t::BGR >(rhs)); break;
          case pixel_format_t::BGRA: put(datamap<pixel_format_t::BGRA>(rhs)); break;
          case pixel_format_t::ARGB: put(datamap<pixel_format_t::ARGB>(rhs)); break;
          case pixel_format_t::ABGR: put(datamap<pixel_format_t::ABGR>(rhs)); break;
          default: break;
        }
      }
    }

    template<pixel_format_t T>
    pixmap& pixmap::operator= (const datamap<T>& rhs) {
      put<T>(rhs);
      return *this;
    }

    template<pixel_format_t T>
    pixmap& pixmap::operator= (const const_image_data<T>& rhs) {
      put<T>(rhs);
      return *this;
    }

    template<pixel_format_t T>
    datamap<T> pixmap::get () const {
      blob data;
      bitmap_info bmi;
      get_raw(data, bmi);

      if (bmi.pixel_format == T) {
        return datamap<T>(std::move(data), std::move(bmi));
      } else {
        switch (bmi.pixel_format) {
          case pixel_format_t::BW:   return datamap<pixel_format_t::BW>(std::move(data), std::move(bmi)).convert<T>();
          case pixel_format_t::GRAY: return datamap<pixel_format_t::GRAY>(std::move(data), std::move(bmi)).convert<T>();
          case pixel_format_t::RGB:  return datamap<pixel_format_t::RGB>(std::move(data), std::move(bmi)).convert<T>();
          case pixel_format_t::RGBA: return datamap<pixel_format_t::RGBA>(std::move(data), std::move(bmi)).convert<T>();
          case pixel_format_t::BGR:  return datamap<pixel_format_t::BGR>(std::move(data), std::move(bmi)).convert<T>();
          case pixel_format_t::BGRA: return datamap<pixel_format_t::BGRA>(std::move(data), std::move(bmi)).convert<T>();
          case pixel_format_t::ARGB: return datamap<pixel_format_t::ARGB>(std::move(data), std::move(bmi)).convert<T>();
          case pixel_format_t::ABGR: return datamap<pixel_format_t::ABGR>(std::move(data), std::move(bmi)).convert<T>();
          default:                return datamap<T>();
        }
      }
    }

    template<pixel_format_t T>
    pixmap::operator datamap<T> () const {
      return get<T>();
    }

    // --------------------------------------------------------------------------
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

    inline core::native_size masked_bitmap::native_size () const {
      return mask.native_size();
    }

    inline core::size masked_bitmap::scaled_size () const {
      return mask.scaled_size();
    }

    inline bool masked_bitmap::is_valid () const {
      return mask.is_valid() || image.is_valid();
    }

    inline masked_bitmap::operator bool () const {
      return is_valid();
    }

  } // namespace draw

} // namespace gui

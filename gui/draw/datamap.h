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
#include "gui/draw/converter.h"


namespace gui {

  namespace draw {

    template<pixel_format_t T>
    class datamap;

    class GUIPP_DRAW_EXPORT basic_datamap {
    public:
      bool is_valid () const;

      basic_datamap ();
      basic_datamap (const basic_datamap&);
      basic_datamap (basic_datamap&&);

      ~basic_datamap ();

      basic_datamap& operator= (const basic_datamap&);

      operator bool () const;

      const bitmap_info& get_info () const;
      bitmap_info& get_info ();

      template<pixel_format_t S>
      datamap<S> convert () const;

      core::native_size native_size () const;

      core::size scaled_size () const;

      byte depth () const;
      pixel_format_t pixel_format () const;

      void clear ();

      void swap (basic_datamap& rhs);

      template<pixel_format_t S>
      const image_data<S> reinterpret () const;

      template<pixel_format_t S>
      image_data<S> reinterpret ();

    protected:
      basic_datamap (const blob&, const bitmap_info&);
      basic_datamap (blob&&, bitmap_info&&);

      void prepare (const bitmap_info& info);
      void assign (const byte* data, std::size_t sz);

      core::array_wrapper<byte> access () ;

    private:
      friend class graphics;

      std::size_t size () const;
      byte* data ();
      const byte* data () const;

      bitmap_info info;
      blob buffer;
    };

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    class datamap : public basic_datamap {
    public:
      typedef basic_datamap super;
      using pixel_type = typename pixel::BPP2Pixel<T>::pixel;
      static constexpr pixel_format_t px_fmt = T;

      datamap ();
      datamap (datamap&&);
      datamap (const datamap&);

      datamap (uint32_t w, uint32_t h);
      explicit datamap (const core::native_size& sz);
      explicit datamap (const image_data<T>& data);
      explicit datamap (const core::size& sz);

      datamap (const blob& data, const bitmap_info& bmi);
      datamap (blob&& data, bitmap_info&& bmi);

      template<pixel_format_t S>
      explicit datamap (const datamap<S>& src);

      template<pixel_format_t S>
      explicit datamap (const image_data<S>& src);

      datamap& operator= (const datamap&);

      void create (uint32_t w, uint32_t h);
      void create (const core::size& sz);
      void create (const core::native_size& sz);
      void create (const image_data<T>& rhs);

      void copy_from (const datamap& src_img,
                      const core::native_rect& src_rect,
                      const core::native_point& dest_pt);

      const draw::image_data<T> get_data () const {
        return super::reinterpret<T>();
      }

      image_data<T> get_data () {
        return super::reinterpret<T>();
      }

      void crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h);

      template<convert::interpolation I = convert::interpolation::nearest>
      void stretch_from (const datamap& src);

      template<convert::interpolation I = convert::interpolation::nearest>
      void stretch_from (const datamap& src_img,
                         const core::native_rect& src_rect,
                         const core::native_rect& dest_rect);

      void adjust_brightness (float f);
      void invert ();

      void fill (const pixel_type&);

      datamap sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const;

      datamap brightness (double f) const;

      datamap<pixel_format_t::BW> get_mask (pixel::gray limit = {0x7F}) const;
    };

    // --------------------------------------------------------------------------
    typedef datamap<pixel_format_t::BW> bwmap;
    typedef datamap<pixel_format_t::GRAY> graymap;
    typedef datamap<pixel_format_t::RGB> rgbmap;
    typedef datamap<pixel_format_t::RGBA> rgbamap;
    typedef datamap<pixel_format_t::BGR> bgrmap;
    typedef datamap<pixel_format_t::BGRA> bgramap;
    typedef datamap<pixel_format_t::ARGB> argbmap;
    typedef datamap<pixel_format_t::ABGR> abgrmap;

    namespace sys {
      template<core::os::platform_t P = core::os::system_platform>
      struct default_datamap {
        using type = bgramap;
      };

      template<>
      struct default_datamap<core::os::platform_t::win32> {
        using type = rgbamap;
      };

      template<>
      struct default_datamap<core::os::platform_t::cocoa> {
        using type = rgbamap;
      };

      template<>
      struct default_datamap<core::os::platform_t::sdl> {
        using type = rgbamap;
      };

    }

    using sysmap = sys::default_datamap<>::type;

    GUIPP_DRAW_EXPORT core::native_rect checked_area (const bitmap_info& bmi, const core::native_rect& area);
    GUIPP_DRAW_EXPORT core::native_rect checked_area (const bitmap_info& bmi, const core::native_point& pt, const core::native_size& sz);

  } //namespace draw

} // namespace gui

#include "gui/draw/datamap.inl"

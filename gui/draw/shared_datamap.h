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
#include "gui/draw/datamap.h"


namespace gui {

  namespace draw {

#ifdef GUIPP_USE_XSHM
    class GUIPP_DRAW_EXPORT shared_datamap {
    public:
      typedef pixel::rgba pixel_type;
      typedef image_data<pixel_format_t::BGRA> image_data_type;
      static constexpr pixel_format_t px_fmt = pixel_format_t::BGRA;

      shared_datamap ();
      shared_datamap (const shared_datamap&);
      shared_datamap (shared_datamap&&);

      explicit shared_datamap (const core::native_size& sz);
      explicit shared_datamap (const core::size& sz);

      explicit shared_datamap (const image_data_type& src);
      
      ~shared_datamap ();
      
      shared_datamap& operator= (const shared_datamap&);
      shared_datamap& operator= (const image_data_type&);

      void create (const core::size& sz);
      void create (const core::native_size& sz);
      void create (const image_data_type& rhs);

      void copy_from (const image_data_type& src_img,
                      const core::native_rect& src_rect,
                      const core::native_point& dest_pt);

      bool is_valid () const;

      operator bool () const;

      const bitmap_info& get_info () const;
      bitmap_info& get_info ();

      const image_data_type get_data () const;
      image_data_type get_data ();

      core::native_size native_size () const;

      core::size scaled_size () const;

      byte depth () const;
      pixel_format_t pixel_format () const;

      void clear ();

      void swap (shared_datamap& rhs);
      
    private:
      friend class graphics;

      std::size_t size () const;
      byte* data ();
      const byte* data () const;

      void destroy();

      bitmap_info info;
      XShmSegmentInfo shminfo;
      XImage* image;
    };

#endif // GUIPP_USE_XSHM

  } //namespace draw

} // namespace gui

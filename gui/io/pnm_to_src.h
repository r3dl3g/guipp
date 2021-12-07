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
 * @brief     save image as pnm to cpp file or stream.
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <iomanip>
#include <fstream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/io/pnm.h>


namespace gui {

  namespace io {

    namespace src {

      // --------------------------------------------------------------------------
      GUIPP_IO_EXPORT void save_pnm_header_src (std::ostream& out, const std::string& name, PNM p, int w, int h, int max);
      GUIPP_IO_EXPORT void save_pnm_header_src (std::ostream& out, const std::string& name, const draw::bitmap_info&);

      // --------------------------------------------------------------------------
      GUIPP_IO_EXPORT void write_pixel (std::ostream& out, const core::bit_wrapper<const pixel::mono>& pixel);
      GUIPP_IO_EXPORT void write_pixel (std::ostream& out, pixel::mono pixel);
      GUIPP_IO_EXPORT void write_pixel (std::ostream& out, pixel::gray pixel);

      template<typename T, typename std::enable_if<pixel::is_rgb_type<T>::value>::type* = nullptr>
      void write_pixel (std::ostream& out, T pixel);

      template<pixel_format_t p>
      void save_pnm_src (std::ostream& out, const draw::const_image_data<p>& img);

      template<pixel_format_t T>
      void save_pnm_src (std::ostream& out, const draw::datamap<T>& bmp, const std::string& name);

      template<pixel_format_t T>
      void save_pnm_src (const std::string& fname, const draw::datamap<T>& bmp, const std::string& name);

      GUIPP_IO_EXPORT void save_pnm_src (const std::string& fname, const draw::basic_datamap& bmp, const std::string& name);
      GUIPP_IO_EXPORT void save_pnm_src (std::ostream& out, const draw::basic_datamap& bmp, const std::string& name);

      // --------------------------------------------------------------------------
      template<pixel_format_t T>
      class opnm {
      public:
        opnm (const draw::datamap<T>& bmp, const std::string& name);
        void write (std::ostream& out) const;

      private:
        const draw::datamap<T>& bmp;
        const std::string name;
      };

      // --------------------------------------------------------------------------
      template<pixel_format_t T>
      std::ostream& operator<< (std::ostream& out, const opnm<T>& p);

    } // namespace src

  } // namespace io

} // namespace gui

#include <gui/io/pnm_to_src.inl>

/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui io lib
 *
 * Customer   -
 *
 * @brief     C++ API: save bitmap as pnm to cpp file or stream.
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
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
      // --------------------------------------------------------------------------
      GUIPP_IO_EXPORT void save_pnm_header_src (std::ostream& out, const std::string& name, PNM p, int w, int h, int max);

      template<PixelFormat p>
      void save_pnm_src (std::ostream& out, const draw::const_image_data<p>& data);

      GUIPP_IO_EXPORT void write_pixel (std::ostream& out, pixel::bw_pixel pixel);
      GUIPP_IO_EXPORT void write_pixel (std::ostream& out, pixel::gray_pixel pixel);
      GUIPP_IO_EXPORT void write_pixel (std::ostream& out, pixel::rgb_pixel pixel);

      template<PixelFormat T>
      void save_pnm_src (std::ostream& out, const draw::datamap<T>& bmp, const std::string& name);

      template<PixelFormat T>
      void save_pnm_src (const std::string& filename, const draw::datamap<T>& bmp, const std::string& name);

      // --------------------------------------------------------------------------
      template<PixelFormat T>
      class opnm {
      public:
        opnm (const draw::datamap<T>& bmp, const std::string& name);
        void write (std::ostream& out) const;

      private:
        const draw::datamap<T>& bmp;
        const std::string name;
      };

      // --------------------------------------------------------------------------
      template<PixelFormat T>
      std::ostream& operator<< (std::ostream& out, const opnm<T>& p);

    } // namespace src

  } // namespace io

} // namespace gui

#include <gui/io/pnm_to_src.inl>

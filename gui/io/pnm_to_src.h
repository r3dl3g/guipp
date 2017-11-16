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
      void save_pnm_header_src (std::ostream& out, const std::string& name, PNM p, int w, int h, int max);

      template<BPP p>
      void save_pnm_src (std::ostream& out, const draw::const_image_data<p>& data);

      std::ostream& operator<< (std::ostream& out, draw::bw_pixel pixel);
      std::ostream& operator<< (std::ostream& out, draw::gray_pixel pixel);
      std::ostream& operator<< (std::ostream& out, draw::rgb_pixel pixel);

      template<BPP T>
      void save_pnm_src (std::ostream& out, const draw::datamap<T>& bmp, const std::string& name);

      template<BPP T>
      void save_pnm_src (const std::string& filename, const draw::datamap<T>& bmp, const std::string& name);

      // --------------------------------------------------------------------------
      template<BPP T>
      class opnm {
      public:
        opnm (const draw::datamap<T>& bmp, const std::string& name);
        void write (std::ostream& out) const;

      private:
        const draw::datamap<T>& bmp;
        const std::string name;
      };

      // --------------------------------------------------------------------------
      template<BPP T>
      std::ostream& operator<< (std::ostream& out, const opnm<T>& p);

      // --------------------------------------------------------------------------
      //
      // inlines
      //
      template<BPP T>
      inline opnm<T>::opnm (const draw::datamap<T>& bmp, const std::string& name)
        : bmp(bmp)
        , name(name)
      {}

      template<BPP T>
      inline std::ostream& operator<< (std::ostream& out, const opnm<T>& p) {
        p.write(out);
        return out;
      }

      // --------------------------------------------------------------------------
      template<BPP T>
      void opnm<T>::write (std::ostream& out) const {
        const auto& bmi = bmp.get_info();
        save_pnm_header_src(out, name, BPP2PNM<T, true>::pnm, bmi.width, bmi.height, BPP2MAX<T>::max);
        save_pnm_src<T>(out, bmp.get_raw());
      }
      // --------------------------------------------------------------------------
      template<BPP T>
      void save_pnm_src (std::ostream& out, const draw::const_image_data<T>& img) {
        auto& bmi = img.get_info();
        out.fill(' ');
        for (uint_fast32_t y = 0; y < bmi.height; ++y) {
          auto row = img.row(y);
          for (uint_fast32_t x = 0; x < bmi.width; ++x) {
            auto pixel = row[x];
            out << pixel;
            if ((y == bmi.height - 1) && (x == bmi.width - 1)) {
              out << "};";
            } else {
              out << ", ";
            }
          }
          out << std::endl;
        }
      }

      // --------------------------------------------------------------------------
      template<BPP T>
      void save_pnm_src (std::ostream& out, const draw::datamap<T>& bmp, const std::string& name) {
        out << opnm<T>(bmp, name);
      }

      template<BPP T>
      void save_pnm_src (const std::string& fname, const draw::datamap<T>& bmp, const std::string& name) {
        std::ofstream(fname) << opnm<T>(bmp, name);
      }

      // --------------------------------------------------------------------------
    } // src
  }

}

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


namespace gui {

  namespace io {

    namespace src {

      template<pixel_format_t T>
      inline opnm<T>::opnm (const draw::datamap<T>& bmp, const std::string& name)
        : bmp(bmp)
        , name(name)
      {}

      template<pixel_format_t T>
      inline std::ostream& operator<< (std::ostream& out, const opnm<T>& p) {
        p.write(out);
        return out;
      }

      // --------------------------------------------------------------------------
      template<pixel_format_t T>
      void opnm<T>::write (std::ostream& out) const {
        const auto& bmi = bmp.get_info();
        save_pnm_header_src(out, name, BPP2PNM<T, true>::pnm, bmi.width, bmi.height, BPP2MAX<T>::max);
        save_pnm_src<T>(out, bmp.get_data());
      }
      // --------------------------------------------------------------------------
      template<pixel_format_t T>
      void save_pnm_src (std::ostream& out, const draw::const_image_data<T>& img) {
        auto& bmi = img.get_info();
        out.fill(' ');
        for (uint_fast32_t y = 0; y < bmi.height; ++y) {
          const auto& row = img.row(y);
          for (uint_fast32_t x = 0; x < bmi.width; ++x) {
            const auto& pixel = row[x];
            write_pixel(out, pixel);
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
      template<pixel_format_t T>
      void save_pnm_src (std::ostream& out, const draw::datamap<T>& bmp, const std::string& name) {
        out << opnm<T>(bmp, name);
      }

      template<pixel_format_t T>
      void save_pnm_src (const std::string& fname, const draw::datamap<T>& bmp, const std::string& name) {
        std::ofstream(fname) << opnm<T>(bmp, name);
      }

    } // namespace src

  } // namespace io

} // namespace gui

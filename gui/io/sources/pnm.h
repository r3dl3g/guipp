/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "bitmap.h"


namespace gui {

  namespace io {

    void save_pnm_header (std::ostream& out, int magic_num, int w, int h, int max);
    void load_pnm_header (std::istream& in, int& magic_num, int& w, int& h, int& max);

    template<int i>
    void save_pnm (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp);

    // --------------------------------------------------------------------------
    template<int i>
    void load_pnm (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp);

    template<>
    void load_pnm<1> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp);
    template<>
    void load_pnm<2> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp);
    template<>
    void load_pnm<3> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp);
    template<>
    void load_pnm<4> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp);
    template<>
    void load_pnm<5> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp);
    template<>
    void load_pnm<6> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp);

    // --------------------------------------------------------------------------
    template<int i>
    struct pnm {
      static char const* suffix;
    };

    // --------------------------------------------------------------------------
    template<bool BIN>
    class opnm {
    public:
      opnm (const draw::bitmap& bmp)
        : bmp(bmp)
      {}

      void save (std::ostream& out) const {
        int w, h, bpl, bpp;
        std::vector<char> data;
        bmp.get_data(data, w, h, bpl, bpp);
        constexpr int offset = BIN ? 3 : 0;
        switch (bpp) {
          case 1:
            save_pnm_header(out, offset + 1, w, h, 0);
            save_pnm<offset + 1>(out, data, w, h, bpl, bpp);
            break;
          case 8:
            save_pnm_header(out, offset + 2, w, h, 255);
            save_pnm<offset + 2>(out, data, w, h, bpl, bpp);
            break;
          case 24:
          case 32:
            save_pnm_header(out, offset + 3, w, h, 255);
            save_pnm<offset + 3>(out, data, w, h, bpl, bpp);
            break;
          default:
            throw std::invalid_argument("unsupportet bit per pixel value");
        }
      }

    private:
      const draw::bitmap& bmp;
    };

    // --------------------------------------------------------------------------
    template<bool BIN>
    inline std::ostream& operator<< (std::ostream& out, const opnm<BIN>& p) {
      p.save(out);
      return out;
    }

    // --------------------------------------------------------------------------
    class ipnm {
    public:
      ipnm (draw::bitmap& bmp)
        : bmp(bmp)
      {}

      void read (std::istream& in) {
        int magic_num, w, h, max;
        load_pnm_header(in, magic_num, w, h, max);

        int bpl, bpp = 24;
        std::vector<char> data;
        switch (magic_num) {
          case 1:
            load_pnm<1>(in, data, w, h, bpl, bpp);
            break;
          case 2:
            load_pnm<2>(in, data, w, h, bpl, bpp);
            break;
          case 3:
            load_pnm<3>(in, data, w, h, bpl, bpp);
            break;
          case 4:
            load_pnm<4>(in, data, w, h, bpl, bpp);
            break;
          case 5:
            load_pnm<5>(in, data, w, h, bpl, bpp);
            break;
          case 6:
            load_pnm<6>(in, data, w, h, bpl, bpp);
            break;
        }
        bmp.create(data, w, h, bpl, bpp);
      }

    private:
      draw::bitmap& bmp;
    };

    // --------------------------------------------------------------------------
    inline std::istream &operator>> (std::istream& in, ipnm& p) {
      p.read(in);
      return in;
    }

    // --------------------------------------------------------------------------
  }

}

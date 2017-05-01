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
#include <fstream>

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

    template<>
    void save_pnm<1> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp);
    template<>
    void save_pnm<2> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp);
    template<>
    void save_pnm<3> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp);
    template<>
    void save_pnm<4> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp);
    template<>
    void save_pnm<5> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp);
    template<>
    void save_pnm<6> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp);

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
    struct pnm_const {
      static constexpr const char*const ppm = "ppm";
      static constexpr const char*const pgm = "pgm";
      static constexpr const char*const pbm = "pbm";

      static constexpr const char*const get_suffix (int i) {
        return ((i - 1) % 3 == 0 ? pbm : ((i - 1) % 3 == 1 ? pgm : ppm));
      }

      static constexpr std::ios::openmode get_open_mode (int i) {
        return (i > 3 ? std::ios::binary : std::ios::openmode(0));
      }

      template<typename T>
      static std::string build_filename(T t, int i) {
        std::string fname = ostreamfmt(t << "." << get_suffix(i));
        return fname;
      }

      static constexpr bool is_bin (int i) {
        return i > 3;
      }

    };

    // --------------------------------------------------------------------------
    template<int i>
    struct pnm : public pnm_const {
      static constexpr const char*const suffix = get_suffix(i);
      static const std::ios::openmode i_open_mode = std::ios::in | get_open_mode(i);
      static const std::ios::openmode o_open_mode = std::ios::out | get_open_mode(i);
      static const bool bin = is_bin(i);

      template<typename T>
      static std::string build_filename(T t) {
        std::string fname = ostreamfmt(t << "." << suffix);
        return fname;
      }

    };

    void save_pnm (std::ostream& out, const draw::bitmap& bmp, bool binary = true);
    void load_pnm (std::istream& in, draw::bitmap& bmp);

    // --------------------------------------------------------------------------
    template<bool BIN>
    class opnm {
    public:
      opnm (const draw::bitmap& bmp)
        : bmp(bmp)
      {}

      void write (std::ostream& out) const {
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
      p.write(out);
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
    template<int i>
    struct ofpnm : public std::ofstream, pnm<i> {
      typedef std::ofstream super;

      ofpnm()
        : super()
      {}

      ofpnm(const char* fname)
        : super(pnm<i>::build_filename(fname), pnm<i>::o_open_mode)
      {}

      ofpnm(const std::string& fname)
        : super(pnm<i>::build_filename(fname), pnm<i>::o_open_mode)
      {}

      void open(const char* fname) {
        super::open(pnm<i>::build_filename(fname), pnm<i>::o_open_mode);
      }

      void open(const std::string& fname) {
        super::open(pnm<i>::build_filename(fname), pnm<i>::o_open_mode);
      }

      void operator<< (const draw::bitmap& b) {
        opnm<pnm<i>::bin>(b).write(*this);
      }

    };

    // --------------------------------------------------------------------------
    template<int i>
    struct ifpnm : public std::ifstream, pnm<i> {
      typedef std::ifstream super;

      ifpnm()
        : super()
      {}

      ifpnm(const char* fname)
        : super(pnm<i>::build_filename(fname), pnm<i>::i_open_mode)
      {}

      ifpnm(const std::string& fname)
        : super(pnm<i>::build_filename(fname), pnm<i>::i_open_mode)
      {}

      void open(const char* fname) {
        super::open(pnm<i>::build_filename(fname), pnm<i>::i_open_mode);
      }

      void open(const std::string& fname) {
        super::open(pnm<i>::build_filename(fname), pnm<i>::i_open_mode);
      }

      void operator>> (draw::bitmap& b) {
        ipnm(b).read(*this);
      }
    };

    // --------------------------------------------------------------------------
  }

}

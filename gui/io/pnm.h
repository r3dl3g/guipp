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
 * @brief     C++ API: save/load bitmap to/from pnm file or stream.
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
#include <gui/core/ostreamfmt.h>
#include <gui/draw/bitmap.h>


namespace gui {

  namespace io {

    // --------------------------------------------------------------------------
    enum class PNM : int {
      P1 = 1,
      P2 = 2,
      P3 = 3,
      P4 = 4,
      P5 = 5,
      P6 = 6
    };

    std::ostream& operator<< (std::ostream&, const PNM&);
    std::istream& operator>> (std::istream&, PNM&);

    // --------------------------------------------------------------------------
    void save_pnm_header (std::ostream& out, PNM magic_num, const draw::bitmap_info&, int max);
    void load_pnm_header (std::istream& in, PNM& magic_num, draw::bitmap_info&, int& max);

    template<PNM i>
    void save_pnm (std::ostream& out, const blob& data, const draw::bitmap_info& bmi);

    template<> void save_pnm<PNM::P1>(std::ostream&, const blob&, const draw::bitmap_info&);
    template<> void save_pnm<PNM::P2>(std::ostream&, const blob&, const draw::bitmap_info&);
    template<> void save_pnm<PNM::P3>(std::ostream&, const blob&, const draw::bitmap_info&);
    template<> void save_pnm<PNM::P4>(std::ostream&, const blob&, const draw::bitmap_info&);
    template<> void save_pnm<PNM::P5>(std::ostream&, const blob&, const draw::bitmap_info&);
    template<> void save_pnm<PNM::P6>(std::ostream&, const blob&, const draw::bitmap_info&);

    // --------------------------------------------------------------------------
    template<PNM i>
    void load_pnm (std::istream& in, blob& data, draw::bitmap_info&);

    template<> void load_pnm<PNM::P1>(std::istream&, blob&, draw::bitmap_info&);
    template<> void load_pnm<PNM::P2>(std::istream&, blob&, draw::bitmap_info&);
    template<> void load_pnm<PNM::P3>(std::istream&, blob&, draw::bitmap_info&);
    template<> void load_pnm<PNM::P4>(std::istream&, blob&, draw::bitmap_info&);
    template<> void load_pnm<PNM::P5>(std::istream&, blob&, draw::bitmap_info&);
    template<> void load_pnm<PNM::P6>(std::istream&, blob&, draw::bitmap_info&);

    // --------------------------------------------------------------------------
    struct pnm_const {
      static constexpr const char*const ppm = "ppm";
      static constexpr const char*const pgm = "pgm";
      static constexpr const char*const pbm = "pbm";

      static constexpr const char*const get_suffix (PNM i) {
        return ((static_cast<int>(i) - 1) % 3 == 0 ? pbm : ((static_cast<int>(i) - 1) % 3 == 1 ? pgm : ppm));
      }

      static constexpr std::ios::openmode get_open_mode (PNM i) {
        return (static_cast<int>(i) > 3 ? std::ios::binary : std::ios::openmode(0));
      }

      template<typename T>
      static std::string build_filename (T t, PNM i) {
        std::string fname = ostreamfmt(t << "." << get_suffix(i));
        return fname;
      }

      static constexpr bool is_bin (PNM i) {
        return static_cast<int>(i) > 3;
      }

    };

    // --------------------------------------------------------------------------
    template<PNM i>
    struct pnm : public pnm_const {
      static constexpr const char*const suffix = get_suffix(i);
      static const std::ios::openmode i_open_mode = std::ios::in | get_open_mode(i);
      static const std::ios::openmode o_open_mode = std::ios::out | get_open_mode(i);
      static const bool bin = is_bin(i);

      template<typename T>
      static std::string build_filename (T t) {
        std::string fname = ostreamfmt(t << "." << suffix);
        return fname;
      }

    };

    void save_pnm (std::ostream& out, const draw::bitmap& bmp, bool binary = true);
    void load_pnm (std::istream& in, draw::bitmap& bmp);
    void save_pnm (const std::string& name, const draw::bitmap& bmp, bool binary = true);
    void load_pnm (const std::string& name, draw::bitmap& bmp);

    template<PNM>
    struct PNM2BPP {};

    template<> struct PNM2BPP<PNM::P1> { static constexpr BPP bpp = BPP::BW;    static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P2> { static constexpr BPP bpp = BPP::GRAY;  static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P3> { static constexpr BPP bpp = BPP::RGB;   static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P4> { static constexpr BPP bpp = BPP::BW;    static constexpr bool bin = true;   };
    template<> struct PNM2BPP<PNM::P5> { static constexpr BPP bpp = BPP::GRAY;  static constexpr bool bin = true;   };
    template<> struct PNM2BPP<PNM::P6> { static constexpr BPP bpp = BPP::RGB;   static constexpr bool bin = true;   };

    template<BPP, bool>
    struct BPP2PNM {};

    template<> struct BPP2PNM<BPP::BW, false>   {static constexpr PNM pnm = PNM::P1; };
    template<> struct BPP2PNM<BPP::GRAY, false> {static constexpr PNM pnm = PNM::P2; };
    template<> struct BPP2PNM<BPP::RGB, false>  {static constexpr PNM pnm = PNM::P3; };
    template<> struct BPP2PNM<BPP::RGBA, false> {static constexpr PNM pnm = PNM::P3; };

    template<> struct BPP2PNM<BPP::BW, true>    {static constexpr PNM pnm = PNM::P4; };
    template<> struct BPP2PNM<BPP::GRAY, true>  {static constexpr PNM pnm = PNM::P5; };
    template<> struct BPP2PNM<BPP::RGB, true>   {static constexpr PNM pnm = PNM::P6; };
    template<> struct BPP2PNM<BPP::RGBA, true>  {static constexpr PNM pnm = PNM::P6; };

    // --------------------------------------------------------------------------
    template<bool BIN>
    class opnm {
    public:
      opnm (const draw::bitmap& bmp);
      void write (std::ostream& out) const;

    private:
      const draw::bitmap& bmp;
    };

    // --------------------------------------------------------------------------
    template<bool BIN>
    std::ostream& operator<< (std::ostream& out, const opnm<BIN>& p);

    // --------------------------------------------------------------------------
    class ipnm {
    public:
      ipnm (draw::bitmap& bmp);
      ipnm (draw::bitmap&& bmp);
      void read (std::istream& in);

    private:
      draw::bitmap& bmp;
    };

    // --------------------------------------------------------------------------
    std::istream &operator>> (std::istream& in, ipnm& p);
    std::istream &operator>> (std::istream& in, ipnm&& p);

    // --------------------------------------------------------------------------
    template<PNM i>
    struct ofpnm : public pnm<i> {
      typedef pnm<i> super;

      ofpnm ();
      ofpnm (const char* fname);
      ofpnm (const std::string& fname);

      void open (const char* fname);
      void open (const std::string& fname);

      void operator<< (const draw::memmap& b);
      void operator<< (const draw::datamap<PNM2BPP<i>::bpp>& b);

    private:
      std::ofstream out;
    };

    // --------------------------------------------------------------------------
    template<PNM i>
    struct ifpnm : public pnm<i> {
      typedef pnm<i> super;

      ifpnm ();
      ifpnm (const char* fname);
      ifpnm (const std::string& fname);

      void open (const char* fname);
      void open (const std::string& fname);

      void operator>> (draw::memmap& b);
      void operator>> (draw::datamap<PNM2BPP<i>::bpp>& b);

    private:
      std::ifstream in;
    };

    // --------------------------------------------------------------------------
    // inlines
    template<bool BIN>
    inline opnm<BIN>::opnm (const draw::bitmap& bmp)
      : bmp(bmp)
    {}

    template<bool BIN>
    inline void opnm<BIN>::write (std::ostream& out) const {
      draw::bitmap_info bmi;
      blob data;
      bmp.get_data(data, bmi);
      switch (bmi.bits_per_pixel) {
      case BPP::BW:
        save_pnm_header(out, BPP2PNM<BPP::BW, BIN>::pnm, bmi, 0);
        save_pnm<BPP2PNM<BPP::BW, BIN>::pnm>(out, data, bmi);
        break;
      case BPP::GRAY:
        save_pnm_header(out, BPP2PNM<BPP::GRAY, BIN>::pnm, bmi, 255);
        save_pnm<BPP2PNM<BPP::GRAY, BIN>::pnm>(out, data, bmi);
        break;
      case BPP::RGB:
        save_pnm_header(out, BPP2PNM<BPP::RGB, BIN>::pnm, bmi, 255);
        save_pnm<BPP2PNM<BPP::RGB, BIN>::pnm>(out, data, bmi);
        break;
      case BPP::RGBA:
        save_pnm_header(out, BPP2PNM<BPP::RGBA, BIN>::pnm, bmi, 255);
        save_pnm<BPP2PNM<BPP::RGBA, BIN>::pnm>(out, data, bmi);
        break;
      default:
        throw std::invalid_argument("unsupportet bit per pixel value");
      }
    }

    // --------------------------------------------------------------------------
    template<bool BIN>
    inline std::ostream& operator<< (std::ostream& out, const opnm<BIN>& p) {
      p.write(out);
      return out;
    }

    // --------------------------------------------------------------------------
    inline ipnm::ipnm (draw::bitmap& bmp)
      : bmp(bmp)
    {}

    // --------------------------------------------------------------------------
    inline ipnm::ipnm (draw::bitmap&& bmp)
      : bmp(bmp)
    {}

    // --------------------------------------------------------------------------
    inline void ipnm::read (std::istream& in) {
      draw::bitmap_info bmi;
      int max;
      PNM pnm;
      load_pnm_header(in, pnm, bmi, max);

      blob data;
      switch (pnm) {
      case PNM::P1:
        load_pnm<PNM::P1>(in, data, bmi);
        break;
      case PNM::P2:
        load_pnm<PNM::P2>(in, data, bmi);
        break;
      case PNM::P3:
        load_pnm<PNM::P3>(in, data, bmi);
        break;
      case PNM::P4:
        load_pnm<PNM::P4>(in, data, bmi);
        break;
      case PNM::P5:
        load_pnm<PNM::P5>(in, data, bmi);
        break;
      case PNM::P6:
        load_pnm<PNM::P6>(in, data, bmi);
        break;
      }
      bmp.create(data, bmi);
    }

    // --------------------------------------------------------------------------
    inline std::istream &operator>> (std::istream& in, ipnm& p) {
      p.read(in);
      return in;
    }

    inline std::istream &operator>> (std::istream& in, ipnm&& p) {
      p.read(in);
      return in;
    }

    // --------------------------------------------------------------------------
    template<PNM i>
    inline ofpnm<i>::ofpnm ()
    {}

    template<PNM i>
    inline ofpnm<i>::ofpnm (const char* fname)
      : out(super::build_filename(fname), super::o_open_mode)
    {}

    template<PNM i>
    inline ofpnm<i>::ofpnm (const std::string& fname)
      : out(super::build_filename(fname), super::o_open_mode)
    {}

    template<PNM i>
    inline void ofpnm<i>::open (const char* fname) {
      out.open(super::build_filename(fname), super::o_open_mode);
    }

    template<PNM i>
    inline void ofpnm<i>::open (const std::string& fname) {
      out.open(super::build_filename(fname), super::o_open_mode);
    }

    template<PNM i>
    inline void ofpnm<i>::operator<< (const draw::memmap& b) {
      opnm<super::bin>(b).write(out);
    }

    template<PNM i>
    inline void ofpnm<i>::operator<< (const draw::datamap<PNM2BPP<i>::bpp>& b) {
      opnm<super::bin>(b).write(out);
    }

    // --------------------------------------------------------------------------
    template<PNM i>
    inline ifpnm<i>::ifpnm ()
      : super()
    {}

    template<PNM i>
    inline ifpnm<i>::ifpnm (const char* fname)
      : in(super::build_filename(fname), super::i_open_mode)
    {}

    template<PNM i>
    inline ifpnm<i>::ifpnm (const std::string& fname)
      : in(super::build_filename(fname), super::i_open_mode)
    {}

    template<PNM i>
    inline void ifpnm<i>::open (const char* fname) {
      in.open(super::build_filename(fname), super::i_open_mode);
    }

    template<PNM i>
    inline void ifpnm<i>::open (const std::string& fname) {
      in.open(super::build_filename(fname), super::i_open_mode);
    }

    template<PNM i>
    inline void ifpnm<i>::operator>> (draw::memmap& b) {
      in >> ipnm(b);
    }

    template<PNM i>
    inline void ifpnm<i>::operator>> (draw::datamap<PNM2BPP<i>::bpp>& b) {
      in >> ipnm(b);
    }

    // --------------------------------------------------------------------------
  }

}

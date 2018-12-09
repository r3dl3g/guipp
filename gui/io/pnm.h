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
#include <base/ostreamfmt.h>
#include <gui/draw/bitmap.h>
#include <gui++-io-export.h>


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

    GUIPP_IO_EXPORT std::ostream& operator<< (std::ostream&, const PNM&);
    GUIPP_IO_EXPORT std::istream& operator>> (std::istream&, PNM&);

    // --------------------------------------------------------------------------
    template<PNM>
    struct PNM2BPP {};

    template<> struct PNM2BPP<PNM::P1> { static constexpr PixelFormat px_fmt = PixelFormat::BW;    static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P2> { static constexpr PixelFormat px_fmt = PixelFormat::GRAY;  static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P3> { static constexpr PixelFormat px_fmt = PixelFormat::RGB;   static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P4> { static constexpr PixelFormat px_fmt = PixelFormat::BW;    static constexpr bool bin = true;   };
    template<> struct PNM2BPP<PNM::P5> { static constexpr PixelFormat px_fmt = PixelFormat::GRAY;  static constexpr bool bin = true;   };
    template<> struct PNM2BPP<PNM::P6> { static constexpr PixelFormat px_fmt = PixelFormat::RGB;   static constexpr bool bin = true;   };

    inline PixelFormat pnm2bpp (PNM pnm) {
      switch (pnm) {
        case PNM::P1:
        case PNM::P4:
          return PixelFormat::BW;
        case PNM::P2:
        case PNM::P5:
          return PixelFormat::GRAY;
        case PNM::P3:
        case PNM::P6:
          return PixelFormat::RGB;
      }
      return PixelFormat::Undefined;
    }

    template<PixelFormat, bool>
    struct BPP2PNM {};

    template<> struct BPP2PNM<PixelFormat::BW, false>   {static constexpr PNM pnm = PNM::P1; };
    template<> struct BPP2PNM<PixelFormat::GRAY, false> {static constexpr PNM pnm = PNM::P2; };
    template<> struct BPP2PNM<PixelFormat::RGB, false>  {static constexpr PNM pnm = PNM::P3; };
    template<> struct BPP2PNM<PixelFormat::RGBA, false> {static constexpr PNM pnm = PNM::P3; };

    template<> struct BPP2PNM<PixelFormat::BW, true>    {static constexpr PNM pnm = PNM::P4; };
    template<> struct BPP2PNM<PixelFormat::GRAY, true>  {static constexpr PNM pnm = PNM::P5; };
    template<> struct BPP2PNM<PixelFormat::RGB, true>   {static constexpr PNM pnm = PNM::P6; };
    template<> struct BPP2PNM<PixelFormat::RGBA, true>  {static constexpr PNM pnm = PNM::P6; };

    template<PixelFormat>
    struct BPP2MAX {};

    template<> struct BPP2MAX<PixelFormat::BW> { static constexpr int max = 0; };
    template<> struct BPP2MAX<PixelFormat::GRAY> { static constexpr int max = 255; };
    template<> struct BPP2MAX<PixelFormat::RGB> { static constexpr int max = 255; };
    template<> struct BPP2MAX<PixelFormat::RGBA> { static constexpr int max = 255; };

    // --------------------------------------------------------------------------
    GUIPP_IO_EXPORT void write_pnm_header (std::ostream& out, PNM magic_num, const draw::bitmap_info&, int max);
    GUIPP_IO_EXPORT draw::bitmap_info read_pnm_header (std::istream& in, PNM& magic_num, int& max);

    template<PNM i>
    void write_pnm (std::ostream& out, const draw::const_image_data<PNM2BPP<i>::px_fmt>&);

    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P1> (std::ostream&, const draw::const_image_data<PixelFormat::BW>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P2> (std::ostream&, const draw::const_image_data<PixelFormat::GRAY>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P3> (std::ostream&, const draw::const_image_data<PixelFormat::RGB>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P4> (std::ostream&, const draw::const_image_data<PixelFormat::BW>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P5> (std::ostream&, const draw::const_image_data<PixelFormat::GRAY>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P6> (std::ostream&, const draw::const_image_data<PixelFormat::RGB>&);

    template<bool BIN>
    void write_pnm_rgba (std::ostream&, const draw::const_image_data<PixelFormat::RGBA>&);

    template<> GUIPP_IO_EXPORT void write_pnm_rgba<false> (std::ostream&, const draw::const_image_data<PixelFormat::RGBA>&);
    template<> GUIPP_IO_EXPORT void write_pnm_rgba<true> (std::ostream&, const draw::const_image_data<PixelFormat::RGBA>&);

    // --------------------------------------------------------------------------
    template<PNM i>
    draw::datamap<PNM2BPP<i>::px_fmt> read_pnm (std::istream& in, const draw::bitmap_info&);

    template<> GUIPP_IO_EXPORT draw::datamap<PixelFormat::BW>   read_pnm<PNM::P1> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<PixelFormat::GRAY> read_pnm<PNM::P2> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<PixelFormat::RGB>  read_pnm<PNM::P3> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<PixelFormat::BW>   read_pnm<PNM::P4> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<PixelFormat::GRAY> read_pnm<PNM::P5> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<PixelFormat::RGB>  read_pnm<PNM::P6> (std::istream&, const draw::bitmap_info& bmi);

    // --------------------------------------------------------------------------
    struct pnm_const {
      static constexpr const char*const ppm = "ppm";
      static constexpr const char*const pgm = "pgm";
      static constexpr const char*const pbm = "pbm";

      static constexpr const char* get_suffix (PNM i) {
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

    template<PixelFormat T>
    void save_pnm (std::ostream& out, const draw::datamap<T>& bmp, bool binary = true);

    template<PixelFormat T>
    void save_pnm (const std::string& name, const draw::datamap<T>& bmp, bool binary = true);

    template<PixelFormat T>
    void load_pnm (std::istream& in, draw::datamap<T>& bmp);

    template<PixelFormat T>
    void load_pnm (const std::string& name, draw::datamap<T>& bmp);

    GUIPP_IO_EXPORT void load_pnm (std::istream& in, draw::basic_datamap& bmp);
    GUIPP_IO_EXPORT void load_pnm (const std::string& name, draw::basic_datamap& bmp);

    // --------------------------------------------------------------------------
    template<bool BIN, PixelFormat T>
    class opnm {
    public:
      opnm (const draw::datamap<T>& bmp);
      void write (std::ostream& out) const;

    private:
      const draw::datamap<T>& bmp;
    };

    // --------------------------------------------------------------------------
    template<bool BIN, PixelFormat T>
    std::ostream& operator<< (std::ostream& out, const opnm<BIN, T>& p);

    // --------------------------------------------------------------------------
    template<PixelFormat T>
    class ipnm {
    public:
      ipnm (draw::datamap<T>& bmp);
      ipnm (draw::datamap<T>&& bmp);
      void read (std::istream& in);

    private:
      draw::datamap<T>& bmp;
    };

    // --------------------------------------------------------------------------
    template<PixelFormat T>
    std::istream &operator>> (std::istream& in, ipnm<T>& p);

    template<PixelFormat T>
    std::istream &operator>> (std::istream& in, ipnm<T>&& p);

    // --------------------------------------------------------------------------
    template<PNM i>
    struct ofpnm : public pnm<i> {
      typedef pnm<i> super;

      ofpnm ();
      ofpnm (const char* fname);
      ofpnm (const std::string& fname);

      void open (const char* fname);
      void open (const std::string& fname);

      void operator<< (const draw::pixmap& b);
      void operator<< (const draw::datamap<PNM2BPP<i>::px_fmt>& b);

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

      void operator>> (draw::pixmap& b);
      void operator>> (draw::datamap<PNM2BPP<i>::px_fmt>& b);

    private:
      std::ifstream in;
    };

  } // namespace io

} // namespace gui

#include <gui/io/pnm.inl>

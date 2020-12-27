/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <util/ostreamfmt.h>
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

    template<> struct PNM2BPP<PNM::P1> { static constexpr pixel_format_t px_fmt = pixel_format_t::BW;    static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P2> { static constexpr pixel_format_t px_fmt = pixel_format_t::GRAY;  static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P3> { static constexpr pixel_format_t px_fmt = pixel_format_t::RGB;   static constexpr bool bin = false;  };
    template<> struct PNM2BPP<PNM::P4> { static constexpr pixel_format_t px_fmt = pixel_format_t::BW;    static constexpr bool bin = true;   };
    template<> struct PNM2BPP<PNM::P5> { static constexpr pixel_format_t px_fmt = pixel_format_t::GRAY;  static constexpr bool bin = true;   };
    template<> struct PNM2BPP<PNM::P6> { static constexpr pixel_format_t px_fmt = pixel_format_t::RGB;   static constexpr bool bin = true;   };

    inline pixel_format_t pnm2bpp (PNM pnm) {
      switch (pnm) {
        case PNM::P1:
        case PNM::P4:
          return pixel_format_t::BW;
        case PNM::P2:
        case PNM::P5:
          return pixel_format_t::GRAY;
        case PNM::P3:
        case PNM::P6:
          return pixel_format_t::RGB;
      }
      return pixel_format_t::Undefined;
    }

    template<pixel_format_t, bool>
    struct BPP2PNM {};

    template<> struct BPP2PNM<pixel_format_t::BW, false>   {static constexpr PNM pnm = PNM::P1; };
    template<> struct BPP2PNM<pixel_format_t::GRAY, false> {static constexpr PNM pnm = PNM::P2; };
    template<> struct BPP2PNM<pixel_format_t::RGB, false>  {static constexpr PNM pnm = PNM::P3; };
    template<> struct BPP2PNM<pixel_format_t::BGR, false>  {static constexpr PNM pnm = PNM::P3; };
    template<> struct BPP2PNM<pixel_format_t::RGBA, false> {static constexpr PNM pnm = PNM::P3; };
    template<> struct BPP2PNM<pixel_format_t::BGRA, false> {static constexpr PNM pnm = PNM::P3; };

    template<> struct BPP2PNM<pixel_format_t::BW, true>    {static constexpr PNM pnm = PNM::P4; };
    template<> struct BPP2PNM<pixel_format_t::GRAY, true>  {static constexpr PNM pnm = PNM::P5; };
    template<> struct BPP2PNM<pixel_format_t::RGB, true>   {static constexpr PNM pnm = PNM::P6; };
    template<> struct BPP2PNM<pixel_format_t::BGR, true>   {static constexpr PNM pnm = PNM::P6; };
    template<> struct BPP2PNM<pixel_format_t::RGBA, true>  {static constexpr PNM pnm = PNM::P6; };
    template<> struct BPP2PNM<pixel_format_t::BGRA, true>  {static constexpr PNM pnm = PNM::P6; };

    template<pixel_format_t>
    struct BPP2MAX {};

    template<> struct BPP2MAX<pixel_format_t::BW> { static constexpr int max = 0; };
    template<> struct BPP2MAX<pixel_format_t::GRAY> { static constexpr int max = 255; };
    template<> struct BPP2MAX<pixel_format_t::RGB> { static constexpr int max = 255; };
    template<> struct BPP2MAX<pixel_format_t::BGR> { static constexpr int max = 255; };
    template<> struct BPP2MAX<pixel_format_t::RGBA> { static constexpr int max = 255; };
    template<> struct BPP2MAX<pixel_format_t::BGRA> { static constexpr int max = 255; };

    // --------------------------------------------------------------------------
    GUIPP_IO_EXPORT void write_pnm_header (std::ostream& out, PNM magic_num, const draw::bitmap_info&, int max);
    GUIPP_IO_EXPORT draw::bitmap_info read_pnm_header (std::istream& in, PNM& magic_num, int& max);

    template<PNM i, pixel_format_t fmt>
    void write_pnm (std::ostream& out, const draw::const_image_data<fmt>&);

    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P1, pixel_format_t::BW> (std::ostream&, const draw::const_image_data<pixel_format_t::BW>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P2, pixel_format_t::GRAY> (std::ostream&, const draw::const_image_data<pixel_format_t::GRAY>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P3, pixel_format_t::RGB> (std::ostream&, const draw::const_image_data<pixel_format_t::RGB>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P3, pixel_format_t::RGBA> (std::ostream&, const draw::const_image_data<pixel_format_t::RGBA>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P3, pixel_format_t::BGR> (std::ostream&, const draw::const_image_data<pixel_format_t::BGR>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P3, pixel_format_t::BGRA> (std::ostream&, const draw::const_image_data<pixel_format_t::BGRA>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P4, pixel_format_t::BW> (std::ostream&, const draw::const_image_data<pixel_format_t::BW>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P5, pixel_format_t::GRAY> (std::ostream&, const draw::const_image_data<pixel_format_t::GRAY>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P6, pixel_format_t::RGB> (std::ostream&, const draw::const_image_data<pixel_format_t::RGB>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P6, pixel_format_t::RGBA> (std::ostream&, const draw::const_image_data<pixel_format_t::RGBA>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P6, pixel_format_t::BGR> (std::ostream&, const draw::const_image_data<pixel_format_t::BGR>&);
    template<> GUIPP_IO_EXPORT void write_pnm<PNM::P6, pixel_format_t::BGRA> (std::ostream&, const draw::const_image_data<pixel_format_t::BGRA>&);

    // --------------------------------------------------------------------------
    template<PNM i>
    draw::datamap<PNM2BPP<i>::px_fmt> read_pnm (std::istream& in, const draw::bitmap_info&);

    template<> GUIPP_IO_EXPORT draw::datamap<pixel_format_t::BW>   read_pnm<PNM::P1> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<pixel_format_t::GRAY> read_pnm<PNM::P2> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<pixel_format_t::RGB>  read_pnm<PNM::P3> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<pixel_format_t::BW>   read_pnm<PNM::P4> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<pixel_format_t::GRAY> read_pnm<PNM::P5> (std::istream&, const draw::bitmap_info& bmi);
    template<> GUIPP_IO_EXPORT draw::datamap<pixel_format_t::RGB>  read_pnm<PNM::P6> (std::istream&, const draw::bitmap_info& bmi);

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
      static inline std::string build_filename (T t, PNM i) {
        return ostreamfmt(t << "." << get_suffix(i));
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
      static inline std::string build_filename (T t) {
        return ostreamfmt(t << "." << suffix);
      }

    };

    template<PNM i>
    constexpr const char*const pnm<i>::suffix;

    template<pixel_format_t T>
    void save_pnm (std::ostream& out, const draw::datamap<T>& bmp, bool binary = true);

    template<pixel_format_t T>
    void save_pnm (const std::string& name, const draw::datamap<T>& bmp, bool binary = true);

    template<pixel_format_t T>
    void load_pnm (std::istream& in, draw::datamap<T>& bmp);

    template<pixel_format_t T>
    void load_pnm (const std::string& name, draw::datamap<T>& bmp);

    GUIPP_IO_EXPORT void load_pnm (std::istream& in, draw::basic_datamap& bmp);
    GUIPP_IO_EXPORT void load_pnm (const std::string& name, draw::basic_datamap& bmp);

    // --------------------------------------------------------------------------
    template<bool BIN, pixel_format_t T>
    class opnm {
    public:
      explicit opnm (const draw::datamap<T>& bmp);
      void write (std::ostream& out) const;

    private:
      const draw::datamap<T>& bmp;
    };

    // --------------------------------------------------------------------------
    template<bool BIN, pixel_format_t T>
    std::ostream& operator<< (std::ostream& out, const opnm<BIN, T>& p);

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    class ipnm {
    public:
      explicit ipnm (draw::datamap<T>& bmp);
      explicit ipnm (draw::datamap<T>&& bmp);
      void read (std::istream& in);

    private:
      draw::datamap<T>& bmp;
    };

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    std::istream &operator>> (std::istream& in, ipnm<T>& p);

    template<pixel_format_t T>
    std::istream &operator>> (std::istream& in, ipnm<T>&& p);

    // --------------------------------------------------------------------------
    template<PNM i>
    struct ofpnm : public pnm<i> {
      typedef pnm<i> super;

      ofpnm ();
      explicit ofpnm (const char* fname);
      explicit ofpnm (const std::string& fname);

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
      explicit ifpnm (const char* fname);
      explicit ifpnm (const std::string& fname);

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

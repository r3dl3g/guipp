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
 * @brief     save/load images to/from pnm file or stream.
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace io {

    template<bool BIN, pixel_format_t T>
    inline opnm<BIN, T>::opnm (const draw::datamap<T>& map)
      : bmp(map.get_data())
    {}

    template<bool BIN, pixel_format_t T>
    inline opnm<BIN, T>::opnm (const draw::const_image_data<T>& bmp)
      : bmp(bmp)
    {}

    template<bool BIN, pixel_format_t T>
    inline void opnm<BIN, T>::write (std::ostream& out) const {
      const draw::bitmap_info& bmi = bmp.get_info();
      write_pnm_header(out, BPP2PNM<T, BIN>::pnm, bmi, BPP2MAX<T>::max);
      write_pnm<BPP2PNM<T, BIN>::pnm, T>(out, bmp);
    }

//    template<>
//    inline void opnm<false, pixel_format_t::RGBA>::write (std::ostream& out) const {
//      const draw::bitmap_info& bmi = bmp.get_info();
//      write_pnm_header(out, BPP2PNM<pixel_format_t::RGBA, false>::pnm, bmi, BPP2MAX<pixel_format_t::RGBA>::max);
//      write_pnm_rgba<false>(out, bmp.get_data());
//    }

//    template<>
//    inline void opnm<true, pixel_format_t::RGBA>::write (std::ostream& out) const {
//      const draw::bitmap_info& bmi = bmp.get_info();
//      write_pnm_header(out, BPP2PNM<pixel_format_t::RGBA, true>::pnm, bmi, BPP2MAX<pixel_format_t::RGBA>::max);
//      write_pnm_rgba<true>(out, bmp.get_data());
//    }

    // --------------------------------------------------------------------------
    template<bool BIN, pixel_format_t T>
    inline std::ostream& operator<< (std::ostream& out, const opnm<BIN, T>& p) {
      p.write(out);
      return out;
    }

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline ipnm<T>::ipnm (draw::datamap<T>& bmp)
      : bmp(bmp)
    {}

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline ipnm<T>::ipnm (draw::datamap<T>&& bmp)
      : bmp(bmp)
    {}

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline void ipnm<T>::read (std::istream& in) {
      int max;
      PNM pnm;
      draw::bitmap_info bmi = read_pnm_header(in, pnm, max);
      if (pnm2bpp(pnm) == T) {
        switch (pnm) {
          case PNM::P1: bmp = draw::datamap<T>{read_pnm<PNM::P1>(in, bmi)};  break;
          case PNM::P2: bmp = draw::datamap<T>{read_pnm<PNM::P2>(in, bmi)};  break;
          case PNM::P3: bmp = draw::datamap<T>{read_pnm<PNM::P3>(in, bmi)};  break;
          case PNM::P4: bmp = draw::datamap<T>{read_pnm<PNM::P4>(in, bmi)};  break;
          case PNM::P5: bmp = draw::datamap<T>{read_pnm<PNM::P5>(in, bmi)};  break;
          case PNM::P6: bmp = draw::datamap<T>{read_pnm<PNM::P6>(in, bmi)};  break;
        }
      } else {

      }
    }

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline std::istream &operator>> (std::istream& in, ipnm<T>& p) {
      p.read(in);
      return in;
    }

    template<pixel_format_t T>
    inline std::istream &operator>> (std::istream& in, ipnm<T>&& p) {
      p.read(in);
      return in;
    }

    // --------------------------------------------------------------------------
    template<PNM i>
    inline ofpnm<i>::ofpnm () = default;

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
    inline void ofpnm<i>::operator<< (const draw::pixmap& b) {
      switch (b.pixel_format()) {
        case pixel_format_t::Undefined: break;
        case pixel_format_t::BW:   opnm<super::bin, pixel_format_t::BW>(b.get<pixel_format_t::BW>()).write(out); break;
        case pixel_format_t::GRAY: opnm<super::bin, pixel_format_t::GRAY>(b.get<pixel_format_t::GRAY>()).write(out); break;
        case pixel_format_t::RGB:  opnm<super::bin, pixel_format_t::RGB>(b.get<pixel_format_t::RGB>()).write(out); break;
        case pixel_format_t::BGR:  opnm<super::bin, pixel_format_t::BGR>(b.get<pixel_format_t::BGR>()).write(out); break;
        case pixel_format_t::ARGB:
        case pixel_format_t::RGBA: opnm<super::bin, pixel_format_t::RGBA>(b.get<pixel_format_t::RGBA>()).write(out); break;
        case pixel_format_t::ABGR:
        case pixel_format_t::BGRA: opnm<super::bin, pixel_format_t::BGRA>(b.get<pixel_format_t::BGRA>()).write(out); break;
      }
    }

    template<PNM i>
    inline void ofpnm<i>::operator<< (const draw::datamap<PNM2BPP<i>::px_fmt>& b) {
      opnm<super::bin, PNM2BPP<i>::px_fmt>(b).write(out);
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
    inline void ifpnm<i>::operator>> (draw::pixmap& b) {
      draw::datamap<PNM2BPP<i>::px_fmt> m;
      in >> ipnm<PNM2BPP<i>::px_fmt>(m);
      b = m;
    }

    template<PNM i>
    inline void ifpnm<i>::operator>> (draw::datamap<PNM2BPP<i>::px_fmt>& b) {
      in >> ipnm<PNM2BPP<i>::px_fmt>(b);
    }

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline void save_pnm (std::ostream& out, const draw::const_image_data<T>& bmp, bool binary) {
      if (binary) {
        out << opnm<true, T>(bmp);
      } else {
        out << opnm<false, T>(bmp);
      }
    }

    template<pixel_format_t T>
    inline void save_pnm (const std::string& name, const draw::const_image_data<T>& bmp, bool binary) {
      std::ofstream out(name);
      save_pnm(out, bmp, binary);
    }

    template<pixel_format_t T>
    inline void save_pnm (std::ostream& out, const draw::datamap<T>& bmp, bool binary) {
      save_pnm(out, bmp.get_data(), binary);
    }

    template<pixel_format_t T>
    inline void save_pnm (const std::string& name, const draw::datamap<T>& bmp, bool binary) {
      save_pnm(name, bmp.get_data(), binary);
    }

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline void load_pnm (std::istream& in, draw::datamap<T>& bmp) {
      in >> ipnm<T>(bmp);
    }

    template<pixel_format_t T>
    inline void load_pnm (const std::string& name, draw::datamap<T>& bmp) {
      std::ifstream in(name);
      load_pnm(in, bmp);
    }

    inline void load_pnm (const std::string& name, draw::basic_datamap& bmp) {
      std::ifstream in(name);
      load_pnm(in, bmp);
    }

  } // namespace io

} // namespace gui

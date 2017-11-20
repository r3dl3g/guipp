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


namespace gui {

  namespace io {

    template<bool BIN, BPP T>
    inline opnm<BIN, T>::opnm (const draw::datamap<T>& bmp)
      : bmp(bmp)
    {}

    template<bool BIN, BPP T>
    inline void opnm<BIN, T>::write (std::ostream& out) const {
      const draw::bitmap_info& bmi = bmp.get_info();
      write_pnm_header(out, BPP2PNM<T, BIN>::pnm, bmi, BPP2MAX<T>::max);
      write_pnm<BPP2PNM<T, BIN>::pnm>(out, bmp.get_raw());
    }

    template<>
    inline void opnm<false, BPP::RGBA>::write (std::ostream& out) const {
      const draw::bitmap_info& bmi = bmp.get_info();
      write_pnm_header(out, BPP2PNM<BPP::RGBA, false>::pnm, bmi, BPP2MAX<BPP::RGBA>::max);
      write_pnm_rgba<false>(out, bmp.get_raw());
    }

    template<>
    inline void opnm<true, BPP::RGBA>::write (std::ostream& out) const {
      const draw::bitmap_info& bmi = bmp.get_info();
      write_pnm_header(out, BPP2PNM<BPP::RGBA, true>::pnm, bmi, BPP2MAX<BPP::RGBA>::max);
      write_pnm_rgba<true>(out, bmp.get_raw());
    }

    // --------------------------------------------------------------------------
    template<bool BIN, BPP T>
    inline std::ostream& operator<< (std::ostream& out, const opnm<BIN, T>& p) {
      p.write(out);
      return out;
    }

    // --------------------------------------------------------------------------
    template<BPP T>
    inline ipnm<T>::ipnm (draw::datamap<T>& bmp)
      : bmp(bmp)
    {}

    // --------------------------------------------------------------------------
    template<BPP T>
    inline ipnm<T>::ipnm (draw::datamap<T>&& bmp)
      : bmp(bmp)
    {}

    // --------------------------------------------------------------------------
    template<BPP T>
    inline void ipnm<T>::read (std::istream& in) {
      int max;
      PNM pnm;
      draw::bitmap_info bmi = read_pnm_header(in, pnm, max);
      if (pnm2bpp(pnm) == T) {
        switch (pnm) {
          case PNM::P1: bmp = read_pnm<PNM::P1>(in, bmi);  break;
          case PNM::P2: bmp = read_pnm<PNM::P2>(in, bmi);  break;
          case PNM::P3: bmp = read_pnm<PNM::P3>(in, bmi);  break;
          case PNM::P4: bmp = read_pnm<PNM::P4>(in, bmi);  break;
          case PNM::P5: bmp = read_pnm<PNM::P5>(in, bmi);  break;
          case PNM::P6: bmp = read_pnm<PNM::P6>(in, bmi);  break;
        }
      } else {

      }
    }

    // --------------------------------------------------------------------------
    template<BPP T>
    inline std::istream &operator>> (std::istream& in, ipnm<T>& p) {
      p.read(in);
      return in;
    }

    template<BPP T>
    inline std::istream &operator>> (std::istream& in, ipnm<T>&& p) {
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
    inline void ofpnm<i>::operator<< (const draw::pixmap& b) {
      switch (b.bits_per_pixel()) {
        case BPP::BW:   opnm<super::bin, BPP::BW>(b.get<BPP::BW>()).write(out); break;
        case BPP::GRAY: opnm<super::bin, BPP::GRAY>(b.get<BPP::GRAY>()).write(out); break;
        case BPP::RGB:  opnm<super::bin, BPP::RGB>(b.get<BPP::RGB>()).write(out); break;
        case BPP::RGBA: opnm<super::bin, BPP::RGBA>(b.get<BPP::RGBA>()).write(out); break;
      }
    }

    template<PNM i>
    inline void ofpnm<i>::operator<< (const draw::datamap<PNM2BPP<i>::bpp>& b) {
      opnm<super::bin, PNM2BPP<i>::bpp>(b).write(out);
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
      draw::datamap<PNM2BPP<i>::bpp> m;
      in >> ipnm<PNM2BPP<i>::bpp>(m);
      b = m;
    }

    template<PNM i>
    inline void ifpnm<i>::operator>> (draw::datamap<PNM2BPP<i>::bpp>& b) {
      in >> ipnm<PNM2BPP<i>::bpp>(b);
    }

    // --------------------------------------------------------------------------
    template<BPP T>
    inline void save_pnm (std::ostream& out, const draw::datamap<T>& bmp, bool binary) {
      if (binary) {
        out << opnm<true, T>(bmp);
      } else {
        out << opnm<false, T>(bmp);
      }
    }

    template<BPP T>
    inline void load_pnm (std::istream& in, draw::datamap<T>& bmp) {
      in >> ipnm<T>(bmp);
    }

    template<BPP T>
    inline void save_pnm (const std::string& name, const draw::datamap<T>& bmp, bool binary) {
      std::ofstream out(name);
      save_pnm(out, bmp, binary);
    }

    template<BPP T>
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

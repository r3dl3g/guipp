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
#include "pnm.h"


namespace gui {

  namespace io {

    namespace src {
      // --------------------------------------------------------------------------
      void save_pnm_header_src (std::ostream& out, const std::string& name, PNM p, int w, int h, int max);

      template<BPP>
      void save_pnm_src(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl);

      template<> void save_pnm_src<BPP::BW>(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl);
      template<> void save_pnm_src<BPP::GRAY>(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl);
      template<> void save_pnm_src<BPP::RGB>(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl);
      template<> void save_pnm_src<BPP::RGBA>(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl);

      void save_pnm_src(std::ostream& out, const draw::bitmap& bmp, const std::string& name);
      void save_pnm_src(const std::string& filename, const draw::bitmap& bmp, const std::string& name);

      // --------------------------------------------------------------------------
      class opnm {
      public:
        opnm (const draw::bitmap& bmp, const std::string& name)
          : bmp(bmp)
          , name(name)
        {}

        void write(std::ostream& out) const {
          int w, h, bpl;
          BPP bpp;
          std::vector<char> data;
          bmp.get_data(data, w, h, bpl, bpp);
          switch (bpp) {
          case BPP::BW:
            save_pnm_header_src(out, name, BPP2PNM<BPP::BW, true>::pnm, w, h, 0);
            save_pnm_src<BPP::BW>(out, data, w, h, bpl);
            break;
          case BPP::GRAY:
            save_pnm_header_src(out, name, BPP2PNM<BPP::GRAY, true>::pnm, w, h, 255);
            save_pnm_src<BPP::GRAY>(out, data, w, h, bpl);
            break;
          case BPP::RGB:
            save_pnm_header_src(out, name, BPP2PNM<BPP::RGB, true>::pnm, w, h, 255);
            save_pnm_src<BPP::RGB>(out, data, w, h, bpl);
            break;
          case BPP::RGBA:
            save_pnm_header_src(out, name, BPP2PNM<BPP::RGBA, true>::pnm, w, h, 255);
            save_pnm_src<BPP::RGBA>(out, data, w, h, bpl);
            break;
          default:
            throw std::invalid_argument("unsupportet bit per pixel value");
          }
        }

      private:
        const draw::bitmap& bmp;
        const std::string name;
      };

      // --------------------------------------------------------------------------
      inline std::ostream& operator<< (std::ostream& out, const opnm& p) {
        p.write(out);
        return out;
      }

    } // src
    // --------------------------------------------------------------------------
  }

}

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
      void save_pnm_header_src (std::ostream& out, const std::string& name, PNM p, int w, int h, int max);

      template<BPP>
      void save_pnm_src (std::ostream& out, const blob& data, int width, int height, int bpl);

      template<> void save_pnm_src<BPP::BW>(std::ostream&, const blob&, int, int, int);
      template<> void save_pnm_src<BPP::GRAY>(std::ostream&, const blob&, int, int, int);
      template<> void save_pnm_src<BPP::RGB>(std::ostream&, const blob&, int, int, int);
      template<> void save_pnm_src<BPP::RGBA>(std::ostream&, const blob&, int, int, int);

      void save_pnm_src (std::ostream& out, const draw::bitmap& bmp, const std::string& name);
      void save_pnm_src (const std::string& filename, const draw::bitmap& bmp, const std::string& name);

      // --------------------------------------------------------------------------
      class opnm {
      public:
        opnm (const draw::bitmap& bmp, const std::string& name);
        void write (std::ostream& out) const;

      private:
        const draw::bitmap& bmp;
        const std::string name;
      };

      // --------------------------------------------------------------------------
      std::ostream& operator<< (std::ostream& out, const opnm& p);

      // --------------------------------------------------------------------------
      //
      // inlines
      //
      inline opnm::opnm (const draw::bitmap& bmp, const std::string& name)
        : bmp(bmp)
        , name(name)
      {}

      inline std::ostream& operator<< (std::ostream& out, const opnm& p) {
        p.write(out);
        return out;
      }

      // --------------------------------------------------------------------------
    } // src
  }

}

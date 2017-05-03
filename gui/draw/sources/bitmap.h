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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"


namespace gui {

  namespace draw {

    class bitmap {
    public:
      bitmap ()
        : id(0)
      {}

      explicit bitmap (os::bitmap id)
        : id(id)
      {}

      ~bitmap () {
        clear();
      }

      void operator= (const bitmap&);
      void operator= (bitmap&&);

      bool is_valid () const {
        return id != 0;
      }

      operator bool () const {
        return is_valid();
      }

      os::bitmap get_id () const {
        return id;
      }

      void create (const std::vector<char>& data, int w, int h, int bpl, BPP bpp);
      void clear ();

      void put_data (const std::vector<char>& data, int w, int h, int bpl, BPP bpp);
      void get_data (std::vector<char>& data, int& w, int& h, int& bpl, BPP& bpp) const;

      void put (const bitmap& rhs);

      core::size size () const;
      int depth () const;
      BPP bpp () const;

      static int calc_bytes_per_line (int w, BPP bpp);

    protected:
      bitmap (const bitmap&);
      bitmap (bitmap&&);

      void create (int w, int h);
      void create (int w, int h, BPP bpp);
      void copy_from (const bitmap&);

      os::bitmap id;

    };

    class memmap : public bitmap {
    public:
      typedef bitmap super;

      memmap ()
      {}

      memmap (int w, int h) {
        create(w, h);
      }

      memmap (const core::size& sz) {
        create(sz);
      }

      operator os::drawable () const {
        return id;
      }

      inline void create (int w, int h) {
        super::create(w, h);
      }

      inline void create (const core::size& sz) {
        create(sz.os_width(), sz.os_height());
      }
    };

    template<BPP T>
    class datamap : public bitmap {
    public:
      typedef bitmap super;

      datamap ()
      {}

      datamap (int w, int h) {
        create(w, h);
      }

      datamap (const core::size& sz) {
        create(sz);
      }

      inline void create (int w, int h) {
        super::create(w, h, T);
      }

      inline void create (const core::size& sz) {
        create(sz.os_width(), sz.os_height());
      }
    };

    typedef datamap<BPP::BW> maskmap;
    typedef datamap<BPP::GRAY> graymap;
    typedef datamap<BPP::RGB> rgbmap;
    typedef datamap<BPP::RGBA> rgbamap;


    class transparent_bitmap : public memmap {
    public:
      typedef memmap super;

      transparent_bitmap ()
      {}

      transparent_bitmap (const transparent_bitmap&);
      void operator= (const transparent_bitmap& rhs);

      transparent_bitmap (transparent_bitmap&&);
      void operator= (transparent_bitmap&&);

      transparent_bitmap (const memmap& bmp);
      void operator= (const memmap& bmp);

      transparent_bitmap (memmap&& bmp);
      void operator= (memmap&& bmp);

      maskmap mask;
    };

  }

}

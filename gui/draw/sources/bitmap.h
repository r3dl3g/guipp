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
      BPP bits_per_pixel () const;

      static int calc_bytes_per_line (int w, BPP bpp);

    protected:
      bitmap (const bitmap&);
      bitmap (bitmap&&);

      void create_compatible (int w, int h);
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

      void operator= (const bitmap& rhs) {
        if (this != &rhs) {
          create(rhs.size());
          put(rhs);
        }
      }

      void operator= (const memmap& rhs) {
        if (this != &rhs) {
          create(rhs.size());
          put(rhs);
        }
      }

      operator os::drawable () const {
        return id;
      }

      inline void create (int w, int h) {
        create_compatible(w, h);
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

      void operator= (const bitmap& rhs) {
        if (this != &rhs) {
          create(rhs.size());
          put(rhs);
        }
      }

      void operator= (const datamap& rhs) {
        if (this != &rhs) {
          create(rhs.size());
          put(rhs);
        }
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


    class masked_bitmap : public memmap {
    public:
      typedef memmap super;

      masked_bitmap ()
      {}

      masked_bitmap (const masked_bitmap&);
      void operator= (const masked_bitmap& rhs);

      masked_bitmap (masked_bitmap&&);
      void operator= (masked_bitmap&&);

      masked_bitmap (const memmap& bmp);
      void operator= (const memmap& bmp);

      masked_bitmap (memmap&& bmp);
      void operator= (memmap&& bmp);

      maskmap mask;
    };

  }

}

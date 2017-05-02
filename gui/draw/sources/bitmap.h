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
#include "graphics.h"


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

      bitmap (const core::size& sz)
        : id(0)
      {
        create(sz);
      }

      bitmap (int w, int h)
        : id(0)
      {
        create(w, h);
      }

      bitmap (const core::size& sz, int bpp)
        : id(0)
      {
        create(sz, bpp);
      }

      bitmap (const bitmap&);
      bitmap (bitmap&&);

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

      operator os::drawable () const {
        return id;
      }

      void create (const core::size&);
      void create (const core::size&, int bpp);
      void create (int w, int h);
      void create (int w, int h, int bpp);
      void create_compatible (const bitmap&);
      void create (const std::vector<char>& data, int w, int h, int bpl, int bpp);

      void put (const std::vector<char>& data, int w, int h, int bpl, int bpp);
      void put (const bitmap& rhs);

      void make_compatible ();

      void clear ();

      void get_data (std::vector<char>& data, int& w, int& h, int& bpl, int& bpp) const;

      core::size size () const;
      int depth () const;

      static int calc_bytes_per_line (int w, int bpp);

      void operator() (const graphics&, const core::point&) const;
      void draw (const graphics&, const core::point&) const;

    private:
      os::bitmap id;

    };

    class transparent_bitmap : public bitmap {
    public:
      typedef bitmap super;

      transparent_bitmap ()
      {}

      transparent_bitmap (const transparent_bitmap&);
      void operator= (const transparent_bitmap& rhs);

      transparent_bitmap (transparent_bitmap&&);
      void operator= (transparent_bitmap&&);

      transparent_bitmap (const bitmap& bmp);
      void operator= (const bitmap& bmp);

      transparent_bitmap (bitmap&& bmp);
      void operator= (bitmap&& bmp);

      void operator() (const graphics&, const core::point&) const;
      void draw (const graphics&, const core::point&) const;

    private:
      bitmap mask;
    };

  }

}

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

#if WIN32
    struct log_palette : public LOGPALETTE {
      log_palette (int bpp);

    private:
      PALETTEENTRY moreEntries[255];
    };

    struct palette {
      palette (const log_palette&);
      ~palette ();

      HPALETTE get_id() const {
        return id;
      }

//    private:
      HPALETTE id;
    };

    struct bitmap_info : public BITMAPINFO {

      bitmap_info();
      bitmap_info(int w, int h, int bpl, int bpp);

      void init_colors();
      void init_gray_colors();
      void init_bw_colors();

      void set_gray_colors(HBITMAP id);
      void set_gray_colors(HDC id);
      void set_bw_colors(HBITMAP id);
      void set_bw_colors(HDC id);

    private:
      RGBQUAD moreColors[255];
    };
#endif

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

      bitmap mask;
    };

  }

}

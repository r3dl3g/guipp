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

  namespace core {
    void save_ppm_p6(std::ostream& out, char* data, int width, int height, int bpl, int bpp);
    void load_ppm_p6(std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp);
  }

  namespace draw {

    class bitmap {
    public:
      bitmap (os::bitmap id = 0)
        : id(id)
      {}

      bitmap (const core::size& sz)
        : id(0)
      {
        create(sz);
      }

      bitmap (const bitmap&);

      ~bitmap () {
        clear();
      }

      void operator= (const bitmap&);

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
      void create (int w, int h);
      void create (const std::vector<char>& data, int w, int h, int bpl, int bpp);

      void clear ();

      void get_data (std::vector<char>& data, int& w, int& h, int& bpl, int& bpp) const;

      core::size size () const;

      void load_ppm (std::istream&);
      void save_ppm (std::ostream&) const;

    private:
      os::bitmap id;

    };

    std::ostream& operator<< (std::ostream& out, const bitmap& bmp);
    std::istream& operator>> (std::istream& in, bitmap& bmp);

  }

}

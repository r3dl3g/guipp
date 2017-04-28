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
      bitmap (os::bitmap id = 0)
        : id(id)
        , gc(0)
      {}

      bitmap (const core::size& sz)
        : id(0)
        , gc(0)
      {
        create(sz);
      }

      ~bitmap () {
        clear();
      }

      os::bitmap get_id () const {
        return id;
      }

      os::graphics get_gc ();

      operator os::drawable () const {
        return id;
      }

      void create (const core::size&);
      void clear ();

      core::size size () const;

      void load_ppm (std::istream&);
      void save_ppm (std::ostream&) const;

      void copy_from (os::drawable, const core::rectangle&);
      void copy_to (os::drawable, const core::rectangle&) const;
      void copy_to (os::drawable, const core::point&) const;

    private:
      os::bitmap id;
      os::graphics gc;
    };

    std::ostream& operator<< (std::ostream& out, const bitmap& bmp);
    std::istream& operator>> (std::istream& in, bitmap& bmp);

  }

}

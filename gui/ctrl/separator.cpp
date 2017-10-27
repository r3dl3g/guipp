/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: label
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/separator.h>


namespace gui {

  namespace win {

    namespace detail {

      separator_base::separator_base ()
      {}

      void separator_base::create (const container& parent,
                                   const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
      }

    } // detail

  } // win

} // gui

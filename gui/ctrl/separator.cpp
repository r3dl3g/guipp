/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     pasive separator view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/separator.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      void separator_base::create (win::container& parent,
                                   const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
      }

    } // detail

  } // win

} // gui

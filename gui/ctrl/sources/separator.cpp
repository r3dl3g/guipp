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

#include "separator.h"

namespace gui {

  namespace win {

    namespace detail {

      separator_base::separator_base ()
      {}

      no_erase_window_class separator_base::clazz("SEPARATOR++");

    } // detail

  } // win

} // gui

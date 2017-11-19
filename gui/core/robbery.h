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
 * @brief     Rob private member helper functions
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>

namespace gui {

  // technique for accessing private class members
  //
  //  from: http://bloglitb.blogspot.com/2011/12/access-to-private-members-safer.html
  //
  namespace robbery {

#ifdef X11
# pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif // X11

    template<typename T, typename Class>
    struct jugger {
      typedef T type;
      typedef T Class::*Type;
      friend Type rob (jugger);
    };

#ifdef X11
# pragma GCC diagnostic pop
#endif // X11

    template<typename Tag, typename Tag::Type M>
    struct robber {
      friend typename Tag::Type rob (Tag) {
        return M;
      }
    };

  } // namespace robbery

} // gui

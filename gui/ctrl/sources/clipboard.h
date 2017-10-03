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
* @brief     C++ API: separator view
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
#include "control.h"

namespace gui {

  namespace win {

    class clipboard {
    public:
      typedef void(text_callback)(const std::string&);

      static clipboard& get ();

      void set_text (window&, const std::string&);
      void get_text (window&, const std::function<text_callback>&);

    private:
      clipboard ();

      std::string text;
#ifdef X11
      int filter_id;
#endif // X11
    };

  } // win

} // gui

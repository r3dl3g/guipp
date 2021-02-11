/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

#include <string>
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui++-win-export.h>


namespace gui {

  namespace win {

    class window;

    class GUIPP_WIN_EXPORT clipboard {
    public:
      typedef void (text_callback)(const std::string&);

      static clipboard& get ();

      void set_text (window&, const std::string&);
      void get_text (window&, std::function<text_callback>&&);

    private:
      clipboard ();

      std::string text;
#ifdef GUIPP_X11
      int filter_id;
#endif // GUIPP_X11
    };

  } // win

} // gui

/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
* @brief     C++ API: window class definition
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "color.h"


namespace gui {

  namespace win {

    class window;

    class window_class {
    public:
      window_class ();
      window_class (const window_class&);

      window_class (const std::string& cls_name,
                    os::color background = color::white,
                    os::cursor_type cursor = IF_WIN32(IDC_ARROW) IF_X11(0),
                    os::style style = IF_X11(0) IF_WIN32(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP),
                    os::style ex_style = IF_X11(0) IF_WIN32(WS_EX_NOPARENTNOTIFY));

      virtual void prepare (window*) const;

      const std::string& get_class_name () const;
      const os::color get_background () const;
      const os::cursor get_cursor () const;
      const os::style get_style () const;
      const os::style get_ex_style () const;

      bool is_valid () const;

      inline ~window_class () {
        unregister_class();
      }

    protected:
      virtual os::brush get_background_brush () const;

      void register_class () const;
      void unregister_class ();

      std::string class_name;
      mutable os::color background;
      mutable os::cursor cursor;
      os::cursor_type cursor_type;
      os::style style;
      os::style ex_style;
      mutable bool is_initialized;
    };

  } // win

} // gui

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
* @brief     C++ API: window event handler function
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
#include "guidefs.h"


namespace gui {

  namespace win {

    class window;

    namespace detail {

#ifdef WIN32

      LRESULT CALLBACK WindowEventProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
      
      void set_item_size(int id, const gui::core::size&);

      const gui::core::size& get_item_size(int id);

#endif // WIN32

      window* get_window (os::window id);
      void set_window (os::window id, window* win);
      void unset_window (os::window id);

    } // detail

    int run_main_loop();

  } // win

} // gui

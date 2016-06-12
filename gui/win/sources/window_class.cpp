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

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window_class.h"
#include "window_event_proc.h"


namespace gui {

  namespace win {

    class window;

    std::string getLastErrorText() {
      LPTSTR lpMsgBuf;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
      return lpMsgBuf;
    }

    void window_class::register_class() {

      WNDCLASSEX wc;
      memset(&wc, 0, sizeof(WNDCLASSEX));
      wc.cbSize = sizeof(WNDCLASSEX);

      /* Register the window class. */
      wc.style = class_style;
      wc.lpfnWndProc = detail::WindowEventProc;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = sizeof(window*);
      wc.hInstance = instance;
      wc.hIcon = icon;
      wc.hCursor = cursor;
      wc.hbrBackground = brush;
      wc.lpszMenuName = NULL;
      wc.lpszClassName = class_name.c_str();
      wc.hIconSm = NULL;

      ATOM result = RegisterClassEx(&wc);
      if (!result) {
        throw std::runtime_error(getLastErrorText());
      }
    }

    void window_class::unregister_class() {
      BOOL result = UnregisterClass(class_name.c_str(), instance);
      if (!result) {
        throw std::runtime_error(getLastErrorText());
      }
    }

  } // win

} // gui

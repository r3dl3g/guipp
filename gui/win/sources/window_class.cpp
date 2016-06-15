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

    core::instance_id window_class::global_instance;
    bool window_class::is_global_initialized = false;

    class window;

    std::string getLastErrorText() {
      LPTSTR lpMsgBuf;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
      return lpMsgBuf;
    }

    const std::string& window_class::get_class_name() const {
      register_class();
      return class_name;
    }

    const core::instance_id window_class::get_instance() const {
      register_class();
      return global_instance;
    }

    const core::windows_style window_class::get_class_style() const {
      register_class();
      return class_style;
    }

    const core::windows_style window_class::get_style() const {
      register_class();
      return style;
    }

    const core::windows_style window_class::get_ex_style() const {
      register_class();
      return ex_style;
    }

    const core::icon_id window_class::get_icon() const {
      register_class();
      return icon;
    }

    const core::cursor_id window_class::get_cursor() const {
      register_class();
      return cursor;
    }

    const core::brush_id window_class::get_brush() const {
      register_class();
      return brush;
    }

    void window_class::init(core::instance_id instance) {
      global_instance = instance;
      is_global_initialized = true;
    }

    void window_class::register_class() const {
      if (is_initialized) {
        return;
      }

      if (!is_global_initialized) {
        throw std::runtime_error("window_class::init must be called before first use!");
        return;
      }

      WNDCLASSEX wc;
      memset(&wc, 0, sizeof(WNDCLASSEX));
      wc.cbSize = sizeof(WNDCLASSEX);

      /* Register the window class. */
      wc.style = class_style;
      wc.lpfnWndProc = detail::WindowEventProc;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = sizeof(window*);
      wc.hInstance = global_instance;
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
      is_initialized = true;
    }

    void window_class::unregister_class() {
      BOOL result = UnregisterClass(class_name.c_str(), global_instance);
      if (!result) {
        throw std::runtime_error(getLastErrorText());
      }
    }

  } // win

} // gui

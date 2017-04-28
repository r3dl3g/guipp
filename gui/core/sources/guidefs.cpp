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
* @brief     C++ API: basic window types
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//
#include <stdexcept>
#include <logger.h>
#include <dbgstream.h>
#include <iterator>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"

namespace gui {

  namespace core {

    namespace global {

      bool is_global_initialized = false;
      os::instance global_instance = 0;

#ifdef X11
      os::x11::screen global_screen = 0;

      int XErrorHandler (Display* dpy,
                         XErrorEvent* errev) {
        if ((errev->error_code == 143) && (errev->request_code == 139)) {
          return 0;
        }

        char buffer[256];
        XGetErrorText(dpy, errev->error_code, buffer, sizeof(buffer));

        LogFatal << "Error occured somewhere in X!"
                    " ResourceID: " << errev->resourceid <<
                    " Serial: " << errev->serial <<
                    " Error_code: " << (int) errev->error_code <<
                    " Request_code: " << (int) errev->request_code <<
                    " Minor_code: " << (int) errev->minor_code <<
                    " Text: " << buffer;
        return 0;
      }

#endif // X11

      void init (os::instance instance) {
        global_instance = instance;
#ifdef X11
        global_screen = DefaultScreen(global_instance);
        XSetErrorHandler(XErrorHandler);
#endif // X11
        is_global_initialized = true;
      }

      os::instance get_instance () {
        if (!is_global_initialized) {
          throw std::runtime_error("gui::core::global::init must be called before first use!");
        }
        return global_instance;
      }

      void fini () {
        if (global_instance) {
#ifdef X11
          XSync(global_instance, False);
          XCloseDisplay(global_instance);
          global_screen = 0;
#endif // X11
          global_instance = 0;
        }
      }

      void sync () {
        if (global_instance) {
#ifdef X11
          XSync(global_instance, False);
#endif // X11
        }
      }

      int get_device_bits_per_pixel () {
#ifdef WIN32
        HDC gdc = GetDC(NULL);
        int dbpp = GetDeviceCaps(gdc, BITSPIXEL);
        ReleaseDC(NULL, gdc);
        return dbpp;
#endif // WIN32
#ifdef X11
        return DisplayPlanes(get_instance(), get_screen());
#endif // X11
      }

#ifdef X11

      os::x11::screen get_screen () {
        return global_screen;
      }

      void set_screen (os::x11::screen screen) {
        global_screen = screen;
      }

#endif // X11

    }
  } // core

} // gui


#ifdef WIN32
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  ibr::odebugstream dbgStrm;
  ibr::log::core::instance().addSink(&dbgStrm, ibr::log::level::debug, ibr::log::core::instance().getConsoleFormatter());


  std::vector<std::string> args{
    std::istream_iterator<std::string>{ std::istringstream(lpCmdLine) },
    std::istream_iterator<std::string>{}
  };

  gui::core::global::init(hInstance);
#endif // WIN32

#ifdef X11
int main(int argc, char* argv[]) {
  ibr::log::core::instance().addSink(&std::cerr,
                                     ibr::log::level::debug,
                                     ibr::log::core::instance().getConsoleFormatter());

  std::vector<std::string> args;
  for(int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }

  gui::core::global::init(XOpenDisplay(0));
#endif

  int ret = 0;
  try {
    ret = gui_main(args);
  } catch (std::exception& e) {
    LogFatal << e;
    ret = 1;
  }

  gui::core::global::fini();

#ifdef X11
  ibr::log::core::instance().removeSink(&std::cerr);
#endif

  ibr::log::core::instance().finish();

  return ret;
}


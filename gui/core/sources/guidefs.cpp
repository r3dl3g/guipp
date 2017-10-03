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
#ifdef X11
#include <X11/XKBlib.h>
#endif
// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"
#include "string_util.h"

namespace gui {

  namespace core {

    namespace global {

#ifdef X11
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

      struct gui_init {
        gui_init ()
          : instance(nullptr)
#ifdef X11
          , screen(0)
#endif // X11
        {}

        void init (os::instance i) {
          instance = i;
#ifdef X11
          screen = DefaultScreen(instance);
          XSetErrorHandler(XErrorHandler);
#endif // X11
        }

        bool is_initialized () const {
          return (instance > 0);
        }

        ~gui_init () {
#ifdef X11
          XSync(instance, False);
          XCloseDisplay(instance);
          screen = 0;
#endif // X11
        }

        os::instance instance;

#ifdef X11
        os::x11::screen screen;
#endif // X11
      };

      gui_init gui_static;


      void init (os::instance instance) {
        gui_static.init(instance);
      }

      os::instance get_instance () {
        if (!gui_static.is_initialized()) {
          throw std::runtime_error("gui::core::global::init must be called before first use!");
        }
        return gui_static.instance;
      }

      void sync () {
#ifdef X11
        if (gui_static.is_initialized()) {
          XFlush(gui_static.instance);
        }
#endif // X11
      }

      int get_device_bits_per_pixel () {
#ifdef WIN32
        HDC gdc = GetDC(NULL);
        int dbpp = GetDeviceCaps(gdc, BITSPIXEL);
        ReleaseDC(NULL, gdc);
        return dbpp;
#endif // WIN32
#ifdef X11
        return DefaultDepth(get_instance(), get_screen());
#endif // X11
      }

#ifdef WIN32
      os::key_state get_key_state () {
        return static_cast<os::key_state>((GetKeyState(VK_SHIFT) & 0x8000 ? MK_SHIFT : 0) |
                                          (GetKeyState(VK_CONTROL) & 0x8000 ? MK_CONTROL : 0) |
                                          (GetKeyState(VK_MENU) & 0x8000 ? MK_MENU : 0) |
                                          (GetKeyState(VK_LWIN) & 0x8000 ? MK_SYTEM : 0) |
                                          (GetKeyState(VK_RWIN) & 0x8000 ? MK_SYTEM : 0));
      }
#endif // WIN32

#ifdef X11
      os::key_state get_key_state () {
        XkbStateRec state;
        XkbGetState(get_instance(), XkbUseCoreKbd, &state);
        return state.base_mods;
      }

      os::x11::screen get_screen () {
        return gui_static.screen;
      }

      void set_screen (os::x11::screen screen) {
        gui_static.screen = screen;
      }

      os::x11::visual get_visual () {
        return DefaultVisual(get_instance(), get_screen());
      }
#endif // X11

    }
  } // core

  constexpr byte bw_bits<bit_order::lsb>::value[];
  constexpr byte bw_bits<bit_order::msb>::value[];

  constexpr byte system_bw_bits::mask[];
  constexpr byte system_bw_bits::shift[];

} // gui


#ifdef WIN32
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  ibr::odebugstream dbgStrm;
  ibr::log::core::instance().add_sink(&dbgStrm, ibr::log::level::debug, ibr::log::core::instance().get_console_formatter());


  std::vector<std::string> args = ibr::string::split<' '>(lpCmdLine);
  gui::core::global::init(hInstance);
#endif // WIN32

#ifdef X11
int main(int argc, char* argv[]) {
//  ibr::log::core::instance().addSink(&std::cerr,
//                                     ibr::log::level::debug,
//                                     ibr::log::core::instance().getConsoleFormatter());

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

#ifdef X11
//  ibr::log::core::instance().removeSink(&std::cerr);
#endif

  ibr::log::core::instance().finish();

  return ret;
}


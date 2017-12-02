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
#include <iterator>
#ifdef X11
# include <X11/XKBlib.h>
# include <iostream>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/logger.h>
#include <gui/core/dbgstream.h>
#include <gui/core/guidefs.h>
#include <gui/core/string_util.h>


namespace gui {

  namespace core {

    namespace global {

#ifdef X11
      int ErrorHandler (Display* dpy, XErrorEvent* errev) {
        if ((errev->error_code == 143) && (errev->request_code == 139)) {
          return 0;
        }

        char buffer[256];
        XGetErrorText(dpy, errev->error_code, buffer, sizeof (buffer));

        LogFatal << "Error occured somewhere in X!"
                    " ResourceID: " << errev->resourceid <<
                    " Serial: " << errev->serial <<
                    " Error_code: " << (int)errev->error_code <<
                    " Request_code: " << (int)errev->request_code <<
                    " Minor_code: " << (int)errev->minor_code <<
                    " Text: " << buffer;

        return 0;
      }

      int IOErrorHandler (Display* dpy) {
        LogFatal << "IO Error occured somewhere in X!";
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
          XSetErrorHandler(ErrorHandler);
          XSetIOErrorHandler(IOErrorHandler);
#endif // X11
        }

        bool is_initialized () const {
#ifdef WIN32
          return (instance > 0);
#endif // WIN32
#ifdef X11
          return (instance != nullptr);
#endif // X11
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

      int get_device_depth () {
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

      BPP get_device_bits_per_pixel () {
#ifdef WIN32
        HDC gdc = GetDC(NULL);
        int dbpp = GetDeviceCaps(gdc, BITSPIXEL);
        ReleaseDC(NULL, gdc);
        return BPP(dbpp);
#endif // WIN32
#ifdef X11
        return BPP(DefaultDepth(get_instance(), get_screen()));
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
int APIENTRY WinMain (_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPTSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  gui::core::odebugstream dbgStrm;
  gui::log::core::instance().add_sink(&dbgStrm, gui::log::level::debug, gui::log::core::get_console_formatter());


  std::vector<std::string> args = gui::string::split<' '>(lpCmdLine);
  gui::core::global::init(hInstance);
#endif // WIN32

#ifdef X11
int main (int argc, char* argv[]) {

#ifndef NDEBUG
//  gui::log::core::instance().add_sink(&std::cerr,
//                                     gui::log::level::debug,
//                                     gui::log::core::get_standard_formatter());
#endif // NDEBUG

  std::vector<std::string> args;
  for (int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }

  gui::core::global::init(XOpenDisplay(0));
#endif // X11

  int ret = 0;
  try {
    ret = gui_main(args);
  } catch (std::exception& e) {
    LogFatal << e;
    ret = 1;
  }

#ifdef X11
#ifndef NDEBUG
//  gui::log::core::instance().removeSink(&std::cerr);
#endif // NDEBUG
#endif // X11

  gui::log::core::instance().finish();

  return ret;
}


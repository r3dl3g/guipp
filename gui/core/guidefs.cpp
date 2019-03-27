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
#ifdef X11
# include <X11/XKBlib.h>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/logger.h>
#include <gui/core/guidefs.h>


namespace gui {

  const char* PixelFormatNames[] = {
    "Undefined",
    "BW",
    "GRAY",
    "RGB",
    "BGR",
    "RGBA",
    "BGRA",
    "ARGB",
    "ABGR"
  };

  std::ostream& operator<< (std::ostream& out, PixelFormat pf) {
    out << PixelFormatNames[static_cast<int>(pf)];
    return out;
  }

  PixelFormat get_pixel_format (int pixel_format, int byte_order) {
    switch (pixel_format) {
      case 1: return PixelFormat::BW;
      case 8: return PixelFormat::GRAY;
      case 24:
        switch (byte_order) {
          case 0: return PixelFormat::BGR;
          case 1: return PixelFormat::RGB;
        }
      case 32:
        switch (byte_order) {
          case 0: return PixelFormat::BGRA;
          case 1: return PixelFormat::RGBA;
        }
    }
    return PixelFormat::Undefined;
  }

  int get_pixel_format_byte_order (PixelFormat px_fmt) {
    switch (px_fmt) {
      case PixelFormat::BGR:
      case PixelFormat::BGRA:
      case PixelFormat::ABGR:
      default:
        return 0;
      case PixelFormat::BW:
      case PixelFormat::GRAY:
      case PixelFormat::RGB:
      case PixelFormat::RGBA:
      case PixelFormat::ARGB:
        return 1;
    }
  }

  namespace core {

    namespace global {

#ifdef X11
      double calc_scale_factor ();

      namespace {
        bool at_shutdown = false;
        double scale_factor = 0.0;
      }

      int ErrorHandler (Display* dpy, XErrorEvent* errev) {
        if (at_shutdown || ((errev->error_code == 143) && (errev->request_code == 139))) {
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
#ifdef WIN32
          : instance(nullptr)
#endif // WIN32
#ifdef X11
          : instance(nullptr)
          , screen(0)
#endif // X11
#ifdef COCOA
          : instance(0)
#endif // COCOA
        {}

        void init (os::instance i) {
          instance = i;
#ifdef X11
          screen = DefaultScreen(instance);
          XSetErrorHandler(ErrorHandler);
          XSetIOErrorHandler(IOErrorHandler);
          set_scale_factor(calc_scale_factor());
#endif // X11
        }

        bool is_initialized () const {
#ifdef WIN32
          return (instance != nullptr);
#endif // WIN32
#ifdef X11
          return (instance != nullptr);
#endif // X11
#ifdef COCOA
          return (instance != 0);
#endif // COCOA
        }

        ~gui_init () {
#ifdef X11
          at_shutdown = true;
//          XSync(instance, False);
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

      void fini () {
#ifdef X11
        at_shutdown = true;
#endif // X11

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
#ifdef COCOA
        return 24;
#endif // COCOA
      }

      PixelFormat get_device_pixel_format () {
#ifdef WIN32
        HDC gdc = GetDC(NULL);
        int dbpp = GetDeviceCaps(gdc, BITSPIXEL);
        ReleaseDC(NULL, gdc);
        return get_pixel_format(static_cast<int>(PixelFormat(dbpp)), 1);
#endif // WIN32
#ifdef X11
        auto inst = get_instance();
        return get_pixel_format(DefaultDepth(inst, get_screen()), ImageByteOrder(inst));
#endif // X11
#ifdef COCOA
        return PixelFormat::RGB;
#endif // COCOA
      }

#ifdef WIN32
      os::key_state get_key_state () {
        return static_cast<os::key_state>((GetKeyState(VK_SHIFT) & 0x8000 ? MK_SHIFT : 0) |
                                          (GetKeyState(VK_CONTROL) & 0x8000 ? MK_CONTROL : 0) |
                                          (GetKeyState(VK_MENU) & 0x8000 ? MK_MENU : 0) |
                                          (GetKeyState(VK_LWIN) & 0x8000 ? MK_SYTEM : 0) |
                                          (GetKeyState(VK_RWIN) & 0x8000 ? MK_SYTEM : 0));
      }

      double get_scale_factor () {
        const static double scale_factor = []() {
          return 1.0;
        } ();
        return scale_factor;
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

      void set_scale_factor (double s) {
        scale_factor = s;
      }

      double get_scale_factor () {
        return scale_factor;
      }

      double calc_scale_factor () {
        const char* xscale = getenv("XSCALE");
        if (xscale) {
          double scale = 1.0;
          std::stringstream(xscale) >> scale;
          return scale;
        } else {
          Screen* screen = ScreenOfDisplay(get_instance(), get_screen());
          const char* xscale = getenv("XSCALE");
          if (xscale) {
            double scale = 1.0;
            std::stringstream(xscale) >> scale;
            return scale;
          } else {
            auto dots_w = (double)screen->width;
//          auto dots_h = (double)screen->height;
//          if ((3840 == dots_w) && (2160 == dots_h)) {
//            return 2.0;
//          }
//          auto mm_h = screen->mheight;
//          auto inch_h = (double)mm_h / 25.4;
//          auto dpi_h = dots_h / inch_h;

            auto mm_w = screen->mwidth;
            auto inch_w = (double)mm_w / 25.4;
            auto dpi_w = dots_w / inch_w;

            return round((double)dpi_w / 96.0);
//          return round((double)(dpi_w + dpi_h) / (96.0 * 2.0));
          }
        }
        return scale_factor;
      }

#endif // X11

    } // global

  } // core

} // gui

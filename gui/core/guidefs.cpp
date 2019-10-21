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
# ifdef XCB
#  include <xcb/xcb_xrm.h>
# endif // XCB
#endif
#ifdef WIN32
#include <shellscalingapi.h>
#pragma warning(disable:4996)
#endif // WIN32


// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
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

  PixelFormat get_pixel_format (int pixel_format, basepp::byte_order byte_order) {
    switch (pixel_format) {
      case 1: return PixelFormat::BW;
      case 8: return PixelFormat::GRAY;
      case 24:
        switch (byte_order) {
          case basepp::byte_order::little_endian: return PixelFormat::RGB;
          case basepp::byte_order::big_endian: return PixelFormat::BGR;
        }
      case 32:
        switch (byte_order) {
          case basepp::byte_order::little_endian: return PixelFormat::RGBA;
          case basepp::byte_order::big_endian: return PixelFormat::BGRA;
        }
    }
    return PixelFormat::Undefined;
  }

  basepp::byte_order get_pixel_format_byte_order (PixelFormat px_fmt) {
    switch (px_fmt) {
      case PixelFormat::BW:
      case PixelFormat::RGB:
      case PixelFormat::RGBA:
      case PixelFormat::ARGB:
      default:
        return basepp::byte_order::little_endian;
      case PixelFormat::GRAY:
      case PixelFormat::BGR:
      case PixelFormat::BGRA:
      case PixelFormat::ABGR:
      return basepp::byte_order::big_endian;
    }
  }

  namespace core {

    namespace global {

      double calc_scale_factor ();

      namespace {
        double scale_factor = 1.0;
      }

#ifdef X11

      namespace {
        bool at_shutdown = false;
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
        {
          XInitThreads();
        }

        void init (os::instance i) {
          instance = i;
#ifdef X11
          screen = DefaultScreen(instance);
          XSetErrorHandler(ErrorHandler);
          XSetIOErrorHandler(IOErrorHandler);
# ifdef XCB
          xcb_connection = XGetXCBConnection(instance);
# endif // XCB

#endif // X11
          set_scale_factor(calc_scale_factor());
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
# ifdef XCB
        xcb_connection_t *xcb_connection;
# endif // XCB
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
        return DefaultDepth(get_instance(), x11::get_screen());
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
        return get_pixel_format(static_cast<int>(PixelFormat(dbpp)), os::bitmap_byte_order);
#endif // WIN32
#ifdef X11
        auto inst = get_instance();
        return get_pixel_format(DefaultDepth(inst, x11::get_screen()), basepp::byte_order(ImageByteOrder(inst)));
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

      double calc_scale_factor () {
        const char* xscale = getenv("XSCALE");
        if (xscale) {
          double scale = 1.0;
          std::stringstream(xscale) >> scale;
          return scale;
        } else {
          return 1.0;//(double)GetScaleFactorForDevice(DEVICE_PRIMARY) / 100.0;
        }
      }

#endif // WIN32

      void set_scale_factor (double s) {
        scale_factor = s;
      }

      double get_scale_factor () {
        return scale_factor;
      }


#ifdef X11
      os::key_state get_key_state () {
        XkbStateRec state;
        XkbGetState(get_instance(), XkbUseCoreKbd, &state);
        return state.base_mods;
      }

      namespace x11 {
        os::x11::screen get_screen () {
          return gui_static.screen;
        }

        void set_screen (os::x11::screen screen) {
          gui_static.screen = screen;
        }

        os::x11::visual get_visual () {
          return DefaultVisual(get_instance(), get_screen());
        }
      }

# ifdef XCB
      long get_cxb_dpi_of_screen (xcb_screen_t *data) {
        const int dpi_w = static_cast<int>(static_cast<double>(data->width_in_pixels) * 25.4 / static_cast<double>(data->width_in_millimeters));
        const int dpi_h = static_cast<int>(static_cast<double>(data->height_in_pixels) * 25.4 / static_cast<double>(data->height_in_millimeters));

        return std::max(dpi_w, dpi_h);
      }

      int get_cxb_dpi () {
        long dpi = 0;
        xcb_xrm_database_t* xrm_db = xcb_xrm_database_from_default(gui_static.xcb_connection);
        if (xrm_db != NULL) {
          int i = xcb_xrm_resource_get_long(xrm_db, "Xft.dpi", NULL, &dpi);
          xcb_xrm_database_free(xrm_db);

          if (i < 0) {
            LogError << "Could not fetch value of Xft.dpi from Xresources falling back to highest dpi found";
          } else {
            LogDebug << "XCB.dpi = " << dpi;
            return dpi;
          }
        } else {
          LogError << "Could not open Xresources database falling back to highest dpi found";
        }

        for (xcb_screen_iterator_t i = xcb_setup_roots_iterator(xcb_get_setup(gui_static.xcb_connection)); i.rem; xcb_screen_next(&i)) {
          if (i.data != NULL) {
            dpi = std::max(dpi, get_cxb_dpi_of_screen(i.data));
          }
        }

         LogDebug << "XCB.dpi = " << dpi;

        return dpi;
      }
# endif // XCB

      int get_xlib_dpi_of_screen (Screen* screen) {
        const int dpi_w = static_cast<int>(static_cast<double>(screen->width) * 25.4 / static_cast<double>(screen->mwidth));
        const int dpi_h = static_cast<int>(static_cast<double>(screen->height) * 25.4 / static_cast<double>(screen->mheight));

        return std::max(dpi_w, dpi_h);
      }

      int get_xlib_dpi () {
        int dpi = 0;
        const int screen_count = ScreenCount(get_instance());
        for (int i = 0; i < screen_count; ++i) {
          auto screen = XScreenOfDisplay(get_instance(), i);
          dpi = std::max(dpi, get_xlib_dpi_of_screen(screen));
        }
        LogDebug << "X11.dpi = " << dpi;
        return dpi;
      }

      double calc_scale_factor () {
        const char* xscale = getenv("XSCALE");
        if (xscale) {
          double scale = 1.0;
          std::stringstream(xscale) >> scale;
          return scale;
        } else {
# ifdef XCB
          int dpi = std::max(get_xlib_dpi(), get_cxb_dpi());
# else
          int dpi = get_xlib_dpi();
# endif // XCB
          if (dpi == 0) {
            LogError <<  "Could get highest dpi, using 96 as default";
            dpi = 96;
          }

          return round((double)dpi / 96.0);
        }
        return scale_factor;
      }

#endif // X11

    } // global

  } // core

} // gui

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
 * @brief     C++ API: basic window types
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_X11
# include <X11/XKBlib.h>
# ifdef GUIPP_USE_XCB
#  include <xcb/xcb_xrm.h>
# endif // GUIPP_USE_XCB
# ifdef GUIPP_USE_XRANDR
#  include <X11/extensions/Xrandr.h>
# endif // GUIPP_USE_XRANDR
#elif GUIPP_WIN
//#include <shellscalingapi.h>
#pragma warning(disable:4996)
#elif GUIPP_QT
#include <QtWidgets/QApplication>
#include <QtGui/QScreen>
#include <QtWidgets/QDesktopWidget>
#endif // GUIPP_QT


// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/core/guidefs.h>
#include <gui/core/color.h>


namespace gui {

  pixel_format_t get_pixel_format (int pixel_format, core::byte_order_t byte_order_t) {
    switch (pixel_format) {
      case 1: return pixel_format_t::BW;
      case 8: return pixel_format_t::GRAY;
      case 24:
        switch (byte_order_t) {
          case core::byte_order_t::little_endian: return pixel_format_t::RGB;
          case core::byte_order_t::big_endian: return pixel_format_t::BGR;
        }
      case 32:
        switch (byte_order_t) {
          case core::byte_order_t::little_endian: return pixel_format_t::RGBA;
          case core::byte_order_t::big_endian: return pixel_format_t::BGRA;
        }
    }
    return pixel_format_t::Undefined;
  }

  core::byte_order_t get_pixel_format_byte_order (pixel_format_t px_fmt) {
    switch (px_fmt) {
      case pixel_format_t::BW:
      case pixel_format_t::RGB:
      case pixel_format_t::RGBA:
      case pixel_format_t::ARGB:
      default:
        return core::byte_order_t::little_endian;
      case pixel_format_t::GRAY:
      case pixel_format_t::BGR:
      case pixel_format_t::BGRA:
      case pixel_format_t::ABGR:
      return core::byte_order_t::big_endian;
    }
  }

  namespace core {

    namespace global {

      double calc_scale_factor ();

      namespace {
        double scale_factor = 1.0;
      }

#ifdef GUIPP_X11

      namespace {
        bool at_shutdown = false;
      }

      int ErrorHandler (Display* dpy, XErrorEvent* errev) {
        if (at_shutdown ||
//            ((errev->error_code == 143) && (errev->request_code == 139)) ||
            (errev->error_code == 3)) {  // BadWindow
          return 0;
        }

        char buffer[256];
        XGetErrorText(dpy, errev->error_code, buffer, sizeof (buffer));

        clog::fatal() << "Error occured somewhere in X!"
                    " ResourceID: " << errev->resourceid <<
                    " Serial: " << errev->serial <<
                    " Error_code: " << (int)errev->error_code <<
                    " Request_code: " << (int)errev->request_code <<
                    " Minor_code: " << (int)errev->minor_code <<
                    " Text: " << buffer;

        return 0;
      }

      int IOErrorHandler (Display*) {
        clog::fatal() << "IO Error occured somewhere in X!";
        return 0;
      }

#endif // GUIPP_X11

      struct gui_init {
        gui_init ()
          : instance(nullptr)
#ifdef GUIPP_X11
          , screen(0)
#endif // GUIPP_X11
        {
#ifdef GUIPP_X11
          XInitThreads();
#endif // GUIPP_X11
        }

        void init (gui::os::instance i) {
          instance = i;
#ifdef GUIPP_X11
          if (instance != nullptr) {
            screen = DefaultScreen(instance);
            XSetErrorHandler(ErrorHandler);
            XSetIOErrorHandler(IOErrorHandler);
          } else {
            clog::fatal() << "X server instance is 0!";
          }
# ifdef GUIPP_USE_XCB
          xcb_connection = XGetXCBConnection(instance);
# endif // GUIPP_USE_XCB

#endif // GUIPP_X11
          set_scale_factor(calc_scale_factor());
        }

        bool is_initialized () const {
          return (instance != nullptr);
        }

        ~gui_init () {
#ifdef GUIPP_X11
          at_shutdown = true;
//          XSync(instance, False);
          if (instance != nullptr) {
            XCloseDisplay(instance);
          }
          screen = 0;
#endif // GUIPP_X11
        }

        gui::os::instance instance;
#ifdef GUIPP_X11
        gui::os::x11::screen screen;
# ifdef GUIPP_USE_XCB
        xcb_connection_t *xcb_connection;
# endif // GUIPP_USE_XCB
#endif // GUIPP_X11

      };

      gui_init gui_static; // NOLINT(cert-err58-cpp)

      void init (gui::os::instance instance) {
        gui_static.init(instance);
        clog::info() << "os::bitmap_bit_order_t: " << core::os::bitmap_bit_order;
        clog::info() << "os::bitmap_byte_order_t: " << core::os::bitmap_byte_order;
        clog::info() << "os::platform_t: " << core::os::system_platform;
        clog::info() << "os::ui_t: " << core::os::system_ui;
        clog::info() << "global::scale_factor: " << scale_factor;
        clog::info() << "color::part: RGBA=" << static_cast<unsigned int>(color::part::red)
                     << ":" << static_cast<unsigned int>(color::part::green)
                     << ":" << static_cast<unsigned int>(color::part::blue)
                     << ":" << static_cast<unsigned int>(color::part::alpha);
      }

      void fini () {
#ifdef GUIPP_X11
        at_shutdown = true;
#endif // GUIPP_X11
      }

      gui::os::instance get_instance () {
        if (!gui_static.is_initialized()) {
          throw std::runtime_error("gui::core::global::init must be called before first use!");
        }
        return gui_static.instance;
      }

      void sync () {
#ifdef GUIPP_X11
        if (gui_static.is_initialized()) {
          XFlush(gui_static.instance);
        }
#endif // GUIPP_X11
      }

#ifdef GUIPP_WIN
      namespace win32 {
        int get_device_depth () {
          HDC gdc = GetDC(NULL);
          int dbpp = GetDeviceCaps(gdc, BITSPIXEL);
          ReleaseDC(NULL, gdc);
          return dbpp;
        }
      }
#endif // GUIPP_WIN

      int get_device_depth () {
        static int depth =
#ifdef GUIPP_WIN
          win32::get_device_depth();
#elif GUIPP_X11
          DefaultDepth(get_instance(), x11::get_screen());
#elif GUIPP_QT
          QGuiApplication::primaryScreen()->depth();
#else
#error  Undefined System: int get_device_depth ()
#endif
        return depth;
      }

      pixel_format_t get_device_pixel_format () {
        static pixel_format_t format =
#ifdef GUIPP_X11
          get_pixel_format(get_device_depth(), byte_order_t(ImageByteOrder(get_instance())));
#elif GUIPP_QT
          get_pixel_format(QPixmap(1, 1).depth(), os::bitmap_byte_order);
#else
          get_pixel_format(get_device_depth(), os::bitmap_byte_order);
#endif
        return format;
      }

#ifdef GUIPP_WIN
      gui::os::key_state get_key_state () {
        return static_cast<gui::os::key_state>((GetKeyState(VK_SHIFT) & 0x8000 ? MK_SHIFT : 0) |
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

#endif // GUIPP_WIN

      void set_scale_factor (double s) {
        scale_factor = s;
      }

      double get_scale_factor () {
        return scale_factor;
      }


#ifdef GUIPP_X11
      gui::os::key_state get_key_state () {
        XkbStateRec state;
        XkbGetState(get_instance(), XkbUseCoreKbd, &state);
        return state.base_mods;
      }

      namespace x11 {
        gui::os::x11::screen get_screen () {
          return gui_static.screen;
        }

        void set_screen (gui::os::x11::screen screen) {
          gui_static.screen = screen;
        }

        gui::os::x11::visual get_visual () {
          return DefaultVisual(get_instance(), get_screen());
        }
      }

# ifdef GUIPP_USE_XCB
      int get_xcb_dpi () {
        long dpi = 0;
        xcb_xrm_database_t* xrm_db = xcb_xrm_database_from_default(gui_static.xcb_connection);
        if (xrm_db != NULL) {
          int i = xcb_xrm_resource_get_long(xrm_db, "Xft.dpi", NULL, &dpi);
          xcb_xrm_database_free(xrm_db);
          clog::info() << "Xcb xrm_resource Xft.dpi: " << dpi;
        } else {
          clog::warn() << "Could not open Xresources database falling back to highest dpi found";

          for (xcb_screen_iterator_t i = xcb_setup_roots_iterator(xcb_get_setup(gui_static.xcb_connection)); i.rem; xcb_screen_next(&i)) {
            if (i.data != NULL) {
              const long xdpi = static_cast<int>((i.data->width_in_pixels * 254) / (i.data->width_in_millimeters * 10));
              const long ydpi = static_cast<int>((i.data->height_in_pixels * 254) / (i.data->height_in_millimeters * 10));

              clog::info() << "Xcb Screen " << i.index << ": "
                           << " pix WxH: " << i.data->width_in_pixels << " x " << i.data->height_in_pixels
                           << " mm WxH: " << i.data->width_in_millimeters << " x " << i.data->height_in_millimeters
                           << " X-DPI: " << xdpi << " Y-DPI: " << ydpi;

              dpi = std::max(dpi, std::max(xdpi, ydpi));
            }
          }
        }

       clog::info() << "XCB.dpi = " << dpi;

        return static_cast<int>(dpi);
      }
# endif // GUIPP_USE_XCB

# ifdef GUIPP_USE_XRANDR
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereferences"
      int get_xrandr_dpi () {
        int dpi = 0;
        auto dpy = get_instance();
        Window root = DefaultRootWindow(dpy);
        XRRScreenResources* res = XRRGetScreenResourcesCurrent(dpy, root);

        for (int i = 0; i < (res ? res->noutput : 0); i++) {
          XRROutputInfo* rroi = XRRGetOutputInfo(dpy, res, res->outputs[i]);

          if (rroi && !rroi->connection) {
            clog::info() << "Xrandr Screen " << i << " (" << rroi->name
                         << "): connection: " << rroi->connection
                         << ", WxH: " << rroi->mm_width << " x " << rroi->mm_height;

            for( int j = 0; j < rroi->ncrtc; j++ ) {
              XRRCrtcInfo* rrci = XRRGetCrtcInfo(dpy, res, res->crtcs[ j ] );
              if (rrci && rrci->noutput) {
                const int xdpi = ((int)rrci->width * 254) / ((int)rroi->mm_width * 10);
                const int ydpi = ((int)rrci->height * 254) / ((int)rroi->mm_height * 10);
                clog::info() << "Xrandr Crtc " << j << " XxY-WxH:" << rrci->x << " y " << rrci->y
                             << " - " << rrci->width << " x " << rrci->height
                             << " X-DPI: " << xdpi << " Y-DPI: " << ydpi;
                dpi = std::max(dpi, std::max(xdpi, ydpi));
              }
              XRRFreeCrtcInfo(rrci);
            }
          }
          XRRFreeOutputInfo(rroi);
        }
        XRRFreeScreenResources(res);
        clog::info() << "Xrandr.dpi = " << dpi;
        return dpi;
      }
#pragma clang diagnostic pop
# endif // GUIPP_USE_XRANDR

      int get_xlib_dpi () {
        int dpi = 96;
        const int screen_count = ScreenCount(get_instance());
        for (int i = 0; i < screen_count; ++i) {
          Screen* screen = XScreenOfDisplay(get_instance(), i);
          const int xdpi = (screen->width * 254) / (screen->mwidth * 10);
          const int ydpi = (screen->height * 254) / (screen->mheight * 10);

          clog::info() << "Xlib Screen " << i << ": "
                       << " pix WxH: " << screen->width << " x " << screen->height
                       << " mm WxH: " << screen->mwidth << " x " << screen->mheight
                       << " X-DPI: " << xdpi << " Y-DPI: " << ydpi;

          dpi = std::max(dpi, std::max(xdpi, ydpi));
        }
        clog::info() << "Xlib.dpi = " << dpi;
        return dpi;
      }

      double calc_scale_factor () {
        const char* xscale = getenv("XSCALE");
        if (xscale) {
          double scale = 1.0;
          std::stringstream(xscale) >> scale;
          return scale;
        } else {
          int dpi = get_xlib_dpi();
# ifdef GUIPP_USE_XCB
          dpi = std::max(dpi, get_xcb_dpi());
# endif // GUIPP_USE_XCB
# ifdef GUIPP_USE_XRANDR
          dpi = std::max(dpi, get_xrandr_dpi());
# endif // GUIPP_USE_XRANDR
          if (dpi == 0) {
            clog::error() <<  "Could get highest dpi, using 96 as default";
            dpi = 96;
          }

          return floor((double)dpi / 95.9);
        }
      }

#endif // GUIPP_X11

#ifdef GUIPP_QT

      gui::os::key_state get_key_state () {
        return QGuiApplication::keyboardModifiers();
      }

      double calc_scale_factor () {
        const char* xscale = getenv("XSCALE");
        if (xscale) {
          double scale = 1.0;
          std::stringstream(xscale) >> scale;
          return scale;
        } else {
          QDesktopWidget* d = QApplication::desktop();
          auto r = (double)d->logicalDpiX() / 96.0;
          clog::info()  << "Display "
                           "W:" << d->width() << ", H:" << d->height()
                        << ", MM-W:" << d->widthMM() << ", MM-H:" << d->heightMM()
                        << ", LogDPI-X:" << d->logicalDpiX() << ", LogDPI-Y:" << d->logicalDpiY()
                        << ", PhysDPI-X:" << d->physicalDpiX() << ", PhysDPI-Y:" << d->physicalDpiY()
                        << ", Pixel Ratio:" << d->devicePixelRatioF()
                        << ", Pixel Ratio Scale:" << d->devicePixelRatioFScale();
          return r;
        }
      }

#endif // GUIPP_QT

    } // global

  } // core

} // gui

namespace std {

  namespace {

    const char* pixel_format_t_names[] = {
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

    const char* orientation_t_names[] = {
      "vertical",
      "horizontal"
    };

    const char* origin_t_names[] = {
      "start",
      "center",
      "end"
    };

    const char* alignment_t_names[] = {
      "left",
      "hcenter",
      "right",
      "top",
      "vcenter",
      "bottom"
    };

  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::pixel_format_t pf) {
    return print_enum(out, pf, pixel_format_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out,gui::orientation_t o) {
    return print_enum(out, o, orientation_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out,gui::origin_t o) {
    return print_enum(out, o, origin_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out,gui::alignment_t a) {
    return print_enum(out, a, alignment_t_names);
  }

} // namespace std

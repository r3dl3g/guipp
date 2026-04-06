/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     global gui defines and typedefs
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//
# include <X11/XKBlib.h>
# ifdef GUIPP_USE_XCB
#  include <xcb/xcb_xrm.h>
# endif // GUIPP_USE_XCB
# ifdef GUIPP_USE_XRANDR
#  include <X11/extensions/Xrandr.h>
# endif // GUIPP_USE_XRANDR

#include <logging/logger.h>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/guidefs.h"
#include "gui/core/context.h"
#include "gui/core/color.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace core {

    // --------------------------------------------------------------------------
    namespace global {

      double calc_scale_factor ();

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

        logging::fatal() << "Error occured somewhere in X!"
                    " ResourceID: " << errev->resourceid <<
                    " Serial: " << errev->serial <<
                    " Error_code: " << (int)errev->error_code <<
                    " Request_code: " << (int)errev->request_code <<
                    " Minor_code: " << (int)errev->minor_code <<
                    " Text: " << buffer;

        return 0;
      }

      int IOErrorHandler (Display*) {
        logging::fatal() << "IO Error occured somewhere in X!";
        return 0;
      }

      struct gui_init {
        gui_init ()
          : instance(nullptr)
          , screen(0)
        {
          XInitThreads();
        }

        void init (gui::os::instance i) {
          instance = i;
          if (instance != nullptr) {
            screen = DefaultScreen(instance);
            XSetErrorHandler(ErrorHandler);
            XSetIOErrorHandler(IOErrorHandler);
          } else {
            logging::fatal() << "X server instance is 0!";
          }
# ifdef GUIPP_USE_XCB
          xcb_connection = XGetXCBConnection(instance);
# endif // GUIPP_USE_XCB

          set_scale_factor(calc_scale_factor());
        }

        bool is_initialized () const {
          return (instance != nullptr);
        }

        ~gui_init () {
          at_shutdown = true;
//          XSync(instance, False);
          if (instance != nullptr) {
            XCloseDisplay(instance);
          }
          screen = 0;
        }

        gui::os::instance instance;
        gui::os::x11::screen screen;
# ifdef GUIPP_USE_XCB
        xcb_connection_t *xcb_connection;
# endif // GUIPP_USE_XCB

      };

      namespace {
        gui_init gui_static;
      }
      

      void init (gui::os::instance instance) {
        gui_static.init(instance);
        info(instance);

        int major = XProtocolVersion(instance);
        int minor = XProtocolRevision(instance);
        logging::debug() << "XProtocol Version " << major << "." << minor;

        XRenderQueryVersion(instance, &major, &minor);
        logging::debug() << "XRender Version " << major << "." << minor;

#ifdef GUIPP_USE_XFT
        int revision = XftGetVersion();
        minor = revision / 100;
        major = minor / 100;
        minor = minor - major * 100;
        revision = revision - minor * 100 - major * 10000;
        logging::debug() << "Xft Version " << major << "." << minor << "." << revision;
#endif //GUIPP_USE_XFT

#ifdef GUIPP_USE_XSHM
        Bool pixmaps;
        if (!XShmQueryVersion(instance, &major, &minor, &pixmaps)) {
          logging::debug() << "No MIT-SHM available";
        } else {
          logging::debug() << "MIT-SHM Version " << major << "." << minor << (pixmaps ? " with" : " without") << " pixmaps";
        }
#endif // GUIPP_USE_XSHM
      }

      gui::os::instance get_instance () {
        if (gui_static.is_initialized()) {
          return gui_static.instance;
        }
        throw std::runtime_error("gui::core::global::init must be called before first use!");
      }

      void fini () {
        at_shutdown = true;
      }

      void sync () {
        if (gui_static.is_initialized()) {
          XFlush(gui_static.instance);
        }
      }

      int get_device_depth () {
        static int depth = DefaultDepth(get_instance(), x11::get_screen());
        return depth;
      }

      pixel_format_t get_device_pixel_format () {
        static pixel_format_t format =
          get_pixel_format(get_device_depth(), byte_order_t(ImageByteOrder(get_instance())));
        return format;
      }

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

        GUIPP_CORE_EXPORT bool has_XShm () {
          static bool has = XShmQueryExtension(get_instance());
          return has;
        }

      }

# ifdef GUIPP_USE_XCB
      int get_xcb_dpi () {
        long dpi = 0;
        xcb_xrm_database_t* xrm_db = xcb_xrm_database_from_default(gui_static.xcb_connection);
        if (xrm_db != NULL) {
          int i = xcb_xrm_resource_get_long(xrm_db, "Xft.dpi", NULL, &dpi);
          xcb_xrm_database_free(xrm_db);
          logging::debug() << "Xcb xrm_resource Xft.dpi: " << dpi;
        } else {
          logging::warn() << "Could not open Xresources database falling back to highest dpi found";

          for (xcb_screen_iterator_t i = xcb_setup_roots_iterator(xcb_get_setup(gui_static.xcb_connection)); i.rem; xcb_screen_next(&i)) {
            if (i.data != NULL) {
              const long xdpi = static_cast<int>((i.data->width_in_pixels * 254) / (i.data->width_in_millimeters * 10));
              const long ydpi = static_cast<int>((i.data->height_in_pixels * 254) / (i.data->height_in_millimeters * 10));

              logging::debug() << "Xcb Screen " << i.index << ": "
                           << " pix WxH: " << i.data->width_in_pixels << " x " << i.data->height_in_pixels
                           << " mm WxH: " << i.data->width_in_millimeters << " x " << i.data->height_in_millimeters
                           << " X-DPI: " << xdpi << " Y-DPI: " << ydpi;

              dpi = std::max(dpi, std::max(xdpi, ydpi));
            }
          }
        }

       logging::debug() << "XCB.dpi = " << dpi;

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
            logging::debug() << "Xrandr Screen " << i << " (" << rroi->name
                         << "): connection: " << rroi->connection
                         << ", WxH: " << rroi->mm_width << " x " << rroi->mm_height;

            for( int j = 0; j < rroi->ncrtc; j++ ) {
              XRRCrtcInfo* rrci = XRRGetCrtcInfo(dpy, res, res->crtcs[ j ] );
              if (rrci && rrci->noutput) {
                const int xdpi = ((int)rrci->width * 254) / ((int)rroi->mm_width * 10);
                const int ydpi = ((int)rrci->height * 254) / ((int)rroi->mm_height * 10);
                logging::debug() << "Xrandr Crtc " << j << " XxY-WxH:" << rrci->x << " y " << rrci->y
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
        logging::debug() << "Xrandr.dpi = " << dpi;
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

          logging::debug() << "Xlib Screen " << i << ": "
                       << " pix WxH: " << screen->width << " x " << screen->height
                       << " mm WxH: " << screen->mwidth << " x " << screen->mheight
                       << " X-DPI: " << xdpi << " Y-DPI: " << ydpi;

          dpi = std::max(dpi, std::max(xdpi, ydpi));
        }
        logging::debug() << "Xlib.dpi = " << dpi;
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
            logging::error() <<  "Could get highest dpi, using 96 as default";
            dpi = 96;
          }

          const auto f = floor((double)dpi * 20.0 / 96.0) / 20.0; // scale factor in 5% steps
          return  std::max(f, 0.5);
        }
      }

    } // global

  } // core

} // gui

#endif // GUIPP_X11

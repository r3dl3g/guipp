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

#ifdef GUIPP_WIN

// --------------------------------------------------------------------------
//
// Common includes
//

#pragma warning(disable:4996)

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
        gui::os::instance instance = nullptr;
      }

      void init (gui::os::instance i) {
        instance = i;
        set_scale_factor(calc_scale_factor());
        info(i);
      }

      gui::os::instance get_instance () {
        if (instance != nullptr) {
          return instance;
        }
        throw std::runtime_error("gui::core::global::init must be called before first use!");
      }

      void fini () {
      }

      void sync () {
      }

      namespace win32 {
        int get_device_depth () {
          HDC gdc = GetDC(NULL);
          int dbpp = GetDeviceCaps(gdc, BITSPIXEL);
          ReleaseDC(NULL, gdc);
          return dbpp;
        }
      }

      int get_device_depth () {
        static int depth = win32::get_device_depth();
        return depth;
      }

      pixel_format_t get_device_pixel_format () {
        static pixel_format_t format =
          get_pixel_format(get_device_depth(), os::bitmap_byte_order);
        return format;
      }

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
          HDC gdc = GetDC(NULL);
          int xsz = GetDeviceCaps(gdc, HORZSIZE);
          int ysz = GetDeviceCaps(gdc, VERTSIZE);
          int xpx = GetDeviceCaps(gdc, HORZRES);
          int ypx = GetDeviceCaps(gdc, VERTRES);
          ReleaseDC(NULL, gdc);
          return std::max(xpx * 25.4 / xsz, ypx * 25.4 / ysz) / 96.0;
        }
      }

    } // global

  } // core

} // gui

#endif // GUIPP_WIN

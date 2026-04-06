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

 #ifdef GUIPP_JS

// --------------------------------------------------------------------------
//
// Common includes
//
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
        gui::os::instance instance = emscripten::val::undefined();

        bool is_initialized () {
          return !(instance.isUndefined() || instance.isNull());
        }

      }

      void init (gui::os::instance i) {
        logging::trace() << "Call gui_init::init(is_undef:" << i.isUndefined() << ", is_null:" << i.isNull() << ")";
        instance = i;
        info(i);
        set_scale_factor(calc_scale_factor());
      }

      gui::os::instance get_instance () {
        if (is_initialized()) {
          return instance;
        }
        throw std::runtime_error("gui::core::global::init must be called before first use!");
      }

      void fini () {
      }

      void sync () {
      }

      int get_device_depth () {
        return 32;
      }

      pixel_format_t get_device_pixel_format () {
        static pixel_format_t format =
          get_pixel_format(get_device_depth(), os::bitmap_byte_order);
        return format;
      }

      double calc_scale_factor () {
        return 1.0;
      }

    } // global

  } // core

} // gui

#endif // GUIPP_JS

/**
 * @copyright (c) 2026-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     JS specific native core functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_JS

// --------------------------------------------------------------------------
//
// Common includes
//


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/core/context.h"

using namespace emscripten;

namespace gui {

  namespace core {

    namespace native {

      void init_clipping (context& ctx) {
        logging::trace() << "Save clipping";
        ctx.graphics().call<void>("save");
      }

      // --------------------------------------------------------------------------
      void set_clip_rect (context& ctx, const gui::os::rectangle& r) {
        const auto x = gui::os::get_x(r);
        const auto y = gui::os::get_y(r);
        const auto w = gui::os::get_width(r);
        const auto h = gui::os::get_height(r);
        ctx.graphics().call<void>("rect", x, y, w, h);
        ctx.graphics().call<void>("clip");
        logging::trace() << "Set clip to (" << x << " ," << y << ", " << w << " ," << h << ")";
        // ctx.graphics().set("strokeStyle", "#FF0000");
        // ctx.graphics().call<void>("strokeRect", x, y, w, h);
      }

      // --------------------------------------------------------------------------
      void clear_clipping (core::context& ctx) {
        logging::trace() << "Restore clipping";
        ctx.graphics().call<void>("restore");
      }

      // --------------------------------------------------------------------------
      gui::os::graphics create_graphics_context (gui::os::drawable id) {
        val options = val::object();
        options.set("willReadFrequently", true);
        return id.call<val>("getContext", val("2d"), options);
      }

      // --------------------------------------------------------------------------
      void delete_graphics_context (gui::os::graphics id) {
      }

    } // namespace native

  } // namespace core

} // namespace gui

#endif // GUIPP_JS

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

      // --------------------------------------------------------------------------
      void set_clip_rect (core::context& ctx, const gui::os::rectangle& r) {
        ctx.drawable().call<val>("save");
        ctx.drawable().call<val>("rect", gui::os::get_x(r), gui::os::get_y(r), gui::os::get_width(r), gui::os::get_height(r));
        ctx.drawable().call<val>("clip");
      }

      // --------------------------------------------------------------------------
      void clear_clip_rect (core::context& ctx) {
        ctx.drawable().call<val>("restore");
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

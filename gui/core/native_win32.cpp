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
 * @brief     win32 specific native core functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_WIN

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
#include "gui/core/keys.h"


namespace gui {

  namespace core {

    namespace native {

      // --------------------------------------------------------------------------
      void init_clipping (context&) {
      }

      // --------------------------------------------------------------------------
      void set_clip_rect (core::context& ctx, const gui::os::rectangle& r) {
        SelectClipRgn(ctx.graphics(), NULL);
        IntersectClipRect(ctx.graphics(), r.left, r.top, r.right, r.bottom);
      }

      // --------------------------------------------------------------------------
      void clear_clipping (core::context& ctx) {
        SelectClipRgn(ctx.graphics(), NULL);
      }

      // --------------------------------------------------------------------------
      gui::os::graphics create_graphics_context (gui::os::drawable id) {
        auto dc = GetDC(NULL);
        auto ndc = CreateCompatibleDC(dc);
        SelectObject(ndc, id);
        ReleaseDC(NULL, dc);
        return ndc;
      }

      // --------------------------------------------------------------------------
      void delete_graphics_context (gui::os::graphics id) {
        DeleteDC(id);
      }

      // --------------------------------------------------------------------------
      std::string key_symbol_to_string (gui::os::key_symbol key) {

        UINT nScanCode = MapVirtualKey(key, MAPVK_VK_TO_VSC);
        switch (key) {
        // Keys which are "extended" (except for Return which is Numeric Enter as extended)
          case VK_INSERT:
          case VK_DELETE:
          case VK_HOME:
          case VK_END:
          case VK_NEXT:  // Page down
          case VK_PRIOR: // Page up
          case VK_LEFT:
          case VK_RIGHT:
          case VK_UP:
          case VK_DOWN:
            nScanCode |= 0x100; // Add extended bit
        }

        std::vector<char> buffer(32);
        int ret = GetKeyNameText(nScanCode << 16, buffer.data(), static_cast<int>(buffer.capacity()));
        std::string str;
        if (ret) {
          str = buffer.data();
        } else {
          switch (key) {
          case core::keys::system:
            str = "Win";
            break;
          default:
            str = (char)key;
          }
        }
        return str;
      }
      
      // --------------------------------------------------------------------------
      std::string key_state_to_string (gui::os::key_state m) {
        return native::key_symbol_to_string(m);
      }

    } // namespace native

  } // namespace core

} // namespace gui

#endif // GUIPP_WIN

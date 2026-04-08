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
 * @brief     Qt specific native core functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_SDL

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


namespace gui {

  namespace core {

    namespace native {

      // --------------------------------------------------------------------------
      void init_clipping (context&) {
      }

      // --------------------------------------------------------------------------
      void set_clip_rect (core::context& ctx, const gui::os::rectangle& r) {
        SDL_RenderSetClipRect(ctx.graphics(), &r);
      }

      // --------------------------------------------------------------------------
      void clear_clipping (core::context& ctx) {
        SDL_RenderSetClipRect(ctx.graphics(), NULL);
      }

      namespace {
        SDL_Renderer* s_font_renderer = nullptr;
      }

      // --------------------------------------------------------------------------
      gui::os::graphics create_graphics_context (gui::os::drawable id) {
        if (std::holds_alternative<SDL_Window*>(id)) {
          s_font_renderer = SDL_CreateRenderer(std::get<SDL_Window*>(id), -1, SDL_RENDERER_ACCELERATED);
        } else if (std::holds_alternative<SDL_Surface*>(id)) {
          s_font_renderer = SDL_CreateSoftwareRenderer(std::get<SDL_Surface*>(id));
        } else {
          return nullptr;
        }
        SDL_RendererInfo info;
        SDL_GetRendererInfo(s_font_renderer, &info);
        logging::debug() << "SDL_Renderer driver: " << info.name;
        return s_font_renderer;
      }

      // --------------------------------------------------------------------------
      void delete_graphics_context (gui::os::graphics id) {
        if (id == s_font_renderer) {
          s_font_renderer = nullptr;
        }
        logging::trace() << "delete_graphics_context->SDL_DestroyRenderer";
        SDL_DestroyRenderer(id);
      }

      namespace sdl {
        gui::os::graphics get_font_renderer () {
          // TDB: Search a better solution!
          return s_font_renderer;
        }
      } // namespace js

      // --------------------------------------------------------------------------
      std::string key_symbol_to_string (gui::os::key_symbol key) {
        return SDL_GetKeyName(key);
      }
      
      std::ostringstream& append (std::ostringstream& os, const char* txt) {
        if (os.tellp() > 0) {
          os << ' ';
        }
        os << txt;
        return os;
      }

      // --------------------------------------------------------------------------
      std::string key_state_to_string (gui::os::key_state mod) {
        if (mod == KMOD_NONE) {
          return "None";
        }
        std::ostringstream os;

        if (mod & KMOD_CTRL)  append(os, "Ctrl");
        if (mod & KMOD_SHIFT) append(os, "Shift");
        if (mod & KMOD_ALT)   append(os, "Alt");
        if (mod & KMOD_GUI)   append(os, "Meta");
        if (mod & KMOD_CAPS)  append(os, "Caps");
        if (mod & KMOD_MODE)  append(os, "AltGr");
        if (mod & KMOD_NUM)   append(os, "NumLock");
        if (mod & KMOD_SCROLL)append(os, "ScrollLock");
        if (mod & KMOD_CAPS)  append(os, "CapsLock");

        return os.str();
      }

    } // namespace native

  } // namespace core

} // namespace gui

#endif // GUIPP_SDL

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
 * @brief     system specific window event handler function
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_SDL

// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <set>
#include <thread>

#include <logging/logger.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/window_event_proc.h"
#include "gui/win/dbg_win_message.h"
#include "gui/win/native.h"
#include "gui/win/clipboard.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {

      typedef std::pair<os::window, core::hot_key::call> hot_key_entry;
      typedef std::map<core::hot_key, hot_key_entry> hot_key_map;
      hot_key_map hot_keys;

      void install_message_filter () {}
    
      // --------------------------------------------------------------------------
      bool check_hot_key (const core::event& e) {
        if (e.type == SDL_KEYDOWN) {
          core::hot_key hk(get_key_symbol(e), get_key_state(e));
#if KEY_DEBUG
          logging::debug() << "Check hot key '" << hk.get_key_string() << "'";
#endif
          auto i = hot_keys.find(hk);
          if (i != hot_keys.end()) {
            i->second.second();
            return true;
          }
        }

        return false;
      }

    }

    namespace global {

      // --------------------------------------------------------------------------
      void register_hot_key (const core::hot_key& hk, const core::hot_key::call& fn, window* win) {
#if KEY_DEBUG
        logging::debug() << "Register hot key '" << hk.get_key_string() << "'";
#endif
        os::window root = {};
        detail::hot_keys.emplace(hk, std::make_pair(root, fn));
      }

      void unregister_hot_key (const core::hot_key& hk) {
        auto i = detail::hot_keys.find(hk);
        if (i == detail::hot_keys.end()) {
          return;
        }

        detail::hot_keys.erase(i);
      }

      std::thread::id get_current_thread_id () {
        return std::this_thread::get_id();
      }

    } // global

    bool is_button_event_outside (const window& w, const core::event& e) {
      if (e.type == SDL_MOUSEMOTION) {
        core::native_point pt = {e.motion.x, e.motion.y};
        return !w.surface_geometry().is_inside(pt);
      } else if ((e.type == SDL_MOUSEBUTTONDOWN) || (e.type == SDL_MOUSEBUTTONUP)) {
        core::native_point pt = {e.button.x, e.button.y};
        return !w.surface_geometry().is_inside(pt);
      } else if (e.type == SDL_MOUSEWHEEL) {
        core::native_point pt = {e.wheel.mouseX, e.wheel.mouseY};
        return !w.surface_geometry().is_inside(pt);
      }
      return false;
    }

    void process_event (const core::event& e, gui::os::event_result& resultValue) {

      Uint32 id = 0;
      switch (e.type) {
        case SDL_WINDOWEVENT:
          id = e.window.windowID;
        break;
        case SDL_MOUSEMOTION:
          id = e.motion.windowID;
        break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          id = e.button.windowID;
        break;
        case SDL_MOUSEWHEEL:
          id = e.wheel.windowID;
        break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          id = e.key.windowID;
        break;
        case SDL_USEREVENT:
          id = e.user.windowID;
        break;
      }

      win::overlapped_window* win = win::native::get_window(SDL_GetWindowFromID(id));
      
      if (win && win->is_valid()) {

        resultValue = 0;

        try {
          win->handle_event(e, resultValue);
        } catch (std::exception& ex) {
          logging::fatal() << "exception in run_main_loop: " << ex;
        } catch (...) {
          logging::fatal() << "Unknown exception in run_main_loop()";
        }
      }
    }

    // --------------------------------------------------------------------------
    int run_loop (volatile bool& running, const detail::filter_call& filter) {

      running = true;

      gui::os::event_result resultValue = 0;

      while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
              running = false;
          // } else if (event.type == SDL_RENDER_TARGETS_RESET) {
          //   native::x11::invalidate_window(e.window, get_expose_rect(event));
          } else {
            if (detail::check_message_filter(event) || (filter && filter(event))) {
              continue;
            } else {
              process_event(event, resultValue);
            }
          }
        }

        native::sdl::draw_invalidated_windows();

      }

      return resultValue;
    }

    namespace {
      bool main_loop_is_running = false;
    }

    int run_main_loop () {
      main_loop_is_running = true;
      return run_loop(main_loop_is_running, &detail::check_hot_key);
    }

    void quit_main_loop () {
      logging::trace() << "Received quit_main_loop()";
      main_loop_is_running = false;
      core::global::fini();
    }

    void run_on_main (const window& w, const std::function<void()>& action) {}

  }   // win
} // gui

#endif // GUIPP_SDL

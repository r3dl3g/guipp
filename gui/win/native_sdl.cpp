/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     Qt specific native system API calls
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_SDL

// --------------------------------------------------------------------------
//
// Common includes
//
#include <map>
#include <SDL_video.h>
#include <logging/logger.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/native.h"
#include "gui/win/overlapped_window.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace {
      std::map<const char*, class_info> s_window_class_info_map;
      const char* s_guipp_identifier = "GUIPP";
    }

    // --------------------------------------------------------------------------
    namespace native {

      const class_info& get_window_class (const char* class_name) {
        return s_window_class_info_map[class_name];
      }

      void register_window_class (const class_info& type) {
        if (0 == s_window_class_info_map.count(type.get_class_name())) {
          s_window_class_info_map[type.get_class_name()] = type;
        }
      }

      void register_utf8_window (const window&) {}

      void unregister_utf8_window (const window&) {}

      void set_os_window (overlapped_window* win, os::window id) {
        if (win) {
          SDL_SetWindowData(id, s_guipp_identifier, win);
        }
      }

      void unset_os_window (os::window id) {
          SDL_SetWindowData(id, s_guipp_identifier, NULL);
      }

      overlapped_window* get_window (os::window id) {
        return static_cast<overlapped_window*>(SDL_GetWindowData(id, s_guipp_identifier));
      }

      void move (os::window w, const core::point& pt) {
        const auto npt = core::global::scale_to_native(pt);
        SDL_SetWindowPosition(w, npt.x(), npt.y());
      }

      void resize (os::window w, const core::size& sz) {
        SDL_SetWindowSize(w, sz.os_width(), sz.os_height());
      }

      void geometry (os::window w, const core::rectangle& r) {
        const auto nr = core::global::scale_to_native(r);
        SDL_SetWindowPosition(w, nr.x(), nr.y());
        SDL_SetWindowSize(w, nr.width(), nr.height());
      }

      void notify_move (window& w, const core::point& pt, const core::point&) {
        const auto npt = core::global::scale_to_native(pt);
        core::event e;
        e.type = SDL_WINDOWEVENT;
        e.window.timestamp = SDL_GetTicks();
        e.window.type = SDL_WINDOWEVENT_MOVED;
        // e.window.windowID = SDL_GetWindowID(w.get_overlapped_window().get_os_window());
        e.window.data1 = npt.x();
        e.window.data2 = npt.y();
        gui::os::event_result result = 0;
        w.handle_event(e, result);
      }

      void notify_resize (window& w, const core::size& sz, const core::size&) {
        const auto nsz = core::global::scale_to_native(sz);
        core::event e;
        e.type = SDL_WINDOWEVENT;
        e.window.timestamp = SDL_GetTicks();
        e.window.type = SDL_WINDOWEVENT_RESIZED;
        // e.window.windowID = SDL_GetWindowID(w.get_overlapped_window().get_os_window());
        e.window.data1 = nsz.width();
        e.window.data2 = nsz.height();
        gui::os::event_result result = 0;
        w.handle_event(e, result);
      }

      core::native_point get_position (os::window w) {
        int x = 0, y = 0;
        SDL_GetWindowPosition(w, &x, &y);
        return {x, y};
      }

      core::size client_size (os::window id, const core::size&) {
        int w = 0, h = 0;
        SDL_GetWindowSize(id, &w, &h);
        return core::global::scale_from_native(core::native_size(w, h));
      }

      void prepare(overlapped_window&) {}

      os::window create (const class_info& type,
                         const core::rectangle& r,
                         os::window parent_id,
                         overlapped_window& data) {

                        
        const auto nr = core::global::scale_to_native(r);
        os::window id = SDL_CreateWindow("Gui++", nr.x(), nr.y(), nr.width(), nr.height(), type.get_style());
        set_os_window(&data, id);
        return id;
      }

      bool is_valid (const os::window& id) {
        return static_cast<bool>(id);
      }

      void destroy (os::window id) {
        if (id) {
          sdl::validate_window(id);
          unset_os_window(id);
          logging::trace() << "SDL_DestroyWindow";
          SDL_DestroyWindow(id);
        }
      }

      void notify_close (window& w) {
        core::event e;
        e.type = SDL_WINDOWEVENT;
        e.window.timestamp = SDL_GetTicks();
        e.window.type = SDL_WINDOWEVENT_CLOSE;
        // e.window.windowID = SDL_GetWindowID(w.get_overlapped_window().get_os_window());
        gui::os::event_result result = 0;
        w.handle_event(e, result);
      }

      void prepare_win_for_event (const overlapped_window&) {}

      bool is_visible (os::window id) {
        if (id) {
          auto flags = SDL_GetWindowFlags(id);
          return flags & SDL_WINDOW_SHOWN;
        }
        return false;
      }

      void set_visible (os::window id, bool s) {
        if (id) {
          if (s) {
            SDL_ShowWindow(id);
          } else {
            SDL_HideWindow(id);
          }
        }
      }

      void enable (overlapped_window& w, bool on) {
      }

      void to_front (os::window id) {
        SDL_RaiseWindow(id);
      }

      void to_back (os::window id) {
      }

      void take_focus (os::window id) {
        SDL_SetWindowInputFocus(id);
      }

      void set_cursor (os::window id, const os::cursor& c) {
        SDL_SetCursor(c);
      }

      void invalidate (os::window id, const core::native_rect& r) {
        sdl::invalidate_window(id, r);
      }

      void redraw (window&, os::window id, const core::native_rect&) {
      }

      void prepare_accept_focus (os::window, bool) {}

#define NO_CAPTUREx

      void capture_pointer (os::window id) {
#ifndef NO_CAPTURE
        if (id) {
          SDL_SetWindowGrab(id, SDL_TRUE);
        }
#endif // NO_CAPTURE
      }

      void uncapture_pointer (os::window id) {
#ifndef NO_CAPTURE
        if (id) {
          SDL_SetWindowGrab(id, SDL_FALSE);
        }
#endif // NO_CAPTURE
      }

      os::window get_desktop_window () {
        return NULL;
      }

      core::size screen_size () {
        SDL_Rect r;
        SDL_GetDisplayBounds(core::global::get_instance(), &r);
        return core::global::scale_from_native(core::native_size(r.w, r.h));
      }

      core::rectangle screen_area () {
        SDL_Rect r;
        SDL_GetDisplayUsableBounds(core::global::get_instance(), &r);
        return core::global::scale_from_native(core::native_rect(r.x, r.y, r.w, r.h));
      }

      core::rectangle adjust_overlapped_area (const core::rectangle& r, const class_info&) {
        return r;
      }

      core::native_point surface_to_screen (os::window id, const core::native_point& pt) {
        return pt + get_position(id);
      }

      core::native_point screen_to_surface (os::window id, const core::native_point& pt) {
        return pt - get_position(id);
      }

      void prepare_overlapped (os::window id, os::window pid) {
      }

      os::window get_overlapped_parent (os::window pid) {
        return get_desktop_window();
      }

      void set_title (os::window id, const std::string& title) {
        if (id) {
          SDL_SetWindowTitle(id, title.c_str());
        }
      }

      std::string get_title (os::window id) {
        if (id) {
          return SDL_GetWindowTitle(id);
        }
        return {};
      }

      bool is_maximized (os::window id) {
        return id && (SDL_GetWindowFlags(id) & SDL_WINDOW_MAXIMIZED);
      }

      bool is_minimized (os::window id) {
        return id && (SDL_GetWindowFlags(id) & SDL_WINDOW_MINIMIZED);
      }

      bool is_top_most (os::window id) {
        return id && (SDL_GetWindowFlags(id) & SDL_WINDOW_ALWAYS_ON_TOP);
      }

      bool is_fullscreen (os::window id) {
        return id && (SDL_GetWindowFlags(id) & SDL_WINDOW_FULLSCREEN_DESKTOP);
      }

      void minimize (os::window id) {
        if (id) {
          SDL_MinimizeWindow(id);
        }
      }

      void maximize (os::window id) {
        if (id) {
          SDL_MaximizeWindow(id);
        }
      }

      void restore (os::window id) {
        if (id) {
          SDL_RestoreWindow(id);
        }
      }

      void set_top_most (os::window id, bool on) {
        if (id) {
          SDL_SetWindowAlwaysOnTop(id, on ? SDL_TRUE : SDL_FALSE);
        }
      }

      void set_fullscreen (os::window id, bool on) {
        if (id) {
          SDL_SetWindowFullscreen(id, on ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
        }
      }

      void prepare_main_window (os::window) {}
      void prepare_popup_window (os::window) {}
      void prepare_tooltip_window (os::window) {}
      void prepare_dialog_window (os::window, os::window) {}
      void prepare_palette_window (os::window) {}

      void erase (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        SDL_SetRenderDrawColor(gc, 
                               static_cast<int>(color::extract<color::part::red>(c)),
                               static_cast<int>(color::extract<color::part::green>(c)),
                               static_cast<int>(color::extract<color::part::blue>(c)), 255);
        SDL_Rect nr{r.x(), r.y(), r.width(), r.height()};
        SDL_RenderFillRect(gc, &nr);
      }

      void frame (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        SDL_SetRenderDrawColor(gc, 
                               static_cast<int>(color::extract<color::part::red>(c)),
                               static_cast<int>(color::extract<color::part::green>(c)),
                               static_cast<int>(color::extract<color::part::blue>(c)), 255);
        SDL_Rect nr{r.x(), r.y(), r.width(), r.height()};
        SDL_RenderDrawRect(gc, &nr);
      }

      os::backstore create_surface (const core::native_size& sz, os::window id) {
        auto renderer = SDL_GetRenderer(id);
        auto fmt = SDL_GetWindowPixelFormat(id);
        return SDL_CreateTexture(renderer, fmt,  SDL_TEXTUREACCESS_TARGET, sz.width(), sz.height());
      }

      void delete_surface (os::backstore id) {
        logging::trace() << "SDL_DestroyTexture";
        SDL_DestroyTexture(id);
      }

      void send_client_message (window* win, os::message_type message, void* v1, void* v2) {
        if (win) {
          core::event e;
          e.type = SDL_USEREVENT;
          e.user.timestamp = SDL_GetTicks();
          e.user.code = message;
          // e.user.windowID = SDL_GetWindowID(win->get_overlapped_window().get_os_window());
          e.user.data1 = v1;
          e.user.data2 = v2;
          gui::os::event_result result = 0;
          win->handle_event(e, result);
        }
      }

      void send_client_message (window* win, os::message_type message, const core::rectangle& r) {
        send_client_message(win, message, static_cast<void*>(const_cast<core::rectangle*>(&r)), nullptr);
      }

      void send_client_message (window* win, os::message_type message, const float f) {
        send_client_message(win, message, const_cast<void*>(static_cast<const void*>(&f)), nullptr);
      }

      void send_client_message (window* win, os::message_type message, int l1, int l2) {
        send_client_message(win, message, static_cast<void*>(&l1), static_cast<void*>(&l2));
      }

      void send_client_message (window* win, os::message_type message, core::context& ctx, const core::native_rect& r) {
        send_client_message(win, message, const_cast<void*>(static_cast<const void*>(&ctx)), 
                                          const_cast<void*>(static_cast<const void*>(&r)));
      }
      
      void send_mouse_event (window* win, bool enter) {
        if (win) {
          core::event e;
          e.type = SDL_WINDOWEVENT;
          e.window.type = enter ? SDL_WINDOWEVENT_ENTER : SDL_WINDOWEVENT_LEAVE;
          e.window.timestamp = SDL_GetTicks();
          // e.window.windowID = SDL_GetWindowID(win->get_overlapped_window().get_os_window());
          gui::os::event_result result = 0;
          win->handle_event(e, result);
        }
      }

      void send_notify_visibility (window* win, bool visible) {
        if (win) {
          core::event e;
          e.type = SDL_WINDOWEVENT;
          e.window.type = visible ? SDL_WINDOWEVENT_SHOWN : SDL_WINDOWEVENT_HIDDEN;
          e.window.timestamp = SDL_GetTicks();
          // e.window.windowID = SDL_GetWindowID(win->get_overlapped_window().get_os_window());
          gui::os::event_result result = 0;
          win->handle_event(e, result);
        }
      }

      core::native_point get_current_pointer_pos (gui::os::window id) {
        int x = 0, y = 0;
        SDL_GetGlobalMouseState(&x, &y);
        const auto wp = get_position(id);
        return {x - wp.x(), y - wp.y()};
      }

      namespace sdl {

        typedef std::map<os::window, core::native_rect> window_rectangle_map;
        window_rectangle_map s_invalidated_windows;

        void invalidate_window (os::window id, const core::native_rect& r) {
          logging::trace() << "invalidate_window: " << id;
          if (!r.empty()) {
            auto& old = s_invalidated_windows[id];
            if (old.empty()) {
              old = r;
            } else {
              old |= r;
            }
          }
        }

        void validate_window (os::window id) {
          logging::trace() << "validate_window: " << id;
          s_invalidated_windows.erase(id);
        }

        void draw_invalidated_windows () {
          for (auto& w : s_invalidated_windows) {
            win::overlapped_window* win = get_window(w.first);
            if (win && win->is_visible()) {
              win->redraw(w.second);
            }
          }
          s_invalidated_windows.clear();
        }

      } // namespace sdl

    } // namespace native

  } // namespace win

} // namespace gui

#endif // GUIPP_SDL

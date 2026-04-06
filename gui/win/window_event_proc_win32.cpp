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

#ifdef GUIPP_WIN

// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <set>
#include <thread>

# include <windowsx.h>
#include <logging/logger.h>
#include <util/robbery.h>
#include <util/blocking_queue.h>

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


namespace util {

  namespace robbery {

#ifdef _LIBCPP_THREAD
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::__id_>;
#elif USE_MINGW
# if __MINGW_GCC_VERSION > 100000
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_M_thread>;
# else
    using thread_id = jugger<DWORD, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::mId>;
# endif
#elif WIN32
    using thread_id = jugger<_Thrd_id_t, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_Id>;
#endif // GUIPP_WIN

    thread_id::type get_native_thread_id (std::thread::id& t) {
      return t.*rob(thread_id());
    }

  } // namespace robbery

} // namespace util

namespace gui {

  namespace win {

    namespace detail {

      const os::event_id ACTION_MESSAGE = WM_USER + 0x101;
      
    } // namespace detail

    // --------------------------------------------------------------------------
    namespace detail {

      typedef std::pair<os::window, core::hot_key::call> hot_key_entry;
      typedef std::map<core::hot_key, hot_key_entry> hot_key_map;
      hot_key_map hot_keys;

#if MINGW
      static std::thread::_M_id main_thread_id;
#else
      static std::thread::id main_thread_id;
#endif

      void install_message_filter () {}

      // --------------------------------------------------------------------------
      bool check_hot_key (const core::event& e) {
        if (e.type == WM_HOTKEY) {
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
        os::window root = {};
#if KEY_DEBUG
        logging::debug() << "Register hot key '" << hk.get_key_string() << "'";
#endif
        UINT modifiers = MOD_NOREPEAT;
        if (core::control_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_CONTROL;
        }
        if (core::alt_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_ALT;
        }
        if (core::system_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_WIN;
        }
        if (core::shift_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_SHIFT;
        }
        if (win && win->is_valid()) {
          auto& o = win->get_overlapped_window();
          root = o.get_os_window();
        }
        RegisterHotKey(root, hk.get_key(), modifiers, hk.get_key());
        if (root) {
          detail::hot_keys.emplace(hk, std::make_pair(root, fn));
        } else {
          logging::warn() << "Root Windows is not yet created. Hotkey could not be registered!";
        }
      }

      void unregister_hot_key (const core::hot_key& hk) {
        auto i = detail::hot_keys.find(hk);
        if (i == detail::hot_keys.end()) {
          return;
        }

        os::window root = i->second.first;
        UnregisterHotKey(root, hk.get_key());

        detail::hot_keys.erase(i);
      }

      window* get_current_focus_window () {
        return native::get_window(GetFocus());
      }

      overlapped_window& get_application_main_window () {
        GUITHREADINFO info;
        memset(&info, 0, sizeof(info));
        info.cbSize = sizeof(GUITHREADINFO);
        if (GetGUIThreadInfo(GetCurrentThreadId(), &info)) {
          HWND win = info.hwndActive;
          if (win) {
            HWND parent = GetParent(win);
            while (parent) {
              win = parent;
              parent = parent = GetParent(win);
            }
          }
          auto* w = native::get_window(win);
          if (w) {
            return w->get_overlapped_window();
          }
        }
        throw std::runtime_error("application_main_window could not be found!");
      }

      std::thread::id get_current_thread_id () {
        return std::this_thread::get_id();
      }

    } // global

    bool is_button_event_outside (const window& w, const core::event& e) {
      switch (e.type) {
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_MBUTTONDBLCLK: {
        POINT pt = {GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam)};
        ClientToScreen(e.id, &pt);
        RECT r;
        GetWindowRect(w.get_overlapped_window().get_os_window(), &r);
        return !PtInRect(&r, pt);
      }
      }
      return false;
    }

    // --------------------------------------------------------------------------
    int run_loop (volatile bool& running, const detail::filter_call& filter) {

      running = true;

      MSG msg;
      while (running && GetMessage(&msg, nullptr, 0, 0)) {

        if (msg.message == detail::ACTION_MESSAGE) {
          std::function<void()>* action = (std::function<void()>*)msg.lParam;
          if (action && *action) {
            (*action)();
            delete action;
          }
          continue;
        }

        TranslateMessage(&msg);

        if (filter && filter(msg)) {
          continue;
        }

        DispatchMessage(&msg);
      }
      return (int)msg.wParam;
    }

    namespace {
      bool main_loop_is_running = false;
    }

    int run_main_loop () {
      main_loop_is_running = true;
      detail::main_thread_id = global::get_current_thread_id();
      return run_loop(main_loop_is_running, &detail::check_hot_key);
    }

    void quit_main_loop () {
      logging::trace() << "Received quit_main_loop()";
      main_loop_is_running = false;
      core::global::fini();
    }

    void run_on_main (const window& w, const std::function<void()>& action) {
      DWORD thread_id = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
      PostThreadMessage(thread_id,
                        detail::ACTION_MESSAGE,
                        0,
                        (ULONG_PTR)new std::function<void()>(action));
    }
  }   // win
} // gui

#endif // GUIPP_WIN

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

#ifdef GUIPP_JS

// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <set>
#include <thread>

#include <emscripten.h>
#include <emscripten/val.h>
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
        if (e.type == os::js::event_type::KeyDown) {
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
      if ((e.type >= os::js::event_type::LButtonDown) && (e.type <= os::js::event_type::MButtonDblClk)) {
        auto pt = std::get<core::native_point>(e.param_1);
        return !w.absolute_geometry().is_inside(core::global::scale_from_native(pt));
      }
      return false;
    }

    void process_event (const core::event& e, gui::os::event_result& resultValue) {
      win::overlapped_window* win = win::native::get_window(e.id);

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

      auto self = emscripten::val::global("self");
      auto queue = self["eventQueue"];
      auto id = self["canvas"];


      while (running) {
        while (queue["length"].as<int>() > 0) {

          auto event = queue.call<emscripten::val>("shift");
          auto type = static_cast<gui::os::event_id>(event["type"].as<int>());

          core::event e;
          if ((type == os::js::event_type::KeyDown) || (type == os::js::event_type::KeyUp)) {
            auto state = event["state"].as<os::key_state>();
            auto keyname = event["key"].as<std::string>();
            auto chars = event["chars"].as<std::string>();
            os::key_symbol key = gui::core::native::js::key_name_to_symbol(keyname);

            e = {id, type, state, key, chars};
            logging::trace() << "Received key event " << e << " state: " << state << " keyname: '" << keyname << "' chars: '" << chars << "'";

          } else if ((type == os::js::event_type::MouseMove) ||
                     ((static_cast<int>(type) >= static_cast<int>(os::js::event_type::ButtonDown)) && 
                      (static_cast<int>(type) <= static_cast<int>(os::js::event_type::RButtonDblClk)))) {

            auto state = event["state"].as<os::key_state>();
            auto x = event["x"].as<core::native_point::type>();
            auto y = event["y"].as<core::native_point::type>();
            e = {id, type, state, core::native_point(x, y)};
            logging::trace() << "Received event " << e << " at " << x << ", " << y << " state " << state;

          } else if ((type == os::js::event_type::WheelH) ||
                     (type == os::js::event_type::WheelV)) {

            auto state = event["state"].as<os::key_state>();
            auto x = event["x"].as<core::native_point::type>();
            auto y = event["y"].as<core::native_point::type>();
            e = {id, type, state > 0 ? 1 : -1, core::native_point(x, y)};
            logging::trace() << "Received event " << e;

          } else if ((type == os::js::event_type::MouseEnter) || (type == os::js::event_type::MouseLeave)) {

            e = {id, type, 0, 0};
            logging::trace() << "Received event " << e;

          } else if (type == os::js::event_type::Layout) {

            int x = event["x"].as<int>();
            int y = event["y"].as<int>();

            e = {id, type, core::rectangle(0, 0, x, y), 0};
            logging::trace() << "Received event " << e;

          } else if (type == os::js::event_type::Size) {

            int w = event["x"].as<int>();
            int h = event["y"].as<int>();

            e = {id, type, core::rectangle(0, 0, w, h), 0};
            logging::trace() << "Received event " << e;

          } else if (static_cast<int>(type) == 50) {

            logging::trace() << "Received paste event";

            auto chars = event["chars"].as<std::string>();
            logging::trace() << "Received chars: '" << chars << "'";

            int id = event["state"].as<int>();
            logging::trace() << "Received id: " << id;

            clipboard::handle_paste(chars, id);

            continue;

          } else {
            logging::trace() << "Received unknown " << static_cast<int>(type) << " event";
          }

          if (detail::check_message_filter(e) || (filter && filter(e))) {
            continue;
          }

          process_event(e, resultValue);
        }

        // if (x11::queued_actions.try_dequeue(action)) {
        //   action();
        // } else {
        //   wait_for_event(fd);
        // }

        // x11::draw_invalidated_windows();

        win::overlapped_window* win = native::get_window(id);
        if (win && win->is_visible()) {
          win->redraw({});
        }

        emscripten_sleep(50);

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

#endif // GUIPP_JS

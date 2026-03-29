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
 * @brief     helper for periodic actions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#if GUIPP_JS
#include <emscripten/eventloop.h>
#include <emscripten/eventloop.h>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/background_repeater.h"
#include "gui/win/window_event_proc.h"


namespace gui {

  namespace win {

    background_repeater::background_repeater (std::chrono::milliseconds delay_ms, action_t action)
      : action(action)
      , delay(delay_ms)
      , active(false)
#if GUIPP_JS
      , task(0)
#endif //GUIPP_JS
    {
      if (delay_ms < std::chrono::milliseconds(1)) {
        throw std::runtime_error("Delay can not be smaller than 1 ms");
      }
    }

    background_repeater::background_repeater (std::chrono::milliseconds delay_ms)
      : delay(delay_ms)
      , active(false)
#if GUIPP_JS
      , task(0)
#endif //GUIPP_JS
    {
      if (delay_ms < std::chrono::milliseconds(1)) {
        throw std::runtime_error("Delay can not be smaller than 1 ms");
      }
    }

    background_repeater::~background_repeater () {
      stop();
      wait_for_finish();
    }

    void background_repeater::start (action_t a) {
      action = a;
      start();
    }

#if GUIPP_JS
    void background_repeater::background_call (void* userData) {
        auto* instance = static_cast<background_repeater*>(userData);
        instance->do_action();
        instance->schedule_next();
    }

    void background_repeater::schedule_next () {
      if (is_active()) {
        logging::trace() << "Schedule background_call in " << delay.count() << " ms";
        task = emscripten_set_timeout(background_call, delay.count(), this);
      }
    }
#endif //GUIPP_JS

    void background_repeater::start () {
      if (active) {
        return;
      }
#if GUIPP_JS
      active = true;
      background_call(this);
#else
      wait_for_finish();
      task = std::thread([&] () {
        active = true;
        while (active) {
          const auto end_time = std::chrono::system_clock::now() + delay;
          do_action();
          if (active) {
            std::this_thread::sleep_until(end_time);
          }
        }
      });
#endif //GUIPP_JS
    }

    void background_repeater::do_action () {
#if GUIPP_JS
      task = 0;
#endif //GUIPP_JS
      action();
    }

    void background_repeater::stop () {
#if GUIPP_JS
      if (task > 0) {
        emscripten_clear_timeout(task);
        task = 0;
      }
#endif //GUIPP_JS
      active = false;
    }

    bool background_repeater::is_active () const {
      return active;
    }

    void background_repeater::set_delay (std::chrono::milliseconds d) {
      delay = d;
    }

    std::chrono::milliseconds background_repeater::get_delay () const {
      return delay;
    }

    void background_repeater::wait_for_finish () {
#ifndef GUIPP_JS
      active = false;
      if (task.joinable()) {
        task.join();
      }
#endif //GUIPP_JS
    }

  } // win

} // gui

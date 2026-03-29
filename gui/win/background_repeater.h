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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>
#include <chrono>
#if defined USE_MINGW && __MINGW_GCC_VERSION < 100000
#include <mingw/mingw.thread.h>
#else
#include <thread>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/gui++-win-export.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable: 4251)
#endif // WIN32

namespace gui {

  namespace win {

    class window;

    struct GUIPP_WIN_EXPORT background_repeater {
      typedef std::function<void()> action_t;

      background_repeater (std::chrono::milliseconds delay_ms, action_t action);
      background_repeater (std::chrono::milliseconds delay_ms);

      ~background_repeater ();

      void start ();
      void start (action_t action);
      void stop ();
      void wait_for_finish ();
      void do_action ();

      bool is_active () const;

      void set_delay (std::chrono::milliseconds d);

      std::chrono::milliseconds get_delay () const;

    private:

      action_t action;
      std::chrono::milliseconds delay;
      volatile bool active;

#if GUIPP_JS
      static void background_call (void* userData);

      void schedule_next ();

      volatile long task;
#else
      std::thread task;
#endif //GUIPP_JS
    };

  } // win

} // gui

#ifdef WIN32
#pragma warning(pop)
#endif // WIN32

/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: separator view
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>
#include <chrono>
#ifdef USE_MINGW
#include <mingw/mingw.thread.h>
#else
#include <thread>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/gui++-win-export.h>


namespace gui {

  namespace win {

    class window;

    struct GUIPP_WIN_EXPORT background_repeater {
      typedef std::function<void()> action_t;

      background_repeater (window& win, std::chrono::milliseconds delay_ms, action_t action);
      background_repeater (window& win, std::chrono::milliseconds delay_ms);

      ~background_repeater ();

      void start ();
      void start (action_t action);
      void stop ();
      void wait_for_finish ();

      void set_delay (std::chrono::milliseconds d);

      std::chrono::milliseconds get_delay () const;

    private:
      window& win;
      std::thread task;
      std::chrono::milliseconds delay;
      action_t action;
      volatile bool active;
    };

  } // win

} // gui

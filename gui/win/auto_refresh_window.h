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
// Library includes
//
#include <gui/win/background_repeater.h>
#include <gui/win/window.h>


namespace gui {

  namespace win {

    template<int delay_ms>
    struct GUIPP_WIN_EXPORT auto_refresh_window : public client_window<color::transparent> {
      typedef client_window<color::transparent> super;

      auto_refresh_window ()
        : refresher(*this, std::chrono::milliseconds(delay_ms), [&] () { invalidate(); })
      {}

      auto_refresh_window (background_repeater::action_t action)
        : refresher(*this, std::chrono::milliseconds(delay_ms), action)
      {}

      void start_auto_refresh () {
        refresher.start();
        invalidate();
      }

      void stop_auto_refresh () {
        refresher.stop();
        invalidate();
      }

      void activate (bool on) {
        if (on) {
          start_auto_refresh();
        } else {
          stop_auto_refresh();
        }
      }

    protected:
      background_repeater refresher;
    };

  } // win

} // gui

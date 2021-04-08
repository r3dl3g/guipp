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
#include <vector>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/background_repeater.h>
#include <gui/core/point.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    struct animation {
      typedef std::function<void(float)> animation_t;
      typedef std::function<void()> finish_t;

      animation (window& win, std::chrono::milliseconds duration, int fps = 50, float f0 = 0, float f1 = 1);

      void start (finish_t fin);
      void stop ();

      void add (const animation_t& a);
      void add (animation_t&& a);

      void clear ();

    private:
      float f0;
      float delta;
      float step;
      std::chrono::milliseconds duration;
      background_repeater repeater;
      std::vector<animation_t> animations;
    };

    // --------------------------------------------------------------------------
    typedef std::shared_ptr<win::window> window_ptr;

    // --------------------------------------------------------------------------
    struct move_animation {

      move_animation (const window_ptr& win, const core::point& from, const core::point& to);

      void operator () (float f);

    protected:
      window_ptr win;
      core::point from;
      core::point diff;
    };

    // --------------------------------------------------------------------------
    struct size_animation {

      size_animation (const window_ptr& win, const core::size& from, const core::size& to);

      void operator () (float f);

    protected:
      window_ptr win;
      core::size from;
      core::size diff;
    };

    // --------------------------------------------------------------------------
  } // win

} // gui

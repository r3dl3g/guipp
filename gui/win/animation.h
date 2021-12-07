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
 * @brief     basic window animation
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
#include <vector>
#include <memory>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/background_repeater.h>
#include <gui/core/rectangle.h>


namespace gui {

  namespace win {

    namespace animation {

      // --------------------------------------------------------------------------
      struct GUIPP_WIN_EXPORT engine {
        typedef std::function<void(float)> animation_t;
        typedef std::function<void()> finish_t;

        engine (window& win, std::chrono::milliseconds duration, int fps = 50, float f0 = 0, float f1 = 1);

        void start (finish_t fin);
        void stop ();

        void add (const animation_t& a);
        void add (animation_t&& a);

        void clear ();
        void disable_animation (bool disable);

      private:
        float f0;
        float delta;
        float step;
        std::chrono::milliseconds duration;
        background_repeater repeater;
        std::vector<animation_t> animations;
        bool animation_disabled;
      };

      // --------------------------------------------------------------------------
      typedef std::shared_ptr<win::window> window_ptr;

      // --------------------------------------------------------------------------
      struct GUIPP_WIN_EXPORT move {

        move (const window_ptr& win, const core::point& from, const core::point& to);

        void operator () (float f);

      protected:
        window_ptr win;
        core::point from;
        core::point diff;
      };

      // --------------------------------------------------------------------------
      struct GUIPP_WIN_EXPORT size {

        size (const window_ptr& win, const core::size& from, const core::size& to);

        void operator () (float f);

      protected:
        window_ptr win;
        core::size from;
        core::size diff;
      };

      // --------------------------------------------------------------------------
      template<typename O, typename I = O>
      struct dual {
        dual (O&& out, I&& in)
          : out(std::move(out))
          , in(std::move(in))
        {}

        void operator () (float f) {
          out(f);
          in(f);
        }

      protected:
        O out;
        I in;
      };

      // --------------------------------------------------------------------------
      enum class position {
        left,
        right,
        top,
        bottom
      };

      // --------------------------------------------------------------------------
      template<position T>
      core::point in_start_point (const core::rectangle& r);

      template<>
      inline core::point in_start_point<position::left> (const core::rectangle& r) {
        return r.top_left().dx(-r.width());
      }

      template<>
      inline core::point in_start_point<position::right> (const core::rectangle& r) {
        return r.top_right();
      }

      template<>
      inline core::point in_start_point<position::top> (const core::rectangle& r) {
        return r.top_left().dy(-r.height());
      }

      template<>
      inline core::point in_start_point<position::bottom> (const core::rectangle& r) {
        return r.bottom_left();
      }

      // --------------------------------------------------------------------------
      template<position T>
      core::point out_end_point (const core::rectangle& r);

      template<>
      inline core::point out_end_point<position::left> (const core::rectangle& r) {
        return r.top_right();
      }

      template<>
      inline core::point out_end_point<position::right> (const core::rectangle& r) {
        return r.top_left().dx(-r.width() / 10);
      }

      template<>
      inline core::point out_end_point<position::top> (const core::rectangle& r) {
        return r.bottom_left();
      }

      template<>
      inline core::point out_end_point<position::bottom> (const core::rectangle& r) {
        return r.top_left().dy(-r.height() / 10);
      }

      // --------------------------------------------------------------------------
      template<position T>
      dual<move> push_from (const window_ptr& out,
                            const window_ptr& in,
                            const core::rectangle& r) {
        return {{out, r.top_left(), out_end_point<T>(r)},
          {in, in_start_point<T>(r), r.top_left()}};
      }
      // --------------------------------------------------------------------------
      template<position T>
      dual<move> pop_to (const window_ptr& out,
                         const window_ptr& in,
                         const core::rectangle& r) {
        return {{out, r.top_left(), in_start_point<T>(r)},
          {in, out_end_point<T>(r), r.top_left()}};
      }

    } // namespace animation
    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

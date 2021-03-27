/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: brush definition
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/icons.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT centerline : public detail::icon_base {

      centerline (const core::point& center, core::size::type radius, const core::angle& angle);

      void operator() (graphics& g, const pen& pn) const;

    protected:
      const core::angle angle;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT centerline_section : public centerline {

      centerline_section (const core::point& center, core::size::type radius, core::size::type radius2, const core::angle& angle);

      void operator() (graphics& g, const pen& pn) const;

    protected:
      core::size::type radius2;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT centerlines {

      centerlines (const core::point& center, core::size::type radius, core::size::type radius2,
                   const core::angle& step, const core::angle& start = 0, const core::angle& end = 360);

      void operator() (graphics& g, const pen& pn) const;

    protected:
      const core::point center;
      const core::size::type radius;
      const core::size::type radius2;
      const core::angle step;
      const core::angle start;
      const core::angle end;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clock_minutes : public centerlines {

      clock_minutes (const core::point& center, core::size::type radius);

    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clock_hours : public centerlines {

      clock_hours (const core::point& center, core::size::type radius);

    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clock_numbers : public detail::icon_base {

      clock_numbers (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const font& font, gui::os::color color) const;

    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clock_face : public detail::icon_base {

      clock_face (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const font& font, gui::os::color color) const;

    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clock_hands : public detail::icon_base {

      clock_hands (const core::point& center, core::size::type radius,
                   int hour, int min, int sec,
                   gui::os::color second_color = color::orange);

      void operator() (graphics& g, const pen& pn) const;

    private:
      gui::os::color second_color;
      int hour;
      int min;
      int sec;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clock_simple_hands : public detail::icon_base {

      clock_simple_hands (const core::point& center, core::size::type radius,
                          int hour, int min);

      void operator() (graphics& g, const pen& pn) const;

    private:
      int hour;
      int min;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT stopwatch_seconds_face : public detail::icon_base {

      stopwatch_seconds_face (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const font& font, gui::os::color color) const;

    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT stopwatch_minutes_face : public detail::icon_base {

      stopwatch_minutes_face (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const font& font, gui::os::color color) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT stopwatch_digits : public detail::icon_base {

      stopwatch_digits (const core::point& center, core::size::type radius,
                                std::chrono::milliseconds duration);

      void operator() (graphics& g, const font& font, gui::os::color color) const;

    private:
      std::chrono::milliseconds duration;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT stopwatch_seconds_hands : public detail::icon_base {

      stopwatch_seconds_hands (const core::point& center, core::size::type radius,
                              std::chrono::milliseconds duration);

      void operator() (graphics& g, const pen& pn) const;

    private:
      std::chrono::milliseconds duration;

    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT stopwatch_minutes_hands : public detail::icon_base {

      stopwatch_minutes_hands (const core::point& center, core::size::type radius,
                              std::chrono::milliseconds duration);

      void operator() (graphics& g, const pen& pn) const;

    private:
      std::chrono::milliseconds duration;

    };
    // --------------------------------------------------------------------------
  } //namespace draw

} //namespace gui

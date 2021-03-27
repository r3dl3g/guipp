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
#include <gui/draw/drawers.h>
#include <gui++-draw-export.h>


namespace gui {

  namespace draw {

    namespace detail {

      // --------------------------------------------------------------------------
      struct GUIPP_DRAW_EXPORT icon_base {

        icon_base (const core::point& center, core::size::type radius);

      protected:
        const core::point center;
        const core::size::type radius;
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    GUIPP_DRAW_EXPORT core::point calc_clock_point (const core::point& center,
                                                    const core::angle& clock_angle,
                                                    core::size::type radius);

    GUIPP_DRAW_EXPORT core::point calc_point_between (const core::point& p0,
                                                      const core::point& p1,
                                                      double f);

    GUIPP_DRAW_EXPORT draw::line calc_centerline (const core::point& center,
                                                  const core::angle& clock_angle,
                                                  core::size::type radius);

    GUIPP_DRAW_EXPORT draw::line calc_centerline (const core::point& center,
                                                  const core::angle& clock_angle,
                                                  core::size::type radius0,
                                                  core::size::type radius1);

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT up_icon : public detail::icon_base {

      up_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT down_icon : public detail::icon_base {

      down_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT left_icon : public detail::icon_base {

      left_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT right_icon : public detail::icon_base {

      right_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clock_icon : public detail::icon_base {

      clock_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT stopwatch_icon : public detail::icon_base {

      stopwatch_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT timer_icon : public detail::icon_base {

      timer_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT play_icon : public detail::icon_base {

      play_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT back_icon : public detail::icon_base {

      back_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT pause_icon : public detail::icon_base {

      pause_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT stop_icon : public detail::icon_base {

      stop_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT reset_icon : public detail::icon_base {

      reset_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT add_icon : public detail::icon_base {

      add_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT remove_icon : public detail::icon_base {

      remove_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT cancel_icon : public detail::icon_base {

      cancel_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT okay_icon : public detail::icon_base {

      okay_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT close_icon : public detail::icon_base {

      close_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT menu_icon : public detail::icon_base {

      menu_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clear_icon : public detail::icon_base {

      clear_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT find_icon : public detail::icon_base {

      find_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT undo_icon : public detail::icon_base {

      undo_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT redo_icon : public detail::icon_base {

      redo_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT info_icon : public detail::icon_base {

      info_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT important_icon : public detail::icon_base {

      important_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT sync_icon : public detail::icon_base {

      sync_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT restart_icon : public detail::icon_base {

      restart_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT zoom_in_icon : public detail::icon_base {

      zoom_in_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT zoom_out_icon : public detail::icon_base {

      zoom_out_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };
    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT off_icon : public detail::icon_base {

      off_icon (const core::point& center, core::size::type radius);

      void operator() (graphics& g, const pen& pn) const;

    };

    // --------------------------------------------------------------------------
  } //namespace draw

} //namespace gui

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
#include <gui/draw/gui++-draw-export.h>


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
    enum class icon_t {
      up, down, left, right,
      remove, add, close, open,
      check_off, check_on, radio_off, radio_on,
      undo, redo, forward, backward,
      okay, cancel, cross, restart,
      off, clear, sync, reset,
      play, pause, stop, back,
      find, zoom_in, zoom_out, menu,
      clock, stopwatch, timer, settings,
      info, important, person, trash,
      file, folder, folder_open
    };

    // --------------------------------------------------------------------------
    template<icon_t I>
    void draw_icon (graphics& g, const pen& pn, const core::point& center, core::size::type radius);

    // --------------------------------------------------------------------------
    template<icon_t I>
    struct icon : public detail::icon_base {
      typedef detail::icon_base super;

      inline icon (const core::point& center, core::size::type radius)
        : super(center, radius)
      {}

      inline void operator() (graphics& g, const pen& pn) const {
        draw_icon<I>(g, pn, center, radius);
      }

    };

    // --------------------------------------------------------------------------
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::up> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::down> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::left> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::right> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::add> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::remove> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::open> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::close> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::check_off> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::check_on> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::radio_off> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::radio_on> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::undo> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::redo> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::forward> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::backward> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::okay> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::cancel> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::cross> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::restart> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::off> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::clear> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::sync> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::reset> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::play> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::pause> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::stop> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::back> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::find> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::zoom_in> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::zoom_out> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::menu> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::clock> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::stopwatch> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::timer> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::info> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::important> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::person> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::trash> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::settings> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::file> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::folder> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_t::folder_open> (graphics&, const pen&, const core::point&, core::size::type);

    // --------------------------------------------------------------------------
  } //namespace draw

} //namespace gui

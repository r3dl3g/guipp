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
 * @brief     icon drawings
 *
 * @license   MIT license. See accompanying file LICENSE.
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
                                                    core::size::type radius,
                                                    const core::angle& clock_angle);

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
    enum class icon_type {
      none,
      up, down, left, right,
      up_arrow, down_arrow, left_arrow, right_arrow,
      remove, add, close, open,
      check_off, check_on, radio_off, radio_on,
      undo, redo, forward, backward,
      okay, cancel, cross, restart,
      off, clear, sync, reset,
      play, pause, stop, back,
      fast_forwad, fast_back, first, last,
      find, zoom_in, zoom_out, menu,
      clock, stopwatch, timer, settings, settings2,
      info, important, person, group,
      trash, file, folder, folder_open,
      list, grid, columns,
      lock, unlock,
      north, east, south, west,
      north_east,north_west, south_east, south_west,
      new_blink, new_folder, new_file,
      fullscreen, restore,
      background,
      MAX
    };

    GUIPP_DRAW_EXPORT const std::string& icon_name (icon_type);

    // --------------------------------------------------------------------------
    template<icon_type I>
    void draw_icon (graphics& g, const pen& pn, const core::point& center, core::size::type radius);

    // --------------------------------------------------------------------------
    template<icon_type I>
    struct icon_t : public detail::icon_base {
      typedef detail::icon_base super;

      inline icon_t (const core::point& center, core::size::type radius)
        : super(center, radius)
      {}

      inline void operator() (graphics& g, const pen& pn) const {
        draw_icon<I>(g, pn, center, radius);
      }

    };

    // --------------------------------------------------------------------------
    GUIPP_DRAW_EXPORT void draw_icon (graphics& g, icon_type type, const pen& pn, const core::point& center, core::size::type radius);

    // --------------------------------------------------------------------------
    struct icon : public detail::icon_base {
      typedef detail::icon_base super;

      inline icon (icon_type type, const core::point& center, core::size::type radius)
        : super(center, radius)
        , type(type)
      {}

      inline void operator() (graphics& g, const pen& pn) const {
        draw_icon(g, type, pn, center, radius);
      }

    private:
      icon_type type;
    };

    // --------------------------------------------------------------------------
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::none> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::up> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::down> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::left> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::right> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::up_arrow> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::down_arrow> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::left_arrow> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::right_arrow> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::add> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::remove> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::open> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::close> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::check_off> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::check_on> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::radio_off> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::radio_on> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::undo> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::redo> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::forward> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::backward> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::okay> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::cancel> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::cross> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::restart> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::off> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::clear> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::sync> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::reset> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::play> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::pause> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::stop> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::back> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::fast_forwad> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::fast_back> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::first> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::last> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::find> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::zoom_in> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::zoom_out> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::menu> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::clock> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::stopwatch> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::timer> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::info> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::important> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::person> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::group> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::trash> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::settings> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::settings2> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::file> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::folder> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::folder_open> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::list> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::grid> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::columns> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::lock> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::unlock> (graphics&, const pen&, const core::point&, core::size::type);

    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::north> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::east> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::south> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::west> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::north_east> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::north_west> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::south_east> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::south_west> (graphics&, const pen&, const core::point&, core::size::type);

    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::new_blink> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::new_folder> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::new_file> (graphics&, const pen&, const core::point&, core::size::type);

    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::fullscreen> (graphics&, const pen&, const core::point&, core::size::type);
    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::restore> (graphics&, const pen&, const core::point&, core::size::type);

    template<> GUIPP_DRAW_EXPORT void draw_icon<icon_type::background> (graphics&, const pen&, const core::point&, core::size::type);
    // --------------------------------------------------------------------------
  } //namespace draw

} //namespace gui

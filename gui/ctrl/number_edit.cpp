/**
 * @copyright (c) 2016-2022 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     pasive separator view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/number_edit.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    date_time_edit_base::date_time_edit_base (const time_point& v)
      : parts(util::time::local_time(v))
      , year(parts.tm_year)
      , month(parts.tm_mon)
      , day(parts.tm_mday)
      , hour(parts.tm_hour)
      , min(parts.tm_min)
      , sec(parts.tm_sec)
    {
      get_layout().add({year, month, day, hour, min, sec});
      on_create([&] () {
        year.create(*this);
        month.create(*this);
        day.create(*this);
        hour.create(*this);
        min.create(*this);
        sec.create(*this);
      });
    }

    // --------------------------------------------------------------------------
    duration_edit_base::duration_edit_base (const duration& v)
      : parts(util::time::duration2parts(v))
      , hour(parts.hours)
      , min(parts.mins)
      , sec(parts.secs)
    {
      get_layout().add({hour, min, sec});
      on_create([&] () {
        hour.create(*this);
        min.create(*this);
        sec.create(*this);
      });
    }

    // --------------------------------------------------------------------------
  } // win

} // gui

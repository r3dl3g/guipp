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
    date_time_edit::date_time_edit (const util::time::time_point& v)
      : date_time_edit(edit_data::copy(v))
    {}

    date_time_edit::date_time_edit (std::reference_wrapper<util::time::time_point> v)
      : date_time_edit(edit_data::ref(v.get()))
    {}

    date_time_edit::date_time_edit (data d)
      : value(d)
      , date(util::time::local_time(value()))
      , year(edit_data::ref(date.tm_year))
      , month(edit_data::ref(date.tm_mon))
      , day(edit_data::ref(date.tm_mday))
      , hour(edit_data::ref(date.tm_hour))
      , min(edit_data::ref(date.tm_min))
      , sec(edit_data::ref(date.tm_sec))
    {
      init();
    }

    void date_time_edit::init () {
      get_layout().add({year, month, day, hour, min, sec});
      on_create([&] () {
        year.create(*this);
        month.create(*this);
        day.create(*this);
        hour.create(*this);
        min.create(*this);
        sec.create(*this);
      });

      year.edit.on_content_changed([&] () { update(); });
      month.edit.on_content_changed([&] () { update(); });
      day.edit.on_content_changed([&] () { update(); });
      hour.edit.on_content_changed([&] () { update(); });
      min.edit.on_content_changed([&] () { update(); });
      sec.edit.on_content_changed([&] () { update(); });
    }

    void date_time_edit::update () {
      std::mktime(&date);
      set(util::time::time_t2time_point(util::time::tm2time_t(date)));
    }

    void date_time_edit::set (time_point v) {
      if (value() != v) {
        value() = v;
        date = util::time::local_time(get());
        invalidate();
        notify_content_changed();
      }
    }

    auto date_time_edit::get () const -> time_point {
      return value();
    }

    // --------------------------------------------------------------------------
    duration_edit::duration_edit (const duration& v)
      : duration_edit(edit_data::copy(v))
    {}

    duration_edit::duration_edit (std::reference_wrapper<duration> v)
      : duration_edit(edit_data::ref(v.get()))
    {}

    duration_edit::duration_edit (data d)
      : value(d)
      , parts(util::time::duration2parts(value()))
      , hour(edit_data::ref(parts.hours))
      , min(edit_data::ref(parts.mins))
      , sec(edit_data::ref(parts.secs))
    {
      get_layout().add({hour, min, sec});
      on_create([&] () {
        hour.create(*this);
        min.create(*this);
        sec.create(*this);
      });

      hour.edit.on_content_changed([&] () { update(); });
      min.edit.on_content_changed([&] () { update(); });
      sec.edit.on_content_changed([&] () { update(); });
    }

    void duration_edit::update () {
      set(util::time::parts2duration(parts));
    }

    void duration_edit::set (duration v) {
      if (value() != v) {
        value() = v;
        parts = util::time::duration2parts(v);
        invalidate();
        notify_content_changed();
      }
    }

    auto duration_edit::get () const -> duration {
      return value();
    }

    // --------------------------------------------------------------------------
    duration_edit_ms::duration_edit_ms (const duration& v)
      : duration_edit_ms(edit_data::copy(v))
    {}

    duration_edit_ms::duration_edit_ms (std::reference_wrapper<duration> v)
      : duration_edit_ms(edit_data::ref(v.get()))
    {}

    duration_edit_ms::duration_edit_ms (data d)
      : super(d)
      , micros(edit_data::ref(parts.micros))
    {
      get_layout().add(micros);
      on_create([&] () {
        micros.create(*this);
      });
      micros.edit.on_content_changed([&] () { update(); });
    }
    // --------------------------------------------------------------------------
  } // win

} // gui

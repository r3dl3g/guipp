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
 * @brief     stack view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename T>
    date_time_header_line<T>::date_time_header_line (date_time_edit<T>& edit)
      : dte(edit)
    {
      day_button.on_clicked([&] () { set_date(); });
      time_button.on_clicked([&] () { set_time(); });
      add({header, day_button, time_button});
    }

    template<typename T>
    date_time_header_line<T>::date_time_header_line (const std::string& th,
                                                     date_time_edit<T>& edit,
                                                     const std::string& today,
                                                     const std::string& now)
      : date_time_header_line(edit)
    {
      set_labels(th, today, now);
    }

    template<typename T>
    void date_time_header_line<T>::set_labels (const std::string& th,
                                               const std::string& today,
                                               const std::string& now) {
      header.set_text(th);
      day_button.set_text(today);
      time_button.set_text(now);
    }
    template<typename T>
    void date_time_header_line<T>::set_date () {
      using namespace util::time;
      std::tm t = local_time(dte.get());
      std::tm now = local_time_now();
      dte.set(mktime_point(year_of(now), month_of(now), day_of(now), t.tm_hour, t.tm_min, t.tm_sec));
    }

    template<typename T>
    void date_time_header_line<T>::set_time () {
      using namespace util::time;
      std::tm t = local_time(dte.get());
      std::tm now = local_time_now();
      dte.set(mktime_point(year_of(t), month_of(t), day_of(t), now.tm_hour, now.tm_min, now.tm_sec));
    }

  } // ctrl

} // gui

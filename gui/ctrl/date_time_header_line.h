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


// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/number_edit.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/adaption_layout.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename T = util::time::time_point>
    class date_time_header_line : public win::group_window<layout::horizontal_adaption<>> {
    public:
      typedef win::group_window<layout::horizontal_adaption<>> super;

      date_time_header_line (date_time_edit<T>& dte);

      date_time_header_line (const std::string& th,
                             date_time_edit<T>& dte,
                             const std::string& today = "Today",
                             const std::string& now = "Now");

      void set_labels (const std::string& th,
                       const std::string& today = "Today",
                       const std::string& now = "Now");

      void set_date ();

      void set_time ();

      date_time_edit<T>& dte;
      label header;
      text_button day_button;
      text_button time_button;
    };

  } // ctrl

} // gui

#include <gui/ctrl/date_time_header_line.inl>

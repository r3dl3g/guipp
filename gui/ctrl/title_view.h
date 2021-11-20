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
 * @brief     C++ API: image view
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/layout_container.h>
#include <gui/layout/dynamic_border_layout.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/label.h>

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT title_view : public
        win::group_window<layout::dynamic_border_layout<layout::border_layout_type::left_right_maximize, 5>, float, float, float, float> {
      typedef win::group_window<layout::dynamic_border_layout<layout::border_layout_type::left_right_maximize, 5>, float, float, float, float> super;

      title_view (int back_size = 40);

      void set_title (const std::string& title);

      void enable_back (bool b);
      void on_back (std::function<notification_fn>&& fn);

      void set_right_view (window& right, core::size::type width);
      void clear_right_view ();

      void set_foreground (os::color);
      void set_background (os::color);
      void set_fore_and_background (os::color fg, os::color bg);

    private:
      icon_push_button_t<draw::icon_type::left_arrow> back_btn;
      label_center title_label;
    };

    // --------------------------------------------------------------------------
  } // ctrl

} // gui

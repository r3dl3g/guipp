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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/title_view.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    title_view::title_view (int back_size)
      : super(0, 0, back_size, 0)
    {
      get_layout().set_center(layout::lay(title_label));
      get_layout().set_left(layout::lay(back_btn));
      on_create([&] () {
        back_btn.create(*this);
        back_btn.set_visible(false);
        title_label.create(*this);
      });
    }

    void title_view::set_title (const std::string& title) {
      title_label.set_text(title);
    }

    void title_view::enable_back (bool b) {
      back_btn.enable(b);
    }

    void title_view::on_back (std::function<notification_fn>&& fn) {
      back_btn.on_clicked(std::move(fn));
    }

    void title_view::set_right_view (window& right, core::size::type width = 0) {
      right.create(right.get_window_class(), *this);
      right.set_visible();
      get_layout().set_right(layout::lay(right));
      get_layout().set_right_width(width);
      layout();
    }

    void title_view::clear_right_view () {
      get_layout().set_right(nullptr);
      get_layout().set_right_width(0);
      layout();
    }

  } // ctrl

} // gui

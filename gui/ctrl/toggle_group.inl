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
 * @brief     C++ API: toggle group
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    toggle_group<O, FG, BG, B, L>::~toggle_group () {
      buttons.clear();
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::add_buttons (const std::initializer_list<std::string>& labels) {
      for (const std::string& label : labels) {
        add_button(label);
      }
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    inline void toggle_group<O, FG, BG, B, L>::add_button (const std::string& label) {
      add_button(const_text(label));
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    inline void toggle_group<O, FG, BG, B, L>::add_button (const text_source& label) {
      button_type b = std::make_shared<B>(label);
      b->on_clicked([&, b] () {
        uncheck_buttons(b);
      });
      buttons.push_back(std::move(b));
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    inline auto toggle_group<O, FG, BG, B, L>::get_button (int idx) -> button_type& {
      return buttons[idx];
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    inline auto toggle_group<O, FG, BG, B, L>::get_button (int idx) const -> const button_type& {
      return buttons[idx];
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::enable (bool on) {
      for (auto& b : buttons) {
        b->enable(on);
      }
      super::enable(on);
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    inline void toggle_group<O, FG, BG, B, L>::disable () {
      enable(false);
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::create (win::container& parent,
                                                const core::rectangle& place) {
      super::create(parent, place);
      for (auto& b : buttons) {
        b->create(*this);
        super::get_layout().add([b] (const core::rectangle& r) { b->place(r); });
      }
      super::set_children_visible();
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::uncheck_buttons (button_type except) {
      for (auto b : buttons) {
        if (b != except) {
          b->set_checked(false);
        }
      }
    }

  } // ctrl

} // gui

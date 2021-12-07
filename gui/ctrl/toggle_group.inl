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
 * @brief     toggled button group control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace ctrl {

    template<orientation_t O, typename B, typename L>
    toggle_group<O, B, L>::toggle_group ()
      : selection(-1)
    {
      super::on_create([&] () {
        for (auto& b : buttons) {
          b->create(*this);
        }
      });
    }

    template<orientation_t O, typename B, typename L>
    toggle_group<O, B, L>::~toggle_group () {
      buttons.clear();
    }

    template<orientation_t O, typename B, typename L>
    inline void toggle_group<O, B, L>::add_button (button_type btn) {
      btn->on_clicked([&, btn] () {
        uncheck_buttons(btn);
      });
      super::get_layout().add(gui::layout::lay(btn.get()));
      btn->set_background(super::get_background());
      btn->set_foreground(get_foreground());
      buttons.push_back(std::move(btn));
    }

    template<orientation_t O, typename B, typename L>
    inline void toggle_group<O, B, L>::add_button (const text_source& label) {
      add_button(std::make_shared<B>(label));
    }

    template<orientation_t O, typename B, typename L>
    inline void toggle_group<O, B, L>::add_button (const std::string& label) {
      add_button(const_text(label));
    }

    template<orientation_t O, typename B, typename L>
    void toggle_group<O, B, L>::add_buttons (std::initializer_list<button_type> buttons) {
      for (button_type button : buttons) {
        add_button(std::move(button));
      }
    }

    template<orientation_t O, typename B, typename L>
    void toggle_group<O, B, L>::add_buttons (std::initializer_list<const_text> labels) {
      for (const const_text& label : labels) {
        add_button(label);
      }
    }

    template<orientation_t O, typename B, typename L>
    inline std::size_t toggle_group<O, B, L>::count () const {
      return buttons.size();
    }

    template<orientation_t O, typename B, typename L>
    inline auto toggle_group<O, B, L>::get_button (int idx) -> button_type& {
      return buttons[idx];
    }

    template<orientation_t O, typename B, typename L>
    inline auto toggle_group<O, B, L>::get_button (int idx) const -> const button_type& {
      return buttons[idx];
    }

    template<orientation_t O, typename B, typename L>
    void toggle_group<O, B, L>::enable (bool on) {
      for (auto& b : buttons) {
        b->enable(on);
      }
      super::enable(on);
    }

    template<orientation_t O, typename B, typename L>
    inline void toggle_group<O, B, L>::disable () {
      enable(false);
    }

    template<orientation_t O, typename B, typename L>
    void toggle_group<O, B, L>::uncheck_buttons (button_type except) {
      selection = -1;
      int i = 0;
      for (auto b : buttons) {
        if (b != except) {
          b->set_checked(false);
        } else {
          selection = i;
        }
        ++i;
      }
      super::notify_event(detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(event_source::logic));
    }

    template<orientation_t O, typename B, typename L>
    void toggle_group<O, B, L>::on_selection_changed (selection_changed_event::function&& f) {
      win::receiver::on<selection_changed_event>(std::move(f));
    }

    template<orientation_t O, typename B, typename L>
    int toggle_group<O, B, L>::get_selection_index () const {
      return selection;
    }

    template<orientation_t O, typename B, typename L>
    void toggle_group<O, B, L>::set_selection_index (int i) {
      if ((i >= 0) && (i < buttons.size())) {
        uncheck_buttons(buttons[i]);
      }
    }

    template<orientation_t O, typename B, typename L>
    void toggle_group<O, B, L>::set_foreground (os::color c) {
      foreground = c;
    }

    template<orientation_t O, typename B, typename L>
    os::color toggle_group<O, B, L>::get_foreground () const {
      return foreground;
    }

  } // ctrl

} // gui

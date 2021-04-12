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
    toggle_group<O, FG, BG, B, L>::toggle_group ()
      : selection(-1)
    {
      super::set_background(BG);
      super::on_create([&] () {
        for (auto& b : buttons) {
          b->create(*this);
        }
      });
    }

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
      super::get_layout().add(gui::layout::lay(b.get()));
      buttons.push_back(std::move(b));
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    inline std::size_t toggle_group<O, FG, BG, B, L>::count () const {
      return buttons.size();
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
    void toggle_group<O, FG, BG, B, L>::uncheck_buttons (button_type except) {
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

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::on_selection_changed (selection_changed_event::function&& f) {
      win::receiver::on<selection_changed_event>(std::move(f));
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    int toggle_group<O, FG, BG, B, L>::get_selection_index () const {
      return selection;
    }

    template<orientation_t O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::set_selection_index (int i) {
      if ((i >= 0) && (i < buttons.size())) {
        uncheck_buttons(buttons[i]);
      }
    }

  } // ctrl

} // gui

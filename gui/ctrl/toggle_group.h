/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: separator view
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/adaption_layout.h>
#include <gui/ctrl/control.h>
#include <gui/ctrl/separator.h>
#include <gui/ctrl/button.h>


namespace gui {

  namespace win {

    template<orientation O,
             os::color foreground = color::dark_gray,
             os::color background = color::light_gray,
             typename B = flat_toggle_button<foreground, background, true>,
             typename L = layout::adaption_layout<O>>
    class toggle_group : public group_window<L, background> {
    public:
      typedef group_window<L, background> super;
      typedef L layout;
      typedef B button_type;

      ~toggle_group ();

      void add_buttons (const std::initializer_list<std::string>& labels);
      void add_button (const std::string& label);
      void add_button (const text_source& label);

      button_type& get_button (int idx);
      const button_type& get_button (int idx) const;

      void enable (bool on = true);
      void disable ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

    private:
      void uncheck_buttons (button_type* except);

      std::vector<button_type*> buttons;
    };

    // --------------------------------------------------------------------------
    template<os::color foreground = color::dark_gray,
             os::color background = color::light_gray,
             typename button_type = flat_toggle_button<foreground, background, true>,
             typename layout_type = layout::horizontal_adaption<>>
    using htoggle_group = toggle_group<orientation::horizontal, foreground, background, button_type, layout_type>;

    template<os::color foreground = color::dark_gray,
             os::color background = color::light_gray,
             typename button_type = flat_toggle_button<foreground, background, true>,
             typename layout_type = layout::vertical_adaption<>>
    using vtoggle_group = toggle_group<orientation::vertical, foreground, background, button_type, layout_type>;

    // --------------------------------------------------------------------------
    // inlines
    // --------------------------------------------------------------------------
    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    toggle_group<O, FG, BG, B, L>::~toggle_group () {
      for (auto* b : buttons) {
        delete b;
      }
      buttons.clear();
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::add_buttons (const std::initializer_list<std::string>& labels) {
      for (const std::string& label : labels) {
        add_button(label);
      }
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    inline void toggle_group<O, FG, BG, B, L>::add_button (const std::string& label) {
      add_button(const_text(label));
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    inline void toggle_group<O, FG, BG, B, L>::add_button (const text_source& label) {
      button_type* b = new button_type(label);
      buttons.push_back(b);
      b->register_event_handler(REGISTER_FUNCTION, button_clicked_event([&, b] () {
        uncheck_buttons(b);
      }));
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    inline auto toggle_group<O, FG, BG, B, L>::get_button (int idx) -> button_type& {
      return *(buttons[idx]);
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    inline auto toggle_group<O, FG, BG, B, L>::get_button (int idx) const -> const button_type& {
      return *(buttons[idx]);
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::enable (bool on) {
      for (auto* b : buttons) {
        b->enable(on);
      }
      super::enable(on);
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    inline void toggle_group<O, FG, BG, B, L>::disable () {
      enable(false);
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::create (const container& parent,
                                                const core::rectangle& place) {
      super::create(parent, place);
      for (auto* b : buttons) {
        b->create(*this);
      }
      super::set_children_visible();
    }

    template<orientation O, os::color FG, os::color BG, typename B, typename L>
    void toggle_group<O, FG, BG, B, L>::uncheck_buttons (button_type* except) {
      for (auto* b : buttons) {
        if (b != except) {
          b->set_checked(false);
        }
      }
    }

  } // win

} // gui

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
             typename button_type = flat_toggle_button<foreground, background, true> >
    class toggle_group : public group_window<layout::adaption_layout<O, 0, 0, 2>, background> {
    public:
      typedef group_window<layout::adaption_layout<O, 0, 0, 2>, background> super;
      typedef basic_separator<!O, true, background> separator_type;

      struct button {
        button (const text_source& label, bool first)
          : btn(label)
          , first(first)
        {}

        button_type btn;
        separator_type sep;
        bool first;

        void create (toggle_group& parent) {
          if (!first) {
            sep.create(parent);
            parent.get_layout().add_separator(&sep);
          }
          btn.create(parent);
        }

      };

      ~toggle_group () {
        for (button* b : buttons) {
          delete b;
        }
        buttons.clear();
      }

      void add_buttons (const std::initializer_list<std::string>& labels) {
        for (const std::string& label : labels) {
          add_button(label);
        }
      }

      void add_button (const std::string& label) {
        add_button(const_text(label));
      }

      void add_button (const text_source& label) {
        button* b = new button(label, buttons.empty());
        buttons.push_back(b);
        b->btn.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&, b] () {
                                                                                uncheck_buttons(b);
                                                                              }));
      }

      button_type& get_button (int idx) {
        return buttons[idx]->btn;
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
        for (button* b : buttons) {
          b->create(*this);
        }
        super::set_children_visible();
      }

    private:
      void uncheck_buttons (button* except) {
        for (button* b : buttons) {
          if (b != except) {
            b->btn.set_checked(false);
          }
        }
      }

      std::vector<button*> buttons;
    };

    // --------------------------------------------------------------------------
    template<os::color foreground = color::dark_gray,
             os::color background = color::light_gray,
             typename button_type = flat_toggle_button<foreground, background, true> >
    using htoggle_group = toggle_group<orientation::horizontal, foreground, background, button_type>;

    template<os::color foreground = color::dark_gray,
             os::color background = color::light_gray,
             typename button_type = flat_toggle_button<foreground, background, true> >
    using vtoggle_group = toggle_group<orientation::vertical, foreground, background, button_type>;

  } // win

} // gui

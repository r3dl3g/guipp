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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "control.h"
#include "separator.h"
#include "button.h"
#include "layout.h"

namespace gui {

  namespace win {

    template<orientation H,
             os::color B = color::dark_gray,
             os::color C = color::light_gray,
             typename button_type = flat_toggle_button<B, C, true>>
    class toggle_group : public group_window<layout::adaption_layout<H, 0, 0, 2>, C> {
    public:
      typedef group_window<layout::adaption_layout<H, 0, 0, 2>, C> super;
      typedef separator_t<!H, true, C> separator_type;

      struct button {
        button (const std::string& label, bool first)
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
    template<os::color B = color::dark_gray,
             os::color C = color::light_gray,
             typename button_type = flat_toggle_button<B, C, true>>
    using htoggle_group = toggle_group<orientation::horizontal, B, C, button_type>;

    template<os::color B = color::dark_gray,
             os::color C = color::light_gray,
             typename button_type = flat_toggle_button<B, C, true>>
    using vtoggle_group = toggle_group<orientation::vertical, B, C, button_type>;

  } // win

} // gui

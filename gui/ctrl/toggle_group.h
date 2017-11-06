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
      typedef L layout;
      typedef B button_type;
      typedef group_window<L, background> super;

      ~toggle_group () {
        for (auto* b : buttons) {
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
        button_type* b = new button_type(label);
        buttons.push_back(b);
        b->register_event_handler(REGISTER_FUNCTION, button_clicked_event([&, b] () {
          uncheck_buttons(b);
        }));
      }

      button_type& get_button (int idx) {
        return *(buttons[idx]);
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
        for (auto* b : buttons) {
          b->create(*this);
        }
        super::set_children_visible();
      }

    private:
      void uncheck_buttons (button_type* except) {
        for (auto* b : buttons) {
          if (b != except) {
            b->set_checked(false);
          }
        }
      }

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

  } // win

} // gui

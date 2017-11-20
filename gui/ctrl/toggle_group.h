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
 * @brief     C++ API: toggle group
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

  } // win

} // gui

#include <gui/ctrl/toggle_group.inl>

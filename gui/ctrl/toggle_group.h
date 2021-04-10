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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/adaption_layout.h>
#include <gui/layout/layout_container.h>
#include <gui/ctrl/control.h>
#include <gui/ctrl/separator.h>
#include <gui/ctrl/button.h>


namespace gui {

  namespace ctrl {

    template<orientation_t O,
             os::color foreground = color::dark_gray,
             os::color background = color::light_gray,
             typename B = flat_toggle_button<foreground, background, true>,
             typename L = layout::adaption_layout<O>>
    class toggle_group : public win::group_window<L, background> {
    public:
      typedef win::group_window<L, background> super;
      typedef L layout;
      typedef B button_t;
      typedef std::shared_ptr<B> button_type;

      toggle_group ();
      ~toggle_group ();

      void add_buttons (const std::initializer_list<std::string>& labels);
      void add_button (const std::string& label);
      void add_button (const text_source& label);

      std::size_t count () const;
      button_type& get_button (int idx);
      const button_type& get_button (int idx) const;

      void enable (bool on = true);
      void disable ();

      void on_selection_changed (std::function<void(event_source)>&& f);

      int get_selection_index () const;
      void set_selection_index (int i);

    private:
      void uncheck_buttons (button_type except);

      std::vector<button_type> buttons;
      int selection;
    };

    // --------------------------------------------------------------------------
    template<os::color foreground = color::dark_gray,
             os::color background = color::light_gray,
             typename button_type = flat_toggle_button<foreground, background, true>,
             typename layout_type = layout::horizontal_adaption<>>
    using htoggle_group = toggle_group<orientation_t::horizontal, foreground, background, button_type, layout_type>;

    template<os::color foreground = color::dark_gray,
             os::color background = color::light_gray,
             typename button_type = flat_toggle_button<foreground, background, true>,
             typename layout_type = layout::vertical_adaption<>>
    using vtoggle_group = toggle_group<orientation_t::vertical, foreground, background, button_type, layout_type>;

  } // ctrl

} // gui

#include <gui/ctrl/toggle_group.inl>

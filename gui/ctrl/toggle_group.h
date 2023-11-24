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
             typename B = flat_toggle_button<true>,
             typename L = layout::adaption_layout<O>>
    class toggle_group : public win::group_window<L> {
    public:
      typedef win::group_window<L> super;
      typedef L layout;
      typedef B button_t;
      typedef std::shared_ptr<B> button_type;

      toggle_group ();
      ~toggle_group ();

      void add_button (button_type btn);
      void add_button (const std::string& label);
      void add_button (const core::text_source& label);

      void add_buttons (const std::vector<button_type>& buttons);
      void add_buttons (const std::vector<core::const_text>& labels);

      template<typename T = B, typename I>
      void add_custom_buttons (std::initializer_list<I> items) {
        for (const I& item : items) {
          add_button(std::make_shared<T>(item));
        }
      }

      std::size_t count () const;
      button_type& get_button (int idx);
      const button_type& get_button (int idx) const;

      void enable (bool on = true);
      void disable ();

      void on_selection_changed (std::function<void(event_source)>&& f);

      int get_selection_index () const;
      void set_selection_index (int i);

      void set_foreground (os::color);
      os::color get_foreground () const;

    private:
      void uncheck_buttons (button_type except);

      std::vector<button_type> buttons;
      int selection;
      os::color foreground;
    };

    // --------------------------------------------------------------------------
    template<typename button_type = flat_toggle_button<true>,
             typename layout_type = layout::horizontal_adaption<>>
    using htoggle_group = toggle_group<orientation_t::horizontal, button_type, layout_type>;

    template<typename button_type = flat_toggle_button<true>,
             typename layout_type = layout::vertical_adaption<>>
    using vtoggle_group = toggle_group<orientation_t::vertical, button_type, layout_type>;

  } // ctrl

} // gui

#include <gui/ctrl/toggle_group.inl>

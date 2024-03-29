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
 * @brief     multi page view with tab group control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/tab_group.h>
#include <gui/layout/split_layout.h>


// --------------------------------------------------------------------------
namespace gui {

  namespace ctrl {

    template<look::look_and_feel_t L = look::system_look_and_feel>
    struct tab_view_button_origin {
      static constexpr origin_t origin = origin_t::start;
      static constexpr uint32_t min = 60;
      static constexpr uint32_t max = std::numeric_limits<uint32_t>::max();
    };

    template<>
    struct tab_view_button_origin<look::look_and_feel_t::osx> {
      static constexpr origin_t origin = origin_t::center;
      static constexpr uint32_t min = 60;
      static constexpr uint32_t max = 180;
    };

    template<alignment_t A>
    struct tab_group_traits {};

    template<>
    struct tab_group_traits<alignment_t::top> {
      typedef top_tab_group<tab_view_button_origin<>::origin,
                            tab_view_button_origin<>::min,
                            tab_view_button_origin<>::max> type;
      static constexpr int size = IF_MOBILE_ELSE(80, 25);
    };

    template<>
    struct tab_group_traits<alignment_t::bottom> {
      typedef bottom_tab_group<tab_view_button_origin<>::origin,
                              tab_view_button_origin<>::min,
                              tab_view_button_origin<>::max> type;
      static constexpr int size = IF_MOBILE_ELSE(80, 25);
    };

    template<>
    struct tab_group_traits<alignment_t::left> {
      typedef left_tab_group<tab_view_button_origin<>::origin,
                              tab_view_button_origin<>::min,
                              tab_view_button_origin<>::max> type;
      static constexpr int size = 80;
    };

    template<>
    struct tab_group_traits<alignment_t::right> {
      typedef right_tab_group<tab_view_button_origin<>::origin,
                              tab_view_button_origin<>::min,
                              tab_view_button_origin<>::max> type;
      static constexpr int size = 80;
    };

    template<alignment_t A = alignment_t::top,
             typename B = typename tab_group_traits<A>::type,
             typename L = layout::split_layout<A, tab_group_traits<A>::size>
             >
    class tab_view : public win::group_window<L> {
    public:
      typedef win::group_window<L> super;
      typedef B button_group_t;
      typedef L layout_t;
      typedef typename button_group_t::button_t button_t;

      tab_view ();
      tab_view (tab_view&&);

      void show_page (int idx);

      void add_page (const std::string& label, win::window& page);

      void on_selection_changed (std::function<void(event_source)>&& f);
      int get_selection_index () const;

      button_group_t& get_buttons () {
        return buttons;
      }

    private:
      void init ();
      
      std::vector<std::reference_wrapper<win::window>> pages;
      button_group_t buttons;

    };

  } // namespace ctrl

} // namespace gui

#include <gui/ctrl/tab_view.inl>

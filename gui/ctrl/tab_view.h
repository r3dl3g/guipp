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
 * @brief     C++ API: scrolled views
 *
 * @file
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
      static constexpr os::color foreground = color::black;
      static constexpr os::color background = color::very_light_gray;
      static constexpr uint32_t min = 60;
      static constexpr uint32_t max = std::numeric_limits<uint32_t>::max();
    };

    template<>
    struct tab_view_button_origin<look::look_and_feel_t::osx> {
      static constexpr origin_t origin = origin_t::center;
      static constexpr os::color foreground = color::black;
      static constexpr os::color background = color::very_light_gray;
      static constexpr uint32_t min = 60;
      static constexpr uint32_t max = 180;
    };

    template<alignment_t A>
    struct tab_group_traits {};

    template<>
    struct tab_group_traits<alignment_t::top> {
      typedef top_tab_group<tab_view_button_origin<>::origin,
                            tab_view_button_origin<>::foreground,
                            tab_view_button_origin<>::background,
                            tab_view_button_origin<>::min,
                            tab_view_button_origin<>::max> type;
      static constexpr int size = 25;
    };

    template<>
    struct tab_group_traits<alignment_t::bottom> {
      typedef bottom_tab_group<tab_view_button_origin<>::origin,
                              tab_view_button_origin<>::foreground,
                              tab_view_button_origin<>::background,
                              tab_view_button_origin<>::min,
                              tab_view_button_origin<>::max> type;
      static constexpr int size = 25;
    };

    template<>
    struct tab_group_traits<alignment_t::left> {
      typedef left_tab_group<tab_view_button_origin<>::origin,
                              tab_view_button_origin<>::foreground,
                              tab_view_button_origin<>::background,
                              tab_view_button_origin<>::min,
                              tab_view_button_origin<>::max> type;
      static constexpr int size = 75;
    };

    template<>
    struct tab_group_traits<alignment_t::right> {
      typedef right_tab_group<tab_view_button_origin<>::origin,
                              tab_view_button_origin<>::foreground,
                              tab_view_button_origin<>::background,
                              tab_view_button_origin<>::min,
                              tab_view_button_origin<>::max> type;
      static constexpr int size = 75;
    };

    template<alignment_t A = alignment_t::top,
             os::color background = color::very_very_light_gray,
             typename B = typename tab_group_traits<A>::type,
             typename L = layout::split_layout<A, tab_group_traits<A>::size>
             >
    class tab_view : public win::group_window<L, background> {
    public:
      typedef win::group_window<L, background> super;
      typedef B button_group_t;
      typedef L layout_t;
      typedef typename button_group_t::button_t button_t;

      tab_view () {
        super::get_layout().set_header(layout::lay(buttons));

        buttons.on_selection_changed([&] (event_source) {
          show_page(buttons.get_selection_index());
        });

        super::on_create([&](){
          buttons.create(*this);
          if (!pages.empty()) {
            pages[0].get().set_visible(true);
          }
        });
      }

      void show_page (int idx) {
        if ((idx >= 0) && (idx < pages.size())) {
          super::get_layout().set_body(layout::lay(pages[idx].get()));
          for (int i = 0; i < pages.size(); ++i) {
            pages[i].get().set_visible(i == idx);
          }
          super::layout();
        }
      }

      void add_page (const std::string& label, win::window& page) {
        buttons.add_button(label);
        if (pages.empty()) {
          super::get_layout().set_body(layout::lay(page));
          buttons.get_button(0).get()->set_checked(true);
        } else {
          page.set_visible(false);
        }

        pages.push_back(page);
      }

    private:
      std::vector<std::reference_wrapper<win::window>> pages;
      button_group_t buttons;

    };

  } // namespace ctrl

} // namespace gui

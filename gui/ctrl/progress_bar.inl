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
 * @brief     C++ API: label
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      inline auto progress_bar_base::get_min () const -> type {
        return data.min;
      }

      inline auto progress_bar_base::get_max () const -> type {
        return data.max;
      }

      inline auto progress_bar_base::get_value () const -> type {
        return data.value;
      }

      inline void progress_bar_base::operator++ () {
        set_value(get_value() + 1);
      }

      inline void progress_bar_base::operator++ (int) {
        set_value(get_value() + 1);
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin_t A, draw::frame::drawer D, os::color F, os::color B, os::color C>
    inline basic_progress_bar<A, D, F, B, C>::basic_progress_bar () {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D, os::color F, os::color B, os::color C>
    inline basic_progress_bar<A, D, F, B, C>::basic_progress_bar (const basic_progress_bar& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D, os::color F, os::color B, os::color C>
    inline basic_progress_bar<A, D, F, B, C>::basic_progress_bar (basic_progress_bar&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin_t align, draw::frame::drawer frame, os::color foreground, os::color background, os::color bar_color>
    inline void basic_progress_bar<align, frame, foreground, background, bar_color>::paint (const draw::graphics& graph) {
      gui::core::rectangle place = frame(graph, client_area());
      auto pos = (get_value() - get_min()) / (get_max() - get_min()) * place.width();
      paint::progress_bar(graph, client_area(), get_text(), foreground, background, align, bar_color, pos);
    }

    template<text_origin_t A, draw::frame::drawer D, os::color F, os::color B, os::color C>
    inline void basic_progress_bar<A, D, F, B, C>::init () {
      on_paint(draw::paint(this, &basic_progress_bar::paint));
    }

    // --------------------------------------------------------------------------

  } // ctrl

} // gui

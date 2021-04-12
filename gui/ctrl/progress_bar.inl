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
    template<text_origin_t A, draw::frame::drawer D>
    inline basic_progress_bar<A, D>::basic_progress_bar (os::color bc,
                                                         os::color fg,
                                                         os::color bg)
      : bar_color(bc)
      , foreground(fg)
    {
      super::set_background(bg);
      init();
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline basic_progress_bar<A, D>::basic_progress_bar (const basic_progress_bar& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline basic_progress_bar<A, D>::basic_progress_bar (basic_progress_bar&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin_t align, draw::frame::drawer frame>
    inline void basic_progress_bar<align, frame>::paint (draw::graphics& graph) {
      gui::core::rectangle place = frame(graph, client_geometry());
      auto pos = (get_value() - get_min()) / (get_max() - get_min()) * place.width();
      gui::look::progress_bar(graph, place, get_text(), foreground, super::get_background(), align, bar_color, pos);
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline void basic_progress_bar<A, D>::init () {
      on_paint(draw::paint(this, &basic_progress_bar::paint));
    }

    // --------------------------------------------------------------------------

  } // ctrl

} // gui

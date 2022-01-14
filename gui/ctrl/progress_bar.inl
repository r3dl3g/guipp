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
 * @brief     progress bar control
 *
 * @license   MIT license. See accompanying file LICENSE.
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
    template<orientation_t H, text_origin_t A, draw::frame::drawer D>
    inline basic_progress_bar<H, A, D>::basic_progress_bar (os::color bc,
                                                         os::color fg,
                                                         os::color bg)
      : super(bc, fg, bg)
    {
      init();
    }

    template<orientation_t H, text_origin_t A, draw::frame::drawer D>
    inline basic_progress_bar<H, A, D>::basic_progress_bar (const basic_progress_bar& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation_t H, text_origin_t A, draw::frame::drawer D>
    inline basic_progress_bar<H, A, D>::basic_progress_bar (basic_progress_bar&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation_t H, text_origin_t origin, draw::frame::drawer frame>
    inline void basic_progress_bar<H, origin, frame>::paint (draw::graphics& graph) {
      detail::progress_bar_drawer<H>(graph, *this, origin, frame);
    }

    template<orientation_t H, text_origin_t A, draw::frame::drawer D>
    inline void basic_progress_bar<H, A, D>::init () {
      on_paint(draw::paint(this, &basic_progress_bar::paint));
    }

    // --------------------------------------------------------------------------

  } // ctrl

} // gui

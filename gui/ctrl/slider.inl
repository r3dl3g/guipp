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
 * @brief     C++ API: slider control
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      inline auto slider_base::get_min() const -> type {
        return min;
      }

      inline auto slider_base::get_max() const -> type {
        return max;
      }

      template<orientation_t O>
      inline void basic_slider<O>::create (win::container& parent,
                                           const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
      }

      // --------------------------------------------------------------------------
      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation_t::vertical>::basic_slider ();

      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation_t::vertical>::basic_slider (const basic_slider&);

      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation_t::vertical>::basic_slider (basic_slider&&) noexcept;

      template<>
      GUIPP_CTRL_EXPORT void basic_slider<orientation_t::vertical>::set_value (core::point::type v);

      template<>
      GUIPP_CTRL_EXPORT core::point::type basic_slider<orientation_t::vertical>::get_value () const;

      template<>
      GUIPP_CTRL_EXPORT void basic_slider<orientation_t::vertical>::init ();

      // --------------------------------------------------------------------------
      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation_t::horizontal>::basic_slider ();

      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation_t::horizontal>::basic_slider (const basic_slider&);

      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation_t::horizontal>::basic_slider (basic_slider&&) noexcept;

      template<>
      GUIPP_CTRL_EXPORT void basic_slider<orientation_t::horizontal>::set_value (core::point::type v);

      template<>
      GUIPP_CTRL_EXPORT core::point::type basic_slider<orientation_t::horizontal>::get_value () const;

      template<>
      GUIPP_CTRL_EXPORT void basic_slider<orientation_t::horizontal>::init ();

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t O,
             draw::frame::drawer F>
    inline basic_framed_slider<O, F>::basic_framed_slider () {
      init();
    }

    template<orientation_t O,
             draw::frame::drawer F>
    inline basic_framed_slider<O, F>::basic_framed_slider (const basic_framed_slider& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation_t O,
             draw::frame::drawer F>
    inline basic_framed_slider<O, F>::basic_framed_slider (basic_framed_slider&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation_t O,
             draw::frame::drawer F>
      void basic_framed_slider<O, F>::init () {
      super::on_paint(draw::paint(this, &basic_framed_slider::paint));
    }

    template<orientation_t O,
             draw::frame::drawer F>
    void basic_framed_slider<O, F>::paint (const draw::graphics& g) {
      look::slider(g, super::client_area(), super::is_focused(), F);
    }

  } // ctrl

} // gui

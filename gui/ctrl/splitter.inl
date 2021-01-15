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
 * @brief     C++ API: splitter control
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      inline auto splitter_base::get_min() const -> type {
        return min;
      }

      inline auto splitter_base::get_max() const -> type {
        return max;
      }

      template<orientation_t O>
      inline void basic_splitter<O>::create (win::container& parent,
                                           const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
      }

      // --------------------------------------------------------------------------
      template<>
      GUIPP_CTRL_EXPORT basic_splitter<orientation_t::vertical>::basic_splitter ();

      template<>
      GUIPP_CTRL_EXPORT basic_splitter<orientation_t::vertical>::basic_splitter (const basic_splitter&);

      template<>
      GUIPP_CTRL_EXPORT basic_splitter<orientation_t::vertical>::basic_splitter (basic_splitter&&) noexcept;

      template<>
      GUIPP_CTRL_EXPORT void basic_splitter<orientation_t::vertical>::set_value (core::point::type v);

      template<>
      GUIPP_CTRL_EXPORT core::point::type basic_splitter<orientation_t::vertical>::get_value () const;

      template<>
      GUIPP_CTRL_EXPORT void basic_splitter<orientation_t::vertical>::init ();

      // --------------------------------------------------------------------------
      template<>
      GUIPP_CTRL_EXPORT basic_splitter<orientation_t::horizontal>::basic_splitter ();

      template<>
      GUIPP_CTRL_EXPORT basic_splitter<orientation_t::horizontal>::basic_splitter (const basic_splitter&);

      template<>
      GUIPP_CTRL_EXPORT basic_splitter<orientation_t::horizontal>::basic_splitter (basic_splitter&&) noexcept;

      template<>
      GUIPP_CTRL_EXPORT void basic_splitter<orientation_t::horizontal>::set_value (core::point::type v);

      template<>
      GUIPP_CTRL_EXPORT core::point::type basic_splitter<orientation_t::horizontal>::get_value () const;

      template<>
      GUIPP_CTRL_EXPORT void basic_splitter<orientation_t::horizontal>::init ();

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t O,
             draw::frame::drawer F>
    inline basic_framed_splitter<O, F>::basic_framed_splitter () {
      init();
    }

    template<orientation_t O,
             draw::frame::drawer F>
    inline basic_framed_splitter<O, F>::basic_framed_splitter (const basic_framed_splitter& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation_t O,
             draw::frame::drawer F>
    inline basic_framed_splitter<O, F>::basic_framed_splitter (basic_framed_splitter&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation_t O,
             draw::frame::drawer F>
      void basic_framed_splitter<O, F>::init () {
      super::on_paint(draw::paint(this, &basic_framed_splitter::paint));
    }

    template<orientation_t O,
             draw::frame::drawer F>
    void basic_framed_splitter<O, F>::paint (const draw::graphics& g) {
      look::splitter(g, super::client_area(), super::is_focused(), F);
    }

  } // ctrl

} // gui

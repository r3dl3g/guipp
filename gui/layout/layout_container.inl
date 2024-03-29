/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     container window to hold clients with a specific layout
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace win {

    namespace detail {

      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (A ... args)
        : layouter(args ...) {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (const layout_container_base& rhs)
        : super(rhs)
        , layouter(rhs.layouter)
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (layout_container_base&& rhs) noexcept
        : super(std::move(rhs))
        //, layouter(std::move(rhs.layouter))
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::layout () {
        super::notify_layout();
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::layout (const core::rectangle& r) {
        logging::trace() << *this << " layout(" << r << ")";
        layouter.layout(r);
      }

      template<typename B, typename L, typename ... A>
      inline auto layout_container_base<B, L, A...>::get_layout () -> layout_type& {
        return layouter;
      }

      template<typename B, typename L, typename ... A>
      inline auto layout_container_base<B, L, A...>::get_layout () const -> const layout_type& {
        return layouter;
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::init () {
        super::on_show([&] () {
          layout();
        });
        super::on_layout([&] (const core::rectangle& r) {
          layout(r);
        });
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::add (const std::vector<std::reference_wrapper<win::window>>& list) {
        get_layout().add(list);
        super::add(list);
      }

    } // namespace detail

    template<typename L, typename ... A>
    inline group_window<L, A...>::group_window (A ... args)
      : super(args ...)
    {}

    template<typename L, typename ... A>
    inline void group_window<L, A...>::create (container& parent,
      										   const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

  } // namespace win

} // namespace gui

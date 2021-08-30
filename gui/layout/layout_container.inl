/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

    namespace detail {

      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (const A& ... args)
        : layouter(args ...)
      {
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
        , layouter(std::move(rhs.layouter))
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::layout () {
        layout(super::client_geometry());
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::layout (const core::rectangle& r) {
        logging::trace() << *this << " layout(" << r << ")";
        layouter.layout(r);
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::resize (const core::size& sz, bool repaint) {
        layouter.layout(core::rectangle(sz));
        super::resize(sz, repaint);
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::geometry (const core::rectangle& r, bool repaint) {
        layouter.layout(core::rectangle(r.size()));
        super::geometry(r, repaint);
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
          layouter.layout(r);
        });
      }

    } // namespace detail

    template<typename L, typename ... A>
    inline group_window<L, A...>::group_window (const A& ... args)
      : super(args ...)
    {}

    template<typename L, typename ... A>
    inline void group_window<L, A...>::create (container& parent,
                                               const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

  } // namespace win

} // namespace gui

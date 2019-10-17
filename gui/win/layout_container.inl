/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
        : layouter(this, args ...)
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (const layout_container_base& rhs)
        : super(rhs)
        , layouter(this, rhs.layouter)
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (layout_container_base&& rhs)
        : super(std::move(rhs))
        , layouter(this, std::move(rhs.layouter))
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::layout () {
        LogTrace << *this << " layout()";
        layouter.layout(super::client_area());
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::resize (const core::size& sz, bool repaint) {
        layouter.layout(core::rectangle(sz));
        super::resize(sz, repaint);
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::place (const core::rectangle& r, bool repaint) {
        layouter.layout(r.size());
        super::place(r, repaint);
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
      }

    } // namespace detail

    template<typename L, os::color B, typename ... A>
    inline group_window<L, B, A...>::group_window (const A& ... args)
      : super(args ...)
    {}

    template<typename L, os::color B, typename ... A>
    inline void group_window<L, B, A...>::create (const container& parent,
                                                  const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

  } // namespace win

} // namespace gui

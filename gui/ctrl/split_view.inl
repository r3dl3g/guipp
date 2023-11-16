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
 * @brief     split window control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    template<orientation_t O>
    core::size::type split_view_traits<O>::get_splitter_width () {
      return 8;
    }

    template<orientation_t O>
    double split_view_traits<O>::get_split_pos (const core::point& pt, const core::size& sz) {
      return double(traits::get_2(pt) + get_splitter_width() / 2) / (double)traits::get_2(sz);
    }

    template<orientation_t O>
    core::point::type split_view_traits<O>::get_position (const core::point& p, const core::size& s, double pos, core::size::type offset) {
      return traits::get_2(p) + core::point::type(traits::get_2(s) * pos + offset);
    }

    template<orientation_t O>
    core::rectangle split_view_traits<O>::get_splitter_geometry (const core::rectangle& r, double pos) {
      core::point p = r.top_left();
      core::size s = r.size();
      const core::point::type x = std::max(get_position(p, s, pos, -get_splitter_width() / 2), 0.F);
      traits::set_2(p, x);
      traits::set_2(s, get_splitter_width());
      return {p, s};
    }

    template<orientation_t O>
    core::rectangle split_view_traits<O>::get_window_geometry (const core::rectangle& r, double pos1, double pos2) {
      core::point p = r.top_left();
      core::size s = r.size();
      const core::point::type p1 = get_position(p, s, pos1, get_splitter_width() / 2);
      const core::point::type p2 = get_position(p, s, pos2, -get_splitter_width() / 2);
      traits::set_2(p, p1);
      traits::set_2(s, p2 - p1);
      return {p, s};
    }

    template<orientation_t O>
    core::rectangle split_view_traits<O>::get_first_geometry (const core::rectangle& r, double pos) {
      core::point p = r.top_left();
      core::size s = r.size();
      const core::point::type p1 = traits::get_2(p);
      const core::point::type p2 = get_position(p, s, pos, -get_splitter_width() / 2);
      traits::set_2(p, p1);
      traits::set_2(s, p2 - p1);
      return {p, s};
    }

    template<orientation_t O>
    core::rectangle split_view_traits<O>::get_last_geometry (const core::rectangle& r, double pos) {
      core::point p = r.top_left();
      core::size s = r.size();
      const core::point::type p1 = get_position(p, s, pos, get_splitter_width() / 2);
      const core::point::type p2 = traits::get_2(r.bottom_right());
      traits::set_2(p, p1);
      traits::set_2(s, p2 - p1);
      return {p, s};
    }

    // --------------------------------------------------------------------------
    template<orientation_t O, std::size_t N>
    inline split_view<O, N>::split_view () {
      for (int i = 0; i < N - 1; ++i) {
        split_pos[i] = (double)(i + 1) / N;
      }
    }

    // --------------------------------------------------------------------------
    template<orientation_t O, std::size_t N>
    inline win::window* split_view<O, N>::get (std::size_t i) const {
      return views[i];
    }

    template<orientation_t O, std::size_t N>
    inline void split_view<O, N>::set (std::size_t i, win::window* s) {
      views[i] = s;
    }

    template<orientation_t O, std::size_t N>
    inline win::window* split_view<O, N>::get_splitter (std::size_t i) const {
      return splitter[i];
    }

    template<orientation_t O, std::size_t N>
    inline void split_view<O, N>::set_splitter (std::size_t i, win::window* s) {
      splitter[i] = s;
    }

    template<orientation_t O, std::size_t N>
    inline void split_view<O, N>::set_split_pos (std::size_t i, double pos) {
      if (i < N - 2) {
        pos = std::min(pos, split_pos[i + 1]);
      }
      if (i > 0) {
        pos = std::max(pos, split_pos[0]);
      }
      split_pos[i] = pos;
    }

    // --------------------------------------------------------------------------
    namespace detail {

      template<std::size_t I, orientation_t O, std::size_t N>
      struct layout_n {
        static void layout (const core::rectangle& r,
                            std::array<win::window*, N>& views,
                            std::array<win::window*, N-1>& splitter,
                            std::array<double, N-1>& split_pos) {
          typedef split_view_traits<O> traits;
          if (views[I]) {
            views[I]->geometry(traits::get_window_geometry(r, split_pos[I-1], split_pos[I]), true, true);
          }
          if (splitter[I-1]) {
            splitter[I-1]->geometry(traits::get_splitter_geometry(r, split_pos[I-1]), true, true);
          }
          layout_n<I-1, O, N>::layout(r, views, splitter, split_pos);
        }
      };

      template<orientation_t O>
      struct layout_n<0, O, 1> {
        static void layout (const core::rectangle& r,
                            std::array<win::window*, 1>& views,
                            std::array<win::window*, 0>& splitter,
                            std::array<double, 0>& split_pos) {
          typedef split_view_traits<O> traits;
          if (views[0]) {
            views[0]->geometry(r, true, true);
          }
        }
      };

      template<orientation_t O, std::size_t N>
      struct layout_n<0, O, N> {
        static void layout (const core::rectangle& r,
                            std::array<win::window*, N>& views,
                            std::array<win::window*, N-1>& splitter,
                            std::array<double, N-1>& split_pos) {
          typedef split_view_traits<O> traits;
          if (views[0]) {
            views[0]->geometry(traits::get_first_geometry(r, split_pos[0]), true, true);
          }
        }
      };

      template<orientation_t O, std::size_t N>
      struct layout_n<N-1, O, N> {
        static void layout (const core::rectangle& r,
                            std::array<win::window*, N>& views,
                            std::array<win::window*, N-1>& splitter,
                            std::array<double, N-1>& split_pos) {
          typedef split_view_traits<O> traits;
          if (views[N-1]) {
            views[N-1]->geometry(traits::get_last_geometry(r, split_pos[N-2]), true, true);
          }
          if (splitter[N-2]) {
            splitter[N-2]->geometry(traits::get_splitter_geometry(r, split_pos[N-2]), true, true);
          }
          layout_n<N-2, O, N>::layout(r, views, splitter, split_pos);
        }
      };

      template<std::size_t I, typename V, typename T>
      struct setter {
        static void set (V& v, T& views) {
          v.set(I-1, &(std::get<I-1>(views)));
          setter<I-1, V, T>::set(v, views);
        }
      };

      template<typename V, typename T>
      struct setter<0, V, T> {
        static void set (V& v, T& views) {
        }
      };

      template<std::size_t I, typename V, typename T>
      struct creater {
        static void create (V& v, T& views) {
          std::get<I-1>(views).create(v);
          creater<I-1, V, T>::create(v, views);
        }
      };

      template<typename V, typename T>
      struct creater<0, V, T> {
        static void create (V& v, T& views) {
        }
      };


    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t O, std::size_t N>
    void split_view<O, N>::layout (const core::rectangle& r) {
      logging::trace() << "split_view::layout(" << r << ") split_pos: " << split_pos[0];
      detail::layout_n<N-1, O, N>::layout(r, views, splitter, split_pos);
    }

    template<orientation_t O, std::size_t N>
    core::rectangle split_view<O, N>::get_splitter_limits (std::size_t i, const core::rectangle& r) {

      core::point p1;
      if (i == 0) {
        p1 = r.top_left();
      } else {
        p1 = traits::get_splitter_geometry(r, split_pos[i - 1]).bottom_right();
      }
      // core::orientation_traits<O>::move_2(p1, traits::get_splitter_width());

      core::point p2;
      if (i == N - 2) {
        p2 = r.bottom_right();
      } else {
        p2 = traits::get_splitter_geometry(r, split_pos[i + 1]).top_left();
      }
      core::orientation_traits<O>::move_2(p2, -traits::get_splitter_width());
      return { p1, p2 };
    }

    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<orientation_t O, typename... Ts>
    inline split_view<O, Ts...>::split_view () {
      init();
    }

    template<orientation_t O, typename... Ts>
    inline split_view<O, Ts...>::split_view (split_view&& rhs) noexcept
      : super(std::move(rhs))
      , splitter(std::move(rhs.splitter))
      , views(std::move(rhs.views))
    {
      init();
    }

    template<orientation_t O, typename... Ts>
    inline split_view<O, Ts...>::split_view (Ts&&... ts)
      : views(std::move(ts)...)
    {
      init();
    }

    template<orientation_t O, typename... Ts>
    void split_view<O, Ts...>::create (win::container& parent,
                                      const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
      layout::detail::creater<N, split_view<O, Ts...>, tuple_t>::create(*this, views);
      for (std::size_t i = 0; i < N-1; ++i) {
        splitter[i].create(*this);
      }
      super::layout();
    }

    template<orientation_t O, typename... Ts>
    inline double split_view<O, Ts...>::get_split_pos (std::size_t i) const {
      return layout::split_view_traits<O>::get_split_pos(splitter[i].position(), super::client_size());
    }

    template<orientation_t O, typename... Ts>
    inline void split_view<O, Ts...>::set_split_pos (double pos, std::size_t i) {
      super::get_layout().set_split_pos(i, pos);
      super::layout();
    }

    template<orientation_t O, typename... Ts>
    void split_view<O, Ts...>::init () {
      layout::detail::setter<N, layout_type, tuple_t>::set(super::get_layout(), views);
      for (std::size_t i = 0; i < N-1; ++i) {
        super::get_layout().set_splitter(i, &(splitter[i]));
        splitter[i].on_slide([&, i] (int) {
          const core::rectangle limits = super::get_layout().get_splitter_limits(i, super::client_geometry());
          const auto pos = limits.limit_inside(splitter[i].position());
          const auto rel = layout::split_view_traits<O>::get_split_pos(pos, super::client_size());
          set_split_pos(rel, i);
        });
      }
      super::on_layout([&] (const core::rectangle& r) {
        super::get_layout().layout(r);
      });
    }

    // --------------------------------------------------------------------------

    template<typename H, typename B, int S, alignment_t A>
    fix_split_view<H, B, S, A>::fix_split_view () {
      super::get_layout().set_header(layout::lay(header));
      super::get_layout().set_body(layout::lay(body));
    }

    template<typename H, typename B, int S, alignment_t A>
    fix_split_view<H, B, S, A>::fix_split_view (fix_split_view&& rhs) noexcept
      : super(std::move(rhs))
      , header(std::move(rhs.header))
      , body(std::move(rhs.body))
    {
      super::get_layout().set_header(layout::lay(header));
      super::get_layout().set_body(layout::lay(body));
    }

    template<typename H, typename B, int S, alignment_t A>
    fix_split_view<H, B, S, A>::fix_split_view (H&& header, B&& body)
      : header(std::move(header))
      , body(std::move(body))
    {
      super::get_layout().set_header(layout::lay(header));
      super::get_layout().set_body(layout::lay(body));
    }

    template<typename H, typename B, int S, alignment_t A>
    void fix_split_view<H, B, S, A>::create (win::container& parent,
                                             const core::rectangle& place) {
      super::create(clazz::get(), parent, place);

      header.create(*this);
      body.create(*this);
      header.set_visible();
      body.set_visible();
    }

    // --------------------------------------------------------------------------

  } // ctrl

} // gui

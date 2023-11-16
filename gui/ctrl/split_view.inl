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
      const core::point::type p1 = get_position(p, s, pos1, -get_splitter_width() / 2);
      const core::point::type p2 = get_position(p, s, pos2, get_splitter_width() / 2);
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
    template<orientation_t O, std::size_t T>
    inline split_view<O, T>::split_view ()
      : views()
      , splitter()
      , split_pos{0.5}
    {}

    // --------------------------------------------------------------------------
    template<orientation_t O, std::size_t T>
    template<std::size_t N>
    inline win::window* split_view<O, T>::get () const {
      return views[N];
    }

    template<orientation_t O, std::size_t T>
    template<std::size_t N>
    inline void split_view<O, T>::set (win::window* f) {
      views[N] = f;
    }

    template<orientation_t O, std::size_t T>
    template<std::size_t N>
    inline auto split_view<O, T>::get_splitter () const -> splitter_t* {
      return splitter[N];
    }

    template<orientation_t O, std::size_t T>
    template<std::size_t N>
    inline void split_view<O, T>::set_splitter (splitter_t* s) {
      splitter[N] = s;
    }

    template<orientation_t O, std::size_t T>
    inline double split_view<O, T>::get_split_pos (const core::point& pt, const core::size& sz) const {
      return traits::get_split_pos(pt, sz);
    }

    template<orientation_t O, std::size_t T>
    template<std::size_t N>
    inline void split_view<O, T>::set_split_pos (double pos) {
      split_pos[N] = pos;
    }

    template<orientation_t O, std::size_t T>
    inline void split_view<O, T>::set_all (win::window* f,
                                           win::window* s,
                                           splitter_t* sl) {
      set<0>(f);
      set<1>(s);
      set_splitter<0>(sl);
    }

    template<orientation_t O, std::size_t T>
    void split_view<O, T>::add (const std::vector<std::reference_wrapper<win::window>>& list) {
      if (list.size() > 0) {
        win::window* f = &(list[0].get());
        if (f) {
          set<0>(f);
        }
        if (list.size() > 1) {
          win::window* s = &(list[1].get());
          if (s) {
            set<1>(s);
          }
          if (list.size() > 2) {
            splitter_t* sp = &(list[2].get());
            if (sp) {
              set_splitter<0>(sp);
            }
          }
        }
      }
    }

    template<orientation_t O, std::size_t T>
    void split_view<O, T>::layout (const core::rectangle& r) const {
      logging::trace() << "split_view::layout(" << r << ") split_pos: " << split_pos[0];
      if (get<0>()) {
        get<0>()->geometry(traits::get_first_geometry(r, split_pos[0]), true, true);
      }
      if (get<1>()) {
        get<1>()->geometry(traits::get_last_geometry(r, split_pos[0]), true, true);
      }
      if (get_splitter<0>()) {
        get_splitter<0>()->geometry(traits::get_splitter_geometry(r, split_pos[0]), true, true);
      }
    }

    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<orientation_t O, typename... Ts>
    inline split_view<O, Ts...>::split_view () {
      init();
      super::get_layout().set_all(&get<0>(), &get<1>(), &splitter[0]);
    }

    template<orientation_t O, typename... Ts>
    inline split_view<O, Ts...>::split_view (split_view&& rhs) noexcept
      : super(std::move(rhs))
      , splitter(std::move(rhs.splitter))
      , views(std::move(rhs.views))
    {
      init();
      super::get_layout().set_all(&get<0>(), &get<1>(), &splitter[0]);
    }

    template<orientation_t O, typename... Ts>
    inline split_view<O, Ts...>::split_view (Ts&&... ts)
      : views(std::move(ts)...)
    {
      init();
      super::get_layout().set_all(&get<0>(), &get<1>(), &splitter[0]);
    }

    template<orientation_t O, typename... Ts>
    void split_view<O, Ts...>::create (win::container& parent,
                                      const core::rectangle& place,
                                      double split_pos) {
      super::create(clazz::get(), parent, place);
      set_split_pos(split_pos);

      const auto area = super::client_geometry();
      splitter[0].create(*this, layout_type::traits::get_splitter_geometry(area, split_pos));
      get<0>().create(*this, layout_type::traits::get_first_geometry(area, split_pos));
      get<1>().create(*this, layout_type::traits::get_last_geometry(area, split_pos));
      splitter[0].set_visible();
      get<0>().set_visible();
      get<1>().set_visible();
    }

    template<orientation_t O, typename... Ts>
    inline double split_view<O, Ts...>::get_split_pos () const {
      return super::get_layout().get_split_pos(splitter[0].position(), super::client_size());
    }

    template<orientation_t O, typename... Ts>
    inline void split_view<O, Ts...>::set_split_pos (double pos) {
      super::get_layout().template set_split_pos<0>(pos);
      super::layout();
    }

    template<orientation_t O, typename... Ts>
    void split_view<O, Ts...>::init () {
      splitter[0].on_slide([&] (int) {
        set_split_pos(get_split_pos());
      });
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

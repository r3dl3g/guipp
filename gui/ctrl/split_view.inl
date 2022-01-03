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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/layout_container.h>
#include <gui/ctrl/splitter.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT double
    split_view_traits<orientation_t::vertical>::get_split_pos (const core::point&, const core::size&);

    template<>
    GUIPP_CTRL_EXPORT core::size::type
    split_view_traits<orientation_t::vertical>::get_splitter_width ();

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::vertical>::get_first_geometry (const core::rectangle&, double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::vertical>::get_second_geometry (const core::rectangle&, double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::vertical>::get_splitter_geometry (const core::rectangle&, double);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT double
    split_view_traits<orientation_t::horizontal>::get_split_pos (const core::point&, const core::size&);

    template<>
    GUIPP_CTRL_EXPORT core::size::type
    split_view_traits<orientation_t::horizontal>::get_splitter_width ();

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::horizontal>::get_first_geometry (const core::rectangle&, double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::horizontal>::get_second_geometry (const core::rectangle&, double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::horizontal>::get_splitter_geometry (const core::rectangle&, double);

    // --------------------------------------------------------------------------
    template<orientation_t O, typename F, typename S>
    inline split_view<O, F, S>::split_view ()
      : first(nullptr)
      , second(nullptr)
      , splitter(nullptr)
      , split_pos(0.5)
    {}

    // --------------------------------------------------------------------------
    template<orientation_t O, typename F, typename S>
    inline F* split_view<O, F, S>::get_first () const {
      return first;
    }

    template<orientation_t O, typename F, typename S>
    inline void split_view<O, F, S>::set_first (F* f) {
      first = f;
    }

    template<orientation_t O, typename F, typename S>
    inline S* split_view<O, F, S>::get_second () const {
      return second;
    }

    template<orientation_t O, typename F, typename S>
    inline void split_view<O, F, S>::set_second (S* s) {
      second = s;
    }

    template<orientation_t O, typename F, typename S>
    inline ctrl::detail::splitter_base* split_view<O, F, S>::get_splitter () const {
      return splitter;
    }

    template<orientation_t O, typename F, typename S>
    inline void split_view<O, F, S>::set_splitter (ctrl::detail::splitter_base* s) {
      splitter = s;
    }

    template<orientation_t O, typename F, typename S>
    inline double split_view<O, F, S>::get_split_pos (const core::point& pt, const core::size& sz) const {
      return traits::get_split_pos(pt, sz);
    }

    template<orientation_t O, typename F, typename S>
    inline void split_view<O, F, S>::set_split_pos (double pos) {
      split_pos = pos;
    }

    template<orientation_t O, typename F, typename S>
    inline void split_view<O, F, S>::set (F* f,
                                          S* s,
                                          ctrl::detail::splitter_base* sl) {
      first = f;
      second = s;
      splitter = sl;
    }

    template<orientation_t O, typename F, typename S>
    void split_view<O, F, S>::layout (const core::rectangle& r) const {
      logging::trace() << "split_view::layout(" << r << ") split_pos: " << split_pos;
      if (first) {
        first->geometry(traits::get_first_geometry(r, split_pos), true);
      }
      if (second) {
        second->geometry(traits::get_second_geometry(r, split_pos), true);
      }
      if (splitter) {
        splitter->geometry(traits::get_splitter_geometry(r, split_pos), true);
      }
    }

    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<orientation_t O, typename F, typename S>
    inline split_view<O, F, S>::split_view () {
      init();
      super::get_layout().set(&first, &second, &splitter);
    }

    template<orientation_t O, typename F, typename S>
    inline split_view<O, F, S>::split_view (split_view&& rhs) noexcept
      : super(std::move(rhs))
      , splitter(std::move(rhs.splitter))
      , first(std::move(rhs.first))
      , second(std::move(rhs.second))
    {
      init();
      super::get_layout().set(&first, &second, &splitter);
    }

    template<orientation_t O, typename F, typename S>
    inline split_view<O, F, S>::split_view (F&& f, S&& s)
      : first(std::move(f))
      , second(std::move(s))
    {
      init();
      super::get_layout().set(&first, &second, &splitter);
    }

    template<orientation_t O, typename F, typename S>
    void split_view<O, F, S>::create (win::container& parent,
                                      const core::rectangle& place,
                                      double split_pos) {
      super::create(clazz::get(), parent, place);
      set_split_pos(split_pos);

      const auto area = super::client_geometry();
      splitter.create(*this, layout_type::traits::get_splitter_geometry(area, split_pos));
      first.create(*this, layout_type::traits::get_first_geometry(area, split_pos));
      second.create(*this, layout_type::traits::get_second_geometry(area, split_pos));
      splitter.set_visible();
      first.set_visible();
      second.set_visible();
    }

    template<orientation_t O, typename F, typename S>
    inline double split_view<O, F, S>::get_split_pos () const {
      return super::get_layout().get_split_pos(splitter.position(), super::client_size());
    }

    template<orientation_t O, typename F, typename S>
    inline void split_view<O, F, S>::set_split_pos (double pos) {
      super::get_layout().set_split_pos(pos);
      super::layout();
    }

    template<orientation_t O, typename F, typename S>
    void split_view<O, F, S>::init () {
      splitter.on_slide([&] (int) {
        set_split_pos(get_split_pos());
      });
      super::on_layout([&] (const core::rectangle& r) {
        super::get_layout().layout(r);
      });
    }

    // --------------------------------------------------------------------------

    template<typename H, typename B, int S, alignment_t A>
    fix_split_view<H, B, S, A>::fix_split_view () {
      super::get_layout().set_header(lay(header));
      super::get_layout().set_body(lay(body));
    }

    template<typename H, typename B, int S, alignment_t A>
    fix_split_view<H, B, S, A>::fix_split_view (fix_split_view&& rhs) noexcept
      : super(std::move(rhs))
      , header(std::move(rhs.header))
      , body(std::move(rhs.body))
    {
      super::get_layout().set_header(lay(header));
      super::get_layout().set_body(lay(body));
    }

    template<typename H, typename B, int S, alignment_t A>
    fix_split_view<H, B, S, A>::fix_split_view (H&& header, B&& body)
      : header(std::move(header))
      , body(std::move(body))
    {
      super::get_layout().set_header(lay(header));
      super::get_layout().set_body(lay(body));
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

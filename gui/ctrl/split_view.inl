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
 * @brief     C++ API: split views
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/layout_container.h>
#include <gui/ctrl/slider.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT double
    split_view_traits<orientation_t::vertical>::get_split_pos (const core::point&, const core::size&);

    template<>
    GUIPP_CTRL_EXPORT core::size::type
    split_view_traits<orientation_t::vertical>::get_slider_width ();

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::vertical>::get_first_place (const core::rectangle&, double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::vertical>::get_second_place (const core::rectangle&, double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::vertical>::get_slider_place (const core::rectangle&, double);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT double
    split_view_traits<orientation_t::horizontal>::get_split_pos (const core::point&, const core::size&);

    template<>
    GUIPP_CTRL_EXPORT core::size::type
    split_view_traits<orientation_t::horizontal>::get_slider_width ();

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::horizontal>::get_first_place (const core::rectangle&, double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::horizontal>::get_second_place (const core::rectangle&, double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle
    split_view_traits<orientation_t::horizontal>::get_slider_place (const core::rectangle&, double);

    // --------------------------------------------------------------------------
    template<orientation_t O, typename F, typename S>
    inline split_view<O, F, S>::split_view ()
      : first(nullptr)
      , second(nullptr)
      , slider(nullptr)
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
    inline ctrl::detail::slider_base* split_view<O, F, S>::get_slider () const {
      return slider;
    }

    template<orientation_t O, typename F, typename S>
    inline void split_view<O, F, S>::set_slider (ctrl::detail::slider_base* s) {
      slider = s;
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
                                          ctrl::detail::slider_base* sl) {
      first = f;
      second = s;
      slider = sl;
    }

    template<orientation_t O, typename F, typename S>
    void split_view<O, F, S>::layout (const core::rectangle& sz) const {
      clog::trace() << "split_view::layout(" << sz << ") split_pos: " << split_pos;
      if (first) {
        first->place(traits::get_first_place(sz, split_pos), IF_WIN32_ELSE(true, false));
      }
      if (second) {
        second->place(traits::get_second_place(sz, split_pos), IF_WIN32_ELSE(true, false));
      }
      if (slider) {
        slider->place(traits::get_slider_place(sz, split_pos), IF_WIN32_ELSE(true, false));
      }
    }

    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<orientation_t O, typename F, typename S>
    inline split_view<O, F, S>::split_view () {
      init();
      super::get_layout().set(&first, &second, &slider);
    }

    template<orientation_t O, typename F, typename S>
    inline split_view<O, F, S>::split_view (split_view&& rhs) noexcept
      : super(std::move(rhs))
      , slider(std::move(rhs.slider))
      , first(std::move(rhs.first))
      , second(std::move(rhs.second))
    {
      init();
      super::get_layout().set(&first, &second, &slider);
    }

    template<orientation_t O, typename F, typename S>
    inline split_view<O, F, S>::split_view (F&& first, S&& second)
      : first(std::move(first))
      , second(std::move(second))
    {
      init();
      super::get_layout().set(&(this->first), &(this->second), &(this->slider));
    }

    template<orientation_t O, typename F, typename S>
    void split_view<O, F, S>::create (win::container& parent,
                                      const core::rectangle& place,
                                      double split_pos) {
      super::create(clazz::get(), parent, place);
      set_split_pos(split_pos);

      slider.create(*this, layout_type::traits::get_slider_place(place, split_pos));
      first.create(*this, layout_type::traits::get_first_place(place, split_pos));
      second.create(*this, layout_type::traits::get_second_place(place, split_pos));
      slider.set_visible();
      first.set_visible();
      second.set_visible();
    }

    template<orientation_t O, typename F, typename S>
    inline double split_view<O, F, S>::get_split_pos () const {
      return super::get_layout().get_split_pos(slider.position(), super::client_size());
    }

    template<orientation_t O, typename F, typename S>
    inline void split_view<O, F, S>::set_split_pos (double pos) {
      super::get_layout().set_split_pos(pos);
      super::layout();
    }

    template<orientation_t O, typename F, typename S>
    void split_view<O, F, S>::init () {
      slider.on_slide([&] (int) {
        super::get_layout().set_split_pos(get_split_pos());
        super::layout(super::client_area());
      });
      super::on_layout([&] (const core::rectangle& r) {
        super::get_layout().layout(r);
      });
    }

    // --------------------------------------------------------------------------

  } // ctrl

} // gui

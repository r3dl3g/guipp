/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
    inline split_view_data::split_view_data ()
      : first(nullptr)
      , second(nullptr)
      , slider(nullptr)
      , split_pos(0.5)
    {}

    // --------------------------------------------------------------------------
    template<orientation_t O>
    inline win::window* split_view<O>::get_first () const {
      return data.first;
    }

    template<orientation_t O>
    inline void split_view<O>::set_first (win::window* first) {
      data.first = first;
    }

    template<orientation_t O>
    inline win::window* split_view<O>::get_second () const {
      return data.second;
    }

    template<orientation_t O>
    inline void split_view<O>::set_second (win::window* second) {
      data.second = second;
    }

    template<orientation_t O>
    inline ctrl::detail::slider_base* split_view<O>::get_slider () const {
      return data.slider;
    }

    template<orientation_t O>
    inline void split_view<O>::set_slider (ctrl::detail::slider_base* slider) {
      data.slider = slider;
    }

    template<orientation_t O>
    inline void split_view<O>::set_split_pos (double pos) {
      data.split_pos = pos;
    }

    template<orientation_t O>
    inline void split_view<O>::set (win::window* first,
                                    win::window* second,
                                    ctrl::detail::slider_base* slider) {
      data.first = first;
      data.second = second;
      data.slider = slider;
    }

    template<orientation_t O>
    void split_view<O>::layout (const core::rectangle& sz) const {
      double pos = data.split_pos;//get_split_pos(sz);
      clog::trace() << "split_view::layout(" << sz << ") split_pos: " << pos;
      if (data.first) {
        data.first->place(get_first_place(sz, pos), IF_WIN32_ELSE(true, false));
      }
      if (data.second) {
        data.second->place(get_second_place(sz, pos), IF_WIN32_ELSE(true, false));
      }
      if (data.slider) {
        data.slider->place(get_slider_place(sz, pos), IF_WIN32_ELSE(true, false));
      }
    }

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT double split_view<orientation_t::vertical>::get_split_pos (const core::rectangle&) const;

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation_t::vertical>::get_first_place (const core::rectangle&,
                                                                        double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation_t::vertical>::get_second_place (const core::rectangle&,
                                                                         double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation_t::vertical>::get_slider_place (const core::rectangle&, double);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT double split_view<orientation_t::horizontal>::get_split_pos (const core::rectangle&) const;

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation_t::horizontal>::get_first_place (const core::rectangle&,
                                                                          double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation_t::horizontal>::get_second_place (const core::rectangle&,
                                                                           double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation_t::horizontal>::get_slider_place (const core::rectangle&, double);
    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    namespace detail {

      template<orientation_t O>
      inline split_view<O>::split_view () {
        init();
      }

      template<orientation_t O>
      inline split_view<O>::split_view (split_view&& rhs)
        : super(std::move(rhs))
        , slider(std::move(rhs.slider))
      {
        init();
      }

      template<orientation_t O>
      void split_view<O>::create (const win::container& parent,
                                  const core::rectangle& place,
                                  double split_pos) {
        super::create(clazz::get(), parent, place);
        super::get_layout().set_split_pos(split_pos);
        slider.create(*this, layout_type::get_slider_place(place, split_pos));
        slider.set_visible();
      }

      template<orientation_t O>
      inline double split_view<O>::get_split_pos () const {
        return super::get_layout().get_split_pos(super::client_area());
      }

      template<orientation_t O>
      inline void split_view<O>::set_split_pos (double pos) {
        super::get_layout().set_split_pos(pos);
        super::layout();
      }

      // --------------------------------------------------------------------------
      template<orientation_t O>
      void split_view<O>::init () {
        slider.on_slide([&] (int) {
          super::get_layout().set_split_pos(get_split_pos());
          super::get_layout().layout(super::client_area());
        });
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view () {
      super::get_layout().set(&first, &second, &(super::slider));
    }

    template<orientation_t O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view (basic_split_view&& rhs)
      : super(std::move(rhs))
      , first(std::move(rhs.first))
      , second(std::move(rhs.second))
    {
      super::get_layout().set(&first, &second, &(super::slider));
    }

    template<orientation_t O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view (F&& first, S&& second)
      : first(std::move(first))
      , second(std::move(second))
    {
      super::get_layout().set(&(this->first), &(this->second), &(super::slider));
    }

    template<orientation_t O, typename F, typename S>
    void basic_split_view<O, F, S>::create (const win::container& parent,
                                            const core::rectangle& place,
                                            double split_pos) {
      super::create(parent, place, split_pos);
      core::rectangle sz(place.size());
      first.create(*this, layout_type::get_first_place(sz, split_pos));
      first.set_visible();
      second.create(*this, layout_type::get_second_place(sz, split_pos));
      second.set_visible();
    }

  } // ctrl

} // gui

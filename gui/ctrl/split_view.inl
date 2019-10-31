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
    {}

    // --------------------------------------------------------------------------
    template<orientation O>
    inline split_view<O>::split_view (win::container* m)
      : super(m)
    {
      init();
    }

    template<orientation O>
    inline split_view<O>::split_view (win::container* m, const split_view& rhs)
      : super(m, rhs)
    {
      init();
    }

    template<orientation O>
    inline split_view<O>::split_view (win::container* m, split_view&& rhs)
      : super(m, std::move(rhs))
    {
      init();
    }

    template<orientation O>
    inline win::window* split_view<O>::get_first () const {
      return data.first;
    }

    template<orientation O>
    inline void split_view<O>::set_first (win::window* first) {
      data.first = first;
    }

    template<orientation O>
    inline win::window* split_view<O>::get_second () const {
      return data.second;
    }

    template<orientation O>
    inline void split_view<O>::set_second (win::window* second) {
      data.second = second;
    }

    template<orientation O>
    inline ctrl::detail::slider_base* split_view<O>::get_slider () const {
      return data.slider;
    }

    template<orientation O>
    inline void split_view<O>::set_slider (ctrl::detail::slider_base* slider) {
      data.slider = slider;
    }

    template<orientation O>
    inline void split_view<O>::set (win::window* first,
                                    win::window* second,
                                    ctrl::detail::slider_base* slider) {
      data.first = first;
      data.second = second;
      data.slider = slider;
    }

    template<orientation O>
    void split_view<O>::layout (const core::rectangle& sz) {
      double pos = get_split_pos(sz);
      LogTrace << "split_view::layout(" << sz << ") split_pos: " << pos;
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

    template<orientation O>
    inline void split_view<O>::init () {
      super::init(basepp::bind_method(this, &split_view::layout));
    }

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT double split_view<orientation::vertical>::get_split_pos (const core::rectangle&) const;

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation::vertical>::get_first_place (const core::rectangle&,
                                                                        double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation::vertical>::get_second_place (const core::rectangle&,
                                                                         double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation::vertical>::get_slider_place (const core::rectangle&, double);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT double split_view<orientation::horizontal>::get_split_pos (const core::rectangle&) const;

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation::horizontal>::get_first_place (const core::rectangle&,
                                                                          double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation::horizontal>::get_second_place (const core::rectangle&,
                                                                           double);

    template<>
    GUIPP_CTRL_EXPORT core::rectangle split_view<orientation::horizontal>::get_slider_place (const core::rectangle&, double);
    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    namespace detail {

      template<orientation O>
      inline split_view<O>::split_view () {
        init();
      }

      template<orientation O>
      inline split_view<O>::split_view (const split_view& rhs)
        : super(rhs)
        , slider(rhs.slider)
      {
        init();
      }

      template<orientation O>
      inline split_view<O>::split_view (split_view&& rhs)
        : super(std::move(rhs))
        , slider(std::move(rhs.slider))
      {
        init();
      }

      template<orientation O>
      void split_view<O>::create (const win::container& parent,
                                  const core::rectangle& place,
                                  double split_pos) {
        super::create(clazz::get(), parent, place);
        slider.create(*this, layout_type::get_slider_place(place, split_pos));
        slider.set_visible();
      }

      template<orientation O>
      inline double split_view<O>::get_split_pos () const {
        return super::get_layout().get_split_pos(super::client_area());
      }

      template<orientation O>
      inline void split_view<O>::set_split_pos (double pos) {
        slider.place(layout_type::get_slider_place(super::client_area(), pos));
        super::layout();
      }

      // --------------------------------------------------------------------------
      template<orientation O>
      void split_view<O>::init () {
        slider.on_slide([&] (int) {
          super::get_layout().layout(super::client_area());
        });
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view () {
      super::get_layout().set(&first, &second, &(super::slider));
    }

    template<orientation O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view (const basic_split_view& rhs)
      : super(rhs)
      , first(rhs.first)
      , second(rhs.second)
    {
      super::get_layout().set(&first, &second, &(super::slider));
    }

    template<orientation O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view (basic_split_view&& rhs)
      : super(std::move(rhs))
      , first(std::move(rhs.first))
      , second(std::move(rhs.second))
    {
      super::get_layout().set(&first, &second, &(super::slider));
    }

    template<orientation O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view (F&& first, S&& second)
      : first(std::move(first))
      , second(std::move(second))
    {
      super::get_layout().set(&(this->first), &(this->second), &(super::slider));
    }

    template<orientation O, typename F, typename S>
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

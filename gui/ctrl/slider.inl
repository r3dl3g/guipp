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
 * @brief     C++ API: slider control
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      inline auto slider_base::get_min() const -> type {
        return min;
      }

      inline auto slider_base::get_max() const -> type {
        return max;
      }

      template<orientation O>
      inline void basic_slider<O>::create (const container& parent,
                                           const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
      }

      // --------------------------------------------------------------------------
      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation::vertical>::basic_slider ();

      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation::vertical>::basic_slider (const basic_slider&);

      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation::vertical>::basic_slider (basic_slider&&);

      template<>
      GUIPP_CTRL_EXPORT void basic_slider<orientation::vertical>::set_value (core::point::type v);

      template<>
      GUIPP_CTRL_EXPORT core::point::type basic_slider<orientation::vertical>::get_value () const;

      template<>
      GUIPP_CTRL_EXPORT void basic_slider<orientation::vertical>::init ();

      // --------------------------------------------------------------------------
      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation::horizontal>::basic_slider ();

      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation::horizontal>::basic_slider (const basic_slider&);

      template<>
      GUIPP_CTRL_EXPORT basic_slider<orientation::horizontal>::basic_slider (basic_slider&&);

      template<>
      GUIPP_CTRL_EXPORT void basic_slider<orientation::horizontal>::set_value (core::point::type v);

      template<>
      GUIPP_CTRL_EXPORT core::point::type basic_slider<orientation::horizontal>::get_value () const;

      template<>
      GUIPP_CTRL_EXPORT void basic_slider<orientation::horizontal>::init ();

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation O,
             draw::frame::drawer F>
    inline basic_framed_slider<O, F>::basic_framed_slider () {
      init();
    }

    template<orientation O,
             draw::frame::drawer F>
    inline basic_framed_slider<O, F>::basic_framed_slider (const basic_framed_slider& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation O,
             draw::frame::drawer F>
    inline basic_framed_slider<O, F>::basic_framed_slider (basic_framed_slider&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation O,
             draw::frame::drawer F>
      void basic_framed_slider<O, F>::init () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &basic_framed_slider::paint));
    }

    template<orientation O,
             draw::frame::drawer F>
    void basic_framed_slider<O, F>::paint (const draw::graphics& g) {
      core::rectangle place = super::client_area();
      g.fill(draw::rectangle(place), color::buttonColor());
      F(g, place);
      if (super::has_focus()) {
        draw::frame::dots(g, place);
      }
    }

  } // win

} // gui

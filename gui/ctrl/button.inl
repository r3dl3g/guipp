/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://wwrothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: button
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    inline void button_base::create (const win::container& parent,
                                     const core::rectangle& place) {
      window::create(clazz::get(), parent, place);
    }

    inline const button_state button_base::get_state () const {
      return button_state(*this);
    }

    inline button_state button_base::get_state () {
      return button_state(*this);
    }

    inline bool button_base::is_hilited () const {
      return get_state().is_hilited();
    }

    inline bool button_base::is_pushed () const {
      return get_state().is_pushed();
    }

    inline bool button_base::is_checked () const {
      return get_state().is_checked();
    }

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void toggle_button_traits<false>::init (button_base&);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void toggle_button_traits<true>::init (button_base&);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void animated_button_traits<false>::init (button_base&);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void animated_button_traits<true>::init (button_base&);

    // --------------------------------------------------------------------------
    template<class T>
    inline basic_button<T>::basic_button () {
      traits.init(*this);
    }

    template<class T>
    inline basic_button<T>::basic_button (const basic_button& rhs)
      : super(rhs) {
      traits.init(*this);
    }

    template<class T>
    inline basic_button<T>::basic_button (basic_button&& rhs)
      : super(std::move(rhs)) {
      traits.init(*this);
    }

    // --------------------------------------------------------------------------
    template<class T, typename U, U D>
    inline basic_text_button<T, U, D>::basic_text_button (const text_source& t)
      : text(t)
    {
      init();
    }

    template<class T, typename U, U D>
    inline basic_text_button<T, U, D>::basic_text_button (const basic_text_button& rhs)
      : super(rhs)
      , text(rhs.text)
    {
      init();
    }

    template<class T, typename U, U D>
    inline basic_text_button<T, U, D>::basic_text_button (basic_text_button&& rhs)
      : super(std::move(rhs))
      , text(std::move(rhs.text))
    {
      init();
    }

    template<class T, typename U, U D>
    inline void basic_text_button<T, U, D>::create (const win::container& parent,
                                                    const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T, typename U, U D>
    inline void basic_text_button<T, U, D>::create (const win::container& parent,
                                                    const std::string& txt,
                                                    const core::rectangle& place) {
      create(parent, const_text(txt), place);
    }

    template<class T, typename U, U D>
    inline void basic_text_button<T, U, D>::create (const win::container& parent,
                                                    const text_source& txt,
                                                    const core::rectangle& place) {
      super::create(parent, place);
      set_text(txt);
    }

    template<class T, typename U, U D>
    inline void basic_text_button<T, U, D>::set_text (const std::string& t) {
      set_text(const_text(t));
    }

    template<class T, typename U, U D>
    inline void basic_text_button<T, U, D>::set_text (const text_source& t) {
      text = t;
      super::redraw();
    }

    template<class T, typename U, U D>
    inline std::string basic_text_button<T, U, D>::get_text () const {
      return text();
    }

    template<class T, typename U, U D>
    void basic_text_button<T, U, D>::init () {
      super::on_paint(draw::paint([&](const draw::graphics & graph) {
        auto r = super::client_area();
        auto t = get_text();
        auto s = super::get_state();
        super::traits.template draw<D>(graph, r, t, s);
      }));
    }

    // --------------------------------------------------------------------------
    template<class T>
    inline custom_button<T>::custom_button () {
      init();
    }

    template<class T>
    inline custom_button<T>::custom_button (const custom_button& rhs)
      : super(rhs)
    {
      init();
    }

    template<class T>
    inline custom_button<T>::custom_button (custom_button&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<class T>
    inline void custom_button<T>::create (const win::container& parent,
                                          const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T>
    inline void custom_button<T>::set_drawer (std::function<button_drawer> d) {
      drawer = d;
    }

    template<class T>
    void custom_button<T>::init () {
      super::on_paint(draw::paint([&] (const draw::graphics & graph) {
        if (drawer) {
          drawer(graph, super::client_area(), super::get_state());
        }
      }));
    }

  } // namespace ctrl

} // namespace gui

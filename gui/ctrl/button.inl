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

  namespace win {

    inline button_state::button_state (const button_base& win)
      : super(win)
    {}

    // --------------------------------------------------------------------------
    inline void button_base::create (const container& parent,
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
    void toggle_button_traits<false>::init (button_base&);

    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<true>::init (button_base&);

    // --------------------------------------------------------------------------
    template<>
    void animated_button_traits<false>::init (animated_button&);

    // --------------------------------------------------------------------------
    template<>
    void animated_button_traits<true>::init (animated_button&);

    // --------------------------------------------------------------------------
    template<class T>
    inline basic_button<T>::basic_button () {
      traits::init(*this);
    }

    template<class T>
    inline basic_button<T>::basic_button (const basic_button& rhs)
      : super(rhs) {
      traits::init(*this);
    }

    template<class T>
    inline basic_button<T>::basic_button (basic_button&& rhs)
      : super(std::move(rhs)) {
      traits::init(*this);
    }

    // --------------------------------------------------------------------------
    template<class T>
    inline text_source_button<T>::text_source_button (const text_source& t)
      : text(t)
    {}

    template<class T>
    inline void text_source_button<T>::set_text (const std::string& t) {
      set_text(const_text(t));
    }

    template<class T>
    inline void text_source_button<T>::set_text (const text_source& t) {
      text = t;
      super::redraw_later();
    }

    template<class T>
    inline std::string text_source_button<T>::get_text () const {
      return text();
    }

    // --------------------------------------------------------------------------
    template<class T, text_button_drawer D>
    inline basic_text_button<T, D>::basic_text_button (const text_source& t)
      : super(t)
    {
      init();
    }

    template<class T, text_button_drawer D>
    inline basic_text_button<T, D>::basic_text_button (const basic_text_button& rhs)
      : super(rhs) {
      init();
    }

    template<class T, text_button_drawer D>
    inline basic_text_button<T, D>::basic_text_button (basic_text_button&& rhs)
      : super(std::move(rhs)) {
      init();
    }

    template<class T, text_button_drawer D>
    inline void basic_text_button<T, D>::create (const container& parent,
                                                 const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T, text_button_drawer D>
    inline void basic_text_button<T, D>::create (const container& parent,
                                                 const std::string& txt,
                                                 const core::rectangle& place) {
      create(parent, const_text(txt), place);
    }

    template<class T, text_button_drawer D>
    inline void basic_text_button<T, D>::create (const container& parent,
                                                 const text_source& txt,
                                                 const core::rectangle& place) {
      super::create(parent, place);
      super::set_text(txt);
    }

    template<class T, text_button_drawer D>
    void basic_text_button<T, D>::init () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics & graph) {
        D(graph, super::client_area(), super::get_text(), super::get_state());
      }));
    }

    // --------------------------------------------------------------------------
    template<class T>
    inline animated_button_base<T>::animated_button_base () {
      traits::init(*this);
    }

    template<class T>
    inline animated_button_base<T>::animated_button_base (const animated_button_base& rhs)
      : super(rhs) {
      traits::init(*this);
    }

    template<class T>
    inline animated_button_base<T>::animated_button_base (animated_button_base&& rhs)
      : super(std::move(rhs)) {
      traits::init(*this);
    }
    // --------------------------------------------------------------------------
    template<class T, animated_text_button_drawer D>
    inline animated_text_button<T, D>::animated_text_button (const text_source& t)
      : text(t)
    {
      init();
    }

    template<class T, animated_text_button_drawer D>
    inline animated_text_button<T, D>::animated_text_button (const animated_text_button& rhs)
      : super(rhs) {
      init();
    }

    template<class T, animated_text_button_drawer D>
    inline animated_text_button<T, D>::animated_text_button (animated_text_button&& rhs)
      : super(std::move(rhs)) {
      init();
    }

    template<class T, animated_text_button_drawer D>
    inline void animated_text_button<T, D>::create (const container& parent,
                                                    const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T, animated_text_button_drawer D>
    inline void animated_text_button<T, D>::create (const container& parent,
                                                    const std::string& txt,
                                                    const core::rectangle& place) {
      create(parent, const_text(txt), place);
    }

    template<class T, animated_text_button_drawer D>
    inline void animated_text_button<T, D>::create (const container& parent,
                                                    const text_source& txt,
                                                    const core::rectangle& place) {
      super::create(parent, place);
      set_text(txt);
    }

    template<class T, animated_text_button_drawer D>
    inline void animated_text_button<T, D>::set_text (const std::string& t) {
      set_text(const_text(t));
    }

    template<class T, animated_text_button_drawer D>
    inline void animated_text_button<T, D>::set_text (const text_source& t) {
      text = t;
      super::redraw_later();
    }

    template<class T, animated_text_button_drawer D>
    inline std::string animated_text_button<T, D>::get_text () const {
      return text();
    }

    template<class T, animated_text_button_drawer D>
    void animated_text_button<T, D>::init () {
      super::register_event_handler(REGISTER_FUNCTION,
                                    paint_event(draw::buffered_paint([&](const draw::graphics & graph) {
        D(graph, super::client_area(), get_text(), super::get_state(), super::animation_step);
      })));
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
    inline void custom_button<T>::create (const container& parent,
                                          const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T>
    inline void custom_button<T>::set_drawer (std::function<button_drawer> d) {
      drawer = d;
    }

    template<class T>
    void custom_button<T>::init () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event([&] (const draw::graphics & graph) {
        if (drawer) {
          drawer(graph, super::client_area(), super::get_state());
        }
      }));
    }

    // --------------------------------------------------------------------------

  } // win

} // gui

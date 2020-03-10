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
      init();
    }

    template<class T>
    inline basic_button<T>::basic_button (const basic_button& rhs)
      : super(rhs) {
      traits.init(*this);
      init();
    }

    template<class T>
    inline basic_button<T>::basic_button (basic_button&& rhs)
      : super(std::move(rhs)) {
      traits.init(*this);
      init();
    }

    template<class T>
    void basic_button<T>::set_hilited (bool b) {
      traits.set_hilited(*this, b);
    }

    template<class T>
    void basic_button<T>::set_pushed (bool b) {
      traits.set_pushed(*this, b);
    }

    template<class T>
    void basic_button<T>::set_checked (bool b) {
      traits.set_checked(*this, b);
    }

    template<class T>
    void basic_button<T>::init () {
#ifdef X11
      static int initialized = detail::init_control_messages();
      (void)initialized;
#endif // X11

      set_accept_focus(true);

      using namespace win;
      super::register_event_handler(event_handler_function([&] (const core::event& e, gui::os::event_result& r) {
        if (set_focus_event::match(e) || lost_focus_event::match(e)) {
          window::invalidate();
        } else if (mouse_enter_event::match(e)) {
          set_hilited(true);
        } else if (mouse_leave_event::match(e)) {
          set_hilited(false);
        } else if (any_key_down_event::match(e)) {
          os::key_symbol k = get_key_symbol(e);
          if ((k == win::keys::enter) || (k == win::keys::space)) {
            set_pushed(true);
          }
        } else if (left_btn_down_event::match(e) && is_enabled()) {
          take_focus();
          set_pushed(true);
        }
        return false;
      }), static_cast<os::event_id>(set_focus_event::mask | lost_focus_event::mask | mouse_enter_event::mask | mouse_leave_event::mask | any_key_down_event::mask | left_btn_down_event::mask));

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
      super::invalidate();
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

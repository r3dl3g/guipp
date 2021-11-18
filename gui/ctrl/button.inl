/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
    inline void button_base::create (win::container& parent,
                                     const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
    }

    inline core::button_state::is button_base::get_state () const {
      return core::button_state::is(flags);
    }

    inline core::button_state::set button_base::set_state () {
      return core::button_state::set(flags);
    }

    inline bool button_base::is_hilited () const {
      return get_state().hilited();
    }

    inline bool button_base::is_pushed () const {
      return get_state().pushed();
    }

    inline bool button_base::is_checked () const {
      return get_state().checked();
    }

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT toggle_button_traits<false>::toggle_button_traits (button_base&);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT toggle_button_traits<true>::toggle_button_traits (button_base&);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT animated_button_traits<false>::animated_button_traits (button_base&);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT animated_button_traits<true>::animated_button_traits (button_base&);

    // --------------------------------------------------------------------------
    template<class T>
    inline basic_button<T>::basic_button ()
      : traits(*this) {
      init();
    }

    template<class T>
    inline basic_button<T>::basic_button (const basic_button& rhs)
      : super(rhs)
      , traits(*this) {
      init();
    }

    template<class T>
    inline basic_button<T>::basic_button (basic_button&& rhs) noexcept
      : super(std::move(rhs))
      , traits(*this) {
      init();
    }

    template<class T>
    void basic_button<T>::init () {
#ifdef GUIPP_X11
      static int initialized = detail::init_control_messages();
      (void)initialized;
#endif // GUIPP_X11

      using namespace win;
      super::on_set_focus([&] () { invalidate(); });
      super::on_lost_focus([&] () { invalidate(); });
      super::on_mouse_enter([&] () { set_hilited(true); });
      super::on_mouse_leave([&] () { set_hilited(false); });
      super::on_key_down<core::keys::enter>([&] () { set_pushed(true); });
      super::on_key_down<core::keys::space>([&] () { set_pushed(true); });
      super::on_left_btn_down([&] (os::key_state, const core::native_point&) {
        take_focus();
        set_pushed(true);
      });

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
    inline basic_text_button<T, U, D>::basic_text_button (basic_text_button&& rhs) noexcept
      : super(std::move(rhs))
      , text(std::move(rhs.text))
    {
      init();
    }

    template<class T, typename U, U D>
    inline void basic_text_button<T, U, D>::create (win::container& parent,
                                                    const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T, typename U, U D>
    inline void basic_text_button<T, U, D>::create (win::container& parent,
                                                    const std::string& txt,
                                                    const core::rectangle& place) {
      create(parent, const_text(txt), place);
    }

    template<class T, typename U, U D>
    inline void basic_text_button<T, U, D>::create (win::container& parent,
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
      super::on_paint(draw::paint([&](draw::graphics& graph) {
        auto r = super::client_geometry();
        auto t = get_text();
        auto s = super::get_state();
        super::traits.template draw<D>(graph, r, t, s);
      }));
    }

    // --------------------------------------------------------------------------
    template<class T, draw::icon_t I, os::color F, os::color B>
    inline icon_button<T, I, F, B>::icon_button () {
      init();
    }

    template<class T, draw::icon_t I, os::color F, os::color B>
    inline icon_button<T, I, F, B>::icon_button (const icon_button& rhs)
      : super(rhs)
    {
      init();
    }

    template<class T, draw::icon_t I, os::color F, os::color B>
    inline icon_button<T, I, F, B>::icon_button (icon_button&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<class T, draw::icon_t I, os::color F, os::color B>
    void icon_button<T, I, F, B>::init () {
      super::on_paint(draw::paint([&] (draw::graphics& g) {
        const auto r = super::client_geometry();
        const auto st = super::get_state();
        g.erase(r, look::get_button_background(st, B));
        g.frame(draw::icon<I>(r.center(), r.max_radius() / 2), look::get_button_foreground(st, F, B));
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
    inline custom_button<T>::custom_button (custom_button&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<class T>
    inline void custom_button<T>::set_drawer (std::function<button_drawer> d) {
      drawer = d;
    }

    template<class T>
    void custom_button<T>::init () {
      super::on_paint(draw::paint([&] (draw::graphics&  graph) {
        if (drawer) {
          drawer(graph, super::client_geometry(), super::get_state());
        }
      }));
    }

  } // namespace ctrl

} // namespace gui

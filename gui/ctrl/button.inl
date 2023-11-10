/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://wwrothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     various button controls
 *
 * @license   MIT license. See accompanying file LICENSE.
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
    template<class T, button_drawer BG>
    inline basic_button<T, BG>::basic_button (os::color f)
      : super(f)
      , traits(*this) {
      init();
    }

    template<class T, button_drawer BG>
    inline basic_button<T, BG>::basic_button (const basic_button& rhs)
      : super(rhs)
      , traits(*this) {
      init();
    }

    template<class T, button_drawer BG>
    inline basic_button<T, BG>::basic_button (basic_button&& rhs) noexcept
      : super(std::move(rhs))
      , traits(*this) {
      init();
    }

    template<class T, button_drawer BG>
    void basic_button<T, BG>::init () {
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
      super::on_paint(draw::paint([&](draw::graphics& graph) {
        BG(graph, super::client_geometry(), super::get_state(), super::get_foreground(), super::get_background());
      }));

    }

    // --------------------------------------------------------------------------
    template<class T, button_drawer BG, text_button_drawer D>
    inline basic_text_button<T, BG, D>::basic_text_button (const text_source& t)
      : text(t)
    {
      init();
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline basic_text_button<T, BG, D>::basic_text_button (const std::string& t)
      : text(const_text(t))
    {
      init();
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline basic_text_button<T, BG, D>::basic_text_button (const basic_text_button& rhs)
      : super(rhs)
      , text(rhs.text)
    {
      init();
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline basic_text_button<T, BG, D>::basic_text_button (basic_text_button&& rhs) noexcept
      : super(std::move(rhs))
      , text(std::move(rhs.text))
    {
      init();
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline void basic_text_button<T, BG, D>::create (win::container& parent,
                                                    const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline void basic_text_button<T, BG, D>::create (win::container& parent,
                                                    const std::string& txt,
                                                    const core::rectangle& place) {
      create(parent, const_text(txt), place);
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline void basic_text_button<T, BG, D>::create (win::container& parent,
                                                    const text_source& txt,
                                                    const core::rectangle& place) {
      super::create(parent, place);
      set_text(txt);
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline void basic_text_button<T, BG, D>::set_text (const std::string& t) {
      set_text(const_text(t));
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline void basic_text_button<T, BG, D>::set_text (const text_source& t) {
      text = t;
      super::invalidate();
    }

    template<class T, button_drawer BG, text_button_drawer D>
    inline std::string basic_text_button<T, BG, D>::get_text () const {
      return text();
    }

    template<class T, button_drawer BG, text_button_drawer D>
    void basic_text_button<T, BG, D>::init () {
      super::set_foreground(color::buttonTextColor());
      super::on_paint(draw::paint([&](draw::graphics& graph) {
        D(graph, super::client_geometry(), get_text(), super::get_state(),
          super::get_foreground(), super::get_background());
      }));
    }

    // --------------------------------------------------------------------------
    template<class T, draw::icon_type I, button_drawer BG>
    inline icon_button_t<T, I, BG>::icon_button_t (os::color f)
      : super(f) {
      init();
    }

    template<class T, draw::icon_type I, button_drawer BG>
    inline icon_button_t<T, I, BG>::icon_button_t (const icon_button_t& rhs)
      : super(rhs)
    {
      init();
    }

    template<class T, draw::icon_type I, button_drawer BG>
    inline icon_button_t<T, I, BG>::icon_button_t (icon_button_t&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<class T, draw::icon_type I, button_drawer BG>
    void icon_button_t<T, I, BG>::init () {
      super::set_background(color::dark_gray);
      super::on_paint(draw::paint([&] (draw::graphics& g) {
        const auto r = super::client_geometry();
        const auto st = super::get_state();
        g.frame(draw::icon_t<I>(r.center(), r.max_radius() * 2 / 3),
                look::get_flat_button_foreground(st, super::get_foreground(), super::get_background()));
      }));
    }

    // --------------------------------------------------------------------------
    template<class T, button_drawer BG>
    inline icon_button<T, BG>::icon_button (draw::icon_type icon, os::color f)
      : super(f)
      , icon(icon) {
      super::set_background(color::dark_gray);
      init();
    }

    template<class T, button_drawer BG>
    inline icon_button<T, BG>::icon_button (const icon_button& rhs)
      : super(rhs)
      , icon(rhs.icon) {
      init();
    }

    template<class T, button_drawer BG>
    inline icon_button<T, BG>::icon_button (icon_button&& rhs) noexcept
      : super(std::move(rhs))
      , icon(rhs.icon) {
      init();
    }

    template<class T, button_drawer BG>
    void icon_button<T, BG>::set_icon (draw::icon_type i) {
      icon = i;
    }

    template<class T, button_drawer BG>
    draw::icon_type icon_button<T, BG>::get_icon () const {
      return icon;
    }

    template<class T, button_drawer BG>
    void icon_button<T, BG>::init () {
      super::on_paint(draw::paint([&] (draw::graphics& g) {
        const auto r = super::client_geometry();
        const auto st = super::get_state();
        g.frame(draw::icon(icon, r.center(), r.max_radius() * 2 / 3),
                look::get_flat_button_foreground(st, super::get_foreground(), super::get_background()));
      }));
    }

    // --------------------------------------------------------------------------
    template<class T, button_drawer BG>
    inline custom_button<T, BG>::custom_button () {
      init();
    }

    template<class T, button_drawer BG>
    inline custom_button<T, BG>::custom_button (const custom_button& rhs)
      : super(rhs)
    {
      init();
    }

    template<class T, button_drawer BG>
    inline custom_button<T, BG>::custom_button (custom_button&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<class T, button_drawer BG>
    inline void custom_button<T, BG>::set_drawer (std::function<button_drawer> d) {
      drawer = d;
    }

    template<class T, button_drawer BG>
    void custom_button<T, BG>::init () {
      super::on_paint(draw::paint([&] (draw::graphics&  graph) {
        if (drawer) {
          drawer(graph, super::client_geometry(), super::get_state(),
                 super::get_foreground(), super::get_background());
        }
      }));
    }

    // --------------------------------------------------------------------------
    template<orientation_t H, typename T>
    inline button_pair<H, T>::button_pair () {
      traits::add(super::get_layout(), plus, minus);
      super::on_create([&] () {
        plus.create(*this);
        minus.create(*this);
      });
    }

    template<orientation_t H, typename T>
    inline void button_pair<H, T>::on_change (sign_fn f) {
      plus.on_clicked([=] () { f(1); });
      minus.on_clicked([=] () { f(-1); });
    }

    // --------------------------------------------------------------------------
  } // namespace ctrl

} // namespace gui

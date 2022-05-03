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
 * @brief     separator view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename T, typename F, typename B, typename L>
    inline number_edit<T, F, B, L>::number_edit (const T& v, T i)
      : increment(i)
      , edit(v)
    {
      init();
    }

    template<typename T, typename F, typename B, typename L>
    inline number_edit<T, F, B, L>::number_edit (std::reference_wrapper<T> d, T i)
      : increment(i)
      , edit(d)
    {
      init();
    }

    template<typename T, typename F, typename B, typename L>
    inline number_edit<T, F, B, L>::number_edit (data d, T i)
      : increment(i)
      , edit(d)
    {
      init();
    }

    template<typename T, typename F, typename B, typename L>
    void number_edit<T, F, B, L>::init () {
      super::get_layout().set_header_and_body(layout::lay(buttons), layout::lay(edit));
      super::on_create([&] () {
        edit.create(*this);
        buttons.create(*this);
      });
      buttons.on_change([&] (int sign) {
        step(increment * sign);
      });
      edit.template on_key_down<core::keys::up>([&] () { inc(); });
      edit.template on_key_down<core::keys::down>([&] () { dec(); });
      edit.on_wheel_y([&] (core::native_point::type delta, core::native_point) {
        step(-static_cast<int>(delta) * increment);
      });
    }

    template<typename T, typename F, typename B, typename L>
    inline void number_edit<T, F, B, L>::set (T v) {
      edit.set(v);
      super::notify_content_changed();
    }

    template<typename T, typename F, typename B, typename L>
    inline T number_edit<T, F, B, L>::get () const {
      return edit.get();
    }

    template<typename T, typename F, typename B, typename L>
    inline void number_edit<T, F, B, L>::step (T i) {
      set(get() + i);
    }

    template<typename T, typename F, typename B, typename L>
    inline void number_edit<T, F, B, L>::inc () {
      step(increment);
    }

    template<typename T, typename F, typename B, typename L>
    inline void number_edit<T, F, B, L>::dec () {
      step(-increment);
    }
    // --------------------------------------------------------------------------

  } // ctrl

} // gui

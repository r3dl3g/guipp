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
    template<typename T, typename F, typename M, typename B, typename L>
    inline number_edit<T, F, M, B, L>::number_edit (T v, type i, limiter_type l)
      : increment(i)
      , edit(std::forward<T>(v), std::forward<M>(l))
    {
      init();
    }

    template<typename T, typename F, typename M, typename B, typename L>
    void number_edit<T, F, M, B, L>::init () {
      super::get_layout().set_header_and_body(layout::lay(buttons), layout::lay(edit));
      super::on_create([this] () {
        // visual studio throws an error without this-> (compiler bug?)
        this->edit.create(*this);
        this->buttons.create(*this);
      });
      buttons.on_change([&] (int sign) {
        step(increment * sign);
      });
      edit.template on_key_down<core::keys::up>([&] () { inc(); });
      edit.template on_key_down<core::keys::down>([&] () { dec(); });
      edit.on_wheel_y([&] (core::native_point::type delta, core::native_point) {
        step(-(increment * delta));
      });
    }

    template<typename T, typename F, typename M, typename B, typename L>
    inline void number_edit<T, F, M, B, L>::step (const type& i) {
      set(get() + i);
    }

    template<typename T, typename F, typename M, typename B, typename L>
    inline void number_edit<T, F, M, B, L>::set (const type& i) {
      edit.set(i);
    }

    template<typename T, typename F, typename M, typename B, typename L>
    inline auto number_edit<T, F, M, B, L>::get () const -> type {
      return edit.get();
    }

    template<typename T, typename F, typename M, typename B, typename L>
    inline void number_edit<T, F, M, B, L>::inc () {
      step(increment);
    }

    template<typename T, typename F, typename M, typename B, typename L>
    inline void number_edit<T, F, M, B, L>::dec () {
      step(-increment);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    date_time_edit<T>::date_time_edit (T t)
      : super(t)
      , value(std::forward<T>(t))
    {
      year.edit.on_content_changed([&] () { update(); });
      month.edit.on_content_changed([&] () { update(); });
      day.edit.on_content_changed([&] () { update(); });
      hour.edit.on_content_changed([&] () { update(); });
      min.edit.on_content_changed([&] () { update(); });
      sec.edit.on_content_changed([&] () { update(); });
    }

    template<typename T>
    void date_time_edit<T>::update () {
      std::mktime(&parts);
      set(util::time::time_t2time_point(util::time::tm2time_t(parts)));
    }

    template<typename T>
    void date_time_edit<T>::set (const time_point& v) {
      if (value != v) {
        value = v;
        parts = util::time::local_time(get());
        invalidate();
        notify_content_changed();
      }
    }

    template<typename T>
    auto date_time_edit<T>::get () const -> time_point {
      return value;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    duration_edit<T>::duration_edit (T t)
      : super(t)
      , value(std::forward<T>(t))
    {
      hour.edit.on_content_changed([&] () { update(); });
      min.edit.on_content_changed([&] () { update(); });
      sec.edit.on_content_changed([&] () { update(); });
    }

    template<typename T>
    void duration_edit<T>::update () {
      set(util::time::parts2duration(parts));
    }

    template<typename T>
    void duration_edit<T>::set (const duration& v) {
      if (value != v) {
        value = v;
        parts = util::time::duration2parts(get());
        invalidate();
        notify_content_changed();
      }
    }

    template<typename T>
    auto duration_edit<T>::get () const -> duration {
      return value;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    duration_edit_ms<T>::duration_edit_ms (T t)
      : super(t)
      , micros(super::parts.micros)
    {
      super::get_layout().add(micros);
      on_create([&] () {
        micros.create(*this);
      });
      micros.edit.on_content_changed([&] () { super::update(); });
    }

    // --------------------------------------------------------------------------
  } // ctrl

} // gui

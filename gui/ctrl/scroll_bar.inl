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
 * @brief     C++ API: scroll bars
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    inline auto scroll_bar::get_range() const -> type {
      return get_max() - get_min();
    }

    // --------------------------------------------------------------------------
    template<orientation H>
    inline basic_scroll_bar<H>::basic_scroll_bar (bool grab_focus) {
      set_accept_focus(grab_focus);
      init();
    }

    template<orientation H>
    inline basic_scroll_bar<H>::basic_scroll_bar (const basic_scroll_bar& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation H>
    inline basic_scroll_bar<H>::basic_scroll_bar (basic_scroll_bar&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation H>
    inline void basic_scroll_bar<H>::create (const win::container& parent,
                                             const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::get_geometry () const -> geometry {
      core::size sz = client_size();
      type l = length(sz);
      type t = thickness(sz);
      type b = button_size(l, t);
      type s = space_size(l, b);
      type th = thumb_size(s, b);
      type sc = get_scale(s, th);
      type tt = thumb_top(b, sc);
      return {l, t, b, s, th, sc, tt};
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::get_scale() const -> type {
      return get_geometry().scale;
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::get_scale (type spc_size, type tmb_size) const->type {
      return (spc_size - tmb_size) / (get_max() - get_min());
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::button_size (type length, type thickness)->type {
      return std::min(thickness, length / type(2));
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::space_size (type length, type btn_size)->type {
      return std::max(length - btn_size * 2, type(0));
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::thumb_size (type spc_size, type btn_size) const->type {
      return std::max(get_page() * spc_size / (get_range() + get_page()), std::min(btn_size, spc_size));
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::thumb_top (type btn_size, type scale) const->type {
      return btn_size + (get_value() - get_min()) * scale;
    }

    template<orientation H>
    inline core::rectangle basic_scroll_bar<H>::up_button_place (const geometry& m) const {
      return core::rectangle(core::point::zero, build_size(m.button_size, m.thickness));
    }

    template<orientation H>
    inline core::rectangle basic_scroll_bar<H>::down_button_place (const geometry& m) const {
      return core::rectangle(build_pos(m.length - m.button_size), build_size(m.button_size, m.thickness));
    }

    template<orientation H>
    inline core::rectangle basic_scroll_bar<H>::page_up_place (const geometry& m) const {
      return core::rectangle(build_pos(m.button_size), build_size(m.thumb_top - m.button_size, m.thickness));
    }

    template<orientation H>
    inline core::rectangle basic_scroll_bar<H>::page_down_place (const geometry& m) const {
      type tmb_bottom = m.thumb_top + m.thumb_size;
      return core::rectangle(build_pos(tmb_bottom), build_size(m.length - m.button_size - tmb_bottom, m.thickness));
    }

    template<orientation H>
    inline core::rectangle basic_scroll_bar<H>::thumb_button_place (const geometry& m) const {
      return core::rectangle(build_pos(m.thumb_top), build_size(m.thumb_size, m.thickness));
    }

    template<orientation H>
    void basic_scroll_bar<H>::handle_left_btn_down (os::key_state, const core::point& pt) {
      if (is_enabled()) {
        if (can_accept_focus()) {
          take_focus();
        }
        set_last_mouse_point(pt);
        set_last_value(get_value());

        auto geo = get_geometry();

        if (up_button_place(geo).is_inside(pt)) {
          set_state(scrollbar_state::up_button);
        } else if (down_button_place(geo).is_inside(pt)) {
          set_state(scrollbar_state::down_button);
        } else if (thumb_button_place(geo).is_inside(pt)) {
          set_state(scrollbar_state::thumb_button);
        } else if (page_up_place(geo).is_inside(pt)) {
          set_state(scrollbar_state::page_up);
        } else if (page_down_place(geo).is_inside(pt)) {
          set_state(scrollbar_state::page_down);
        } else {
          set_state(scrollbar_state::nothing);
        }
        if (get_state() != scrollbar_state::nothing) {
          capture_pointer();
        }
      }
    }

    template<orientation H>
    void basic_scroll_bar<H>::handle_left_btn_up (os::key_state, const core::point& pt) {
      if (is_enabled()) {
        auto geo = get_geometry();
        switch (get_state()) {
        case scrollbar_state::up_button:
          if (up_button_place(geo).is_inside(pt)) {
            set_value(get_value() - get_step(), true);
          }
          break;
        case scrollbar_state::down_button:
          if (down_button_place(geo).is_inside(pt)) {
            set_value(get_value() + get_step(), true);
          }
          break;
        case scrollbar_state::page_up:
          if (page_up_place(geo).is_inside(pt)) {
            set_value(get_value() - get_page(), true);
          }
          break;
        case scrollbar_state::page_down:
          if (page_down_place(geo).is_inside(pt)) {
            set_value(get_value() + get_page(), true);
          }
          break;
        case scrollbar_state::thumb_button:
        case scrollbar_state::nothing:
          break;
        }
        if (get_state() != scrollbar_state::nothing) {
          set_state(scrollbar_state::nothing);
          uncapture_pointer();
          send_notify();
          redraw();
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation::vertical>::init ();

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation::vertical>::handle_paint (const draw::graphics& g);

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation::vertical>::handle_mouse_move (os::key_state, const core::point&);

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation::vertical>::handle_any_key_up (os::key_state, os::key_symbol key);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation::horizontal>::init ();

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation::horizontal>::handle_paint (const draw::graphics& g);

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation::horizontal>::handle_mouse_move (os::key_state, const core::point&);

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation::horizontal>::handle_any_key_up (os::key_state, os::key_symbol key);

    // --------------------------------------------------------------------------
    template<>
    inline scroll_bar::type basic_scroll_bar<orientation::horizontal>::length (const core::size& sz) {
      return sz.width();
    }

    template<>
    inline scroll_bar::type basic_scroll_bar<orientation::vertical>::length (const core::size& sz) {
      return sz.height();
    }

    template<>
    inline scroll_bar::type basic_scroll_bar<orientation::horizontal>::thickness (const core::size& sz) {
      return sz.height();
    }

    template<>
    inline scroll_bar::type basic_scroll_bar<orientation::vertical>::thickness (const core::size& sz) {
      return sz.width();
    }

    template<>
    inline core::size basic_scroll_bar<orientation::horizontal>::build_size (type pos,
                                                                             type thickness) {
      return core::size(pos, thickness);
    }

    template<>
    inline core::size basic_scroll_bar<orientation::vertical>::build_size (type pos,
                                                                           type thickness) {
      return core::size(thickness, pos);
    }

    template<>
    inline core::point basic_scroll_bar<orientation::horizontal>::build_pos (type pos) {
      return core::point(pos, 0);
    }

    template<>
    inline core::point basic_scroll_bar<orientation::vertical>::build_pos (type pos) {
      return core::point(0, pos);
    }

  } // ctrl

} // gui

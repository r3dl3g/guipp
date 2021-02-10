/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
    template<orientation_t H>
    inline basic_scroll_bar<H>::basic_scroll_bar (bool grab_focus) {
      set_accept_focus(grab_focus);
      init();
    }

    template<orientation_t H>
    inline basic_scroll_bar<H>::basic_scroll_bar (const basic_scroll_bar& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation_t H>
    inline basic_scroll_bar<H>::basic_scroll_bar (basic_scroll_bar&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation_t H>
    inline void basic_scroll_bar<H>::create (win::container& parent,
                                             const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
    }

    template<orientation_t H>
    inline auto basic_scroll_bar<H>::get_geometry () const -> geometry {
      const core::rectangle r = super::surface_area();
      const core::point pt = r.top_left();
      const core::size sz = r.size();
      type l = length(sz);
      type t = thickness(sz);
      type b = button_size(l, t);
      type s = space_size(l, b);
      type th = thumb_size(s, b);
      type sc = get_scale(s, th);
      type tt = thumb_top(b, sc);
      type pos = core::orientation_traits<H>::get_1(pt);
      type o = core::orientation_traits<H>::get_2(pt);
      return {l, t, b, s, th, sc, tt, pos, o};
    }

    template<orientation_t H>
    inline auto basic_scroll_bar<H>::get_scale() const -> type {
      return get_geometry().scale;
    }

    template<orientation_t H>
    inline auto basic_scroll_bar<H>::get_scale (type spc_size, type tmb_size) const->type {
      return (spc_size - tmb_size) / (get_max() - get_min());
    }

    namespace detail {

      template<typename T, look::look_and_feel_t L>
      struct geometry {
        static T button_size (T length, T thickness) {
          return std::min(thickness, length / T(2));
        }

        static T thumb_min (T spc_size, T btn_size) {
          return std::min(btn_size, spc_size);
        }
      };

      template<typename T>
      struct geometry<T, look::look_and_feel_t::osx> {
        constexpr static T button_size (T, T) {
          return 0;
        }

        constexpr static T thumb_min (T, T) {
          return 32;
        }
      };

    } // namespace detail

    template<orientation_t H>
    inline auto basic_scroll_bar<H>::button_size (type length, type thickness)->type {
      return detail::geometry<type, look::system_look_and_feel>::button_size(length, thickness);
    }

    template<orientation_t H>
    inline auto basic_scroll_bar<H>::space_size (type length, type btn_size)->type {
      return std::max(length - btn_size * 2, type(0));
    }

    template<orientation_t H>
    inline auto basic_scroll_bar<H>::thumb_size (type spc_size, type btn_size) const->type {
      return std::max(get_page() * spc_size / (get_range() + get_page()),
                      detail::geometry<type, look::system_look_and_feel>::thumb_min(btn_size, spc_size));
    }

    template<orientation_t H>
    inline auto basic_scroll_bar<H>::thumb_top (type btn_size, type scale) const->type {
      return btn_size + (get_value() - get_min()) * scale;
    }

    template<orientation_t H>
    inline core::rectangle basic_scroll_bar<H>::up_button_place (const geometry& m) const {
      return core::rectangle(build_pos(m.pos, m.other), build_size(m.button_size, m.thickness));
    }

    template<orientation_t H>
    inline core::rectangle basic_scroll_bar<H>::down_button_place (const geometry& m) const {
      return core::rectangle(build_pos(m.pos + m.length - m.button_size, m.other),
                             build_size(m.button_size, m.thickness));
    }

    template<orientation_t H>
    inline core::rectangle basic_scroll_bar<H>::page_up_place (const geometry& m) const {
      return core::rectangle(build_pos(m.pos + m.button_size, m.other),
                             build_size(m.thumb_top - m.button_size, m.thickness));
    }

    template<orientation_t H>
    inline core::rectangle basic_scroll_bar<H>::page_down_place (const geometry& m) const {
      type tmb_bottom = m.thumb_top + m.thumb_size;
      return core::rectangle(build_pos(m.pos + tmb_bottom, m.other),
                             build_size(m.length - m.button_size - tmb_bottom, m.thickness));
    }

    template<orientation_t H>
    inline core::rectangle basic_scroll_bar<H>::thumb_button_place (const geometry& m) const {
      return core::rectangle(build_pos(m.pos + m.thumb_top, m.other),
                             build_size(m.thumb_size, m.thickness));
    }

    template<orientation_t H>
    void basic_scroll_bar<H>::handle_left_btn_down (os::key_state, const core::point& pt) {
      if (is_enabled()) {
        if (can_accept_focus()) {
          take_focus();
        }
        set_last_mouse_point(pt);
        set_last_value(get_value());

        auto geo = get_geometry();

        if (up_button_place(geo).is_inside(pt)) {
          set_selection(scrollbar_item::up_button);
        } else if (down_button_place(geo).is_inside(pt)) {
          set_selection(scrollbar_item::down_button);
        } else if (thumb_button_place(geo).is_inside(pt)) {
          set_selection(scrollbar_item::thumb_button);
        } else if (page_up_place(geo).is_inside(pt)) {
          set_selection(scrollbar_item::page_up);
        } else if (page_down_place(geo).is_inside(pt)) {
          set_selection(scrollbar_item::page_down);
        } else {
          set_selection(scrollbar_item::nothing);
        }
        if (get_selection() != scrollbar_item::nothing) {
          capture_pointer();
        }
      }
    }

    template<orientation_t H>
    void basic_scroll_bar<H>::handle_left_btn_up (os::key_state, const core::point& pt) {
      if (is_enabled()) {
        auto geo = get_geometry();
        switch (get_selection()) {
        case scrollbar_item::up_button:
          if (up_button_place(geo).is_inside(pt)) {
            set_value(get_value() - get_step(), true);
          }
          break;
        case scrollbar_item::down_button:
          if (down_button_place(geo).is_inside(pt)) {
            set_value(get_value() + get_step(), true);
          }
          break;
        case scrollbar_item::page_up:
          if (page_up_place(geo).is_inside(pt)) {
            set_value(get_value() - get_page(), true);
          }
          break;
        case scrollbar_item::page_down:
          if (page_down_place(geo).is_inside(pt)) {
            set_value(get_value() + get_page(), true);
          }
          break;
        case scrollbar_item::thumb_button:
        case scrollbar_item::nothing:
          break;
        }
        if (get_selection() != scrollbar_item::nothing) {
          set_selection(scrollbar_item::nothing);
          uncapture_pointer();
          send_notify();
          invalidate();
        }
      }
    }

    template<orientation_t H>
    void basic_scroll_bar<H>::handle_paint (draw::graphics& g) {
      auto geo = get_geometry();
      look::scrollbar<>(g,
                        super::get_selection(),
                        super::get_hilite(),
                        super::is_enabled(),
                        H == orientation_t::horizontal,
                        super::is_focused(),
                        up_button_place(geo),
                        down_button_place(geo),
                        thumb_button_place(geo),
                        page_up_place(geo),
                        page_down_place(geo));
    }

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation_t::vertical>::init ();

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation_t::vertical>::handle_mouse_move (os::key_state, const core::point&);

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation_t::vertical>::handle_any_key_up (os::key_state, os::key_symbol key);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation_t::horizontal>::init ();

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation_t::horizontal>::handle_mouse_move (os::key_state, const core::point&);

    template<>
    GUIPP_CTRL_EXPORT void basic_scroll_bar<orientation_t::horizontal>::handle_any_key_up (os::key_state, os::key_symbol key);

    // --------------------------------------------------------------------------
    template<>
    inline scroll_bar::type basic_scroll_bar<orientation_t::horizontal>::length (const core::size& sz) {
      return sz.width();
    }

    template<>
    inline scroll_bar::type basic_scroll_bar<orientation_t::vertical>::length (const core::size& sz) {
      return sz.height();
    }

    template<>
    inline scroll_bar::type basic_scroll_bar<orientation_t::horizontal>::thickness (const core::size& sz) {
      return sz.height();
    }

    template<>
    inline scroll_bar::type basic_scroll_bar<orientation_t::vertical>::thickness (const core::size& sz) {
      return sz.width();
    }

    template<>
    inline core::size basic_scroll_bar<orientation_t::horizontal>::build_size (type pos,
                                                                               type thickness) {
      return core::size(pos, thickness);
    }

    template<>
    inline core::size basic_scroll_bar<orientation_t::vertical>::build_size (type pos,
                                                                             type thickness) {
      return core::size(thickness, pos);
    }

    template<>
    inline core::point basic_scroll_bar<orientation_t::horizontal>::build_pos (type pos, type other) {
      return core::point(pos, other);
    }

    template<>
    inline core::point basic_scroll_bar<orientation_t::vertical>::build_pos (type pos, type other) {
      return core::point(other, pos);
    }

    // --------------------------------------------------------------------------
    // --------------------------------------------------------------------------
  } // ctrl

} // gui

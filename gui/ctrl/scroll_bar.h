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
 * @brief     C++ API: basic controls
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>


namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    template<>
    float get_param<0, float>(const core::event& e);

    using scroll_event = event_handler<detail::SCROLLBAR_MESSAGE, 0,
                                       params<core::point_type>::getter<get_param<0, core::point_type> > >;
    // --------------------------------------------------------------------------
#endif //WIN32
#ifdef X11
    // --------------------------------------------------------------------------
    using scroll_event = event_handler<ClientMessage, 0,
                                       params<core::point::type>::getter<get_client_data<0, core::point::type> >,
                                       0, client_message_matcher<detail::SCROLLBAR_MESSAGE> >;
    // --------------------------------------------------------------------------
#endif // X11

    // --------------------------------------------------------------------------
    enum class scrollbar_state {
      nothing,
      up_button,
      page_up,
      thumb_button,
      page_down,
      down_button
    };

    // --------------------------------------------------------------------------
    struct scroll_bar_data {
      typedef core::point::type type;

      scroll_bar_data ();

      scrollbar_state state;
      scrollbar_state hilite;

      type min;
      type max;
      type step;
      type value;

      type last_value;
      core::point last_mouse_point;
    };

    // --------------------------------------------------------------------------
    class scroll_bar : public window {
    public:
      typedef window super;
      typedef scroll_bar_data::type type;

      static constexpr int scroll_bar_width = 17;

      type get_min () const;
      type get_max () const;
      type get_step () const;
      type get_value () const;
      type get_range () const;

      void set_min(type);
      void set_max(type);
      void set_min_max(type, type);
      void set_step(type);
      void set_value(type, bool notify = true);

      void set_min_max_step(type, type, type);
      void set_min_max_step_value(type, type, type, type);

      void handle_wheel (const core::point_type delta, const core::point&);

      static constexpr int get_scroll_bar_width () {
        return scroll_bar_width;
      }

      scrollbar_state get_state () const;
      scrollbar_state get_hilite () const;

      void send_notify ();

    protected:
      scroll_bar ();
      scroll_bar (const scroll_bar&);
      scroll_bar (scroll_bar&&);

      void create (const class_info& type,
                   const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void set_state (scrollbar_state);
      void set_hilite (scrollbar_state);

      type get_last_value () const;
      void set_last_value (type last_value);

      core::point get_last_mouse_point () const;
      void set_last_mouse_point (core::point last_mouse_point);

    private:
      void init ();

      scroll_bar_data data;

    };

    // --------------------------------------------------------------------------
    inline auto scroll_bar::get_range() const->type {
      return get_max() + get_step() - get_min();
    }

    // --------------------------------------------------------------------------
    namespace paint {
      void scrollbar (const draw::graphics &g,
                      scrollbar_state state,
                      bool is_enabled,
                      bool horizontal,
                      bool has_focus,
                      const core::rectangle& up,
                      const core::rectangle& down,
                      const core::rectangle& thumb,
                      const core::rectangle& page_up,
                      const core::rectangle& page_down);
    }

    // --------------------------------------------------------------------------
    template<orientation H>
    class basic_scroll_bar : public scroll_bar {
    public:
      typedef scroll_bar super;
      typedef no_erase_window_class<basic_scroll_bar> clazz;

      basic_scroll_bar (bool grab_focus = true);
      basic_scroll_bar (const basic_scroll_bar& rhs);
      basic_scroll_bar (basic_scroll_bar&& rhs);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void handle_paint (const draw::graphics&);
      void handle_left_btn_down (os::key_state, const core::point&);
      void handle_left_btn_up (os::key_state, const core::point&);
      void handle_mouse_move (os::key_state, const core::point&);
      void handle_any_key_up (os::key_state, os::key_symbol key);

    private:
      void init ();

      struct geometry {
        type length;
        type thickness;
        type button_size;
        type space_size;
        type thumb_size;
        type scale;
        type thumb_top;
      };

      geometry get_geometry () const;
      type get_scale () const;
      type get_scale (type spc_size, type tmb_size) const;

      static core::size build_size (type pos, type thickness);
      static core::point build_pos (type pos);

      static type length (const core::size& sz);
      static type thickness (const core::size& sz);

      static type button_size (type length, type thickness);
      static type space_size (type length, type btn_size);

      type thumb_size (type spc_size, type btn_size) const;
      type thumb_top (type btn_size, type scale) const;

      core::rectangle up_button_place (const geometry& m) const;
      core::rectangle down_button_place (const geometry& m) const;
      core::rectangle page_up_place (const geometry& m) const;
      core::rectangle page_down_place (const geometry& m) const;
      core::rectangle thumb_button_place (const geometry& m) const;

    };

    // --------------------------------------------------------------------------
    // inlines
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
    inline void basic_scroll_bar<H>::create (const container& parent,
                                             const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::get_geometry() const->geometry {
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
    inline auto basic_scroll_bar<H>::get_scale() const->type {
      return get_geometry().scale;
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::get_scale(type spc_size, type tmb_size) const->type {
      return (spc_size - tmb_size) / (get_max() - get_min());
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::button_size(type length, type thickness)->type {
      return std::min(thickness, length / type(2));
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::space_size(type length, type btn_size)->type {
      return std::max(length - btn_size * 2, type(0));
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::thumb_size(type spc_size, type btn_size) const->type {
      return std::max(get_step() * spc_size / get_range(), std::min(btn_size, spc_size));
    }

    template<orientation H>
    inline auto basic_scroll_bar<H>::thumb_top(type btn_size, type scale) const->type {
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
        if (accept_focus()) {
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
        redraw_later();
      }
    }

    template<orientation H>
    void basic_scroll_bar<H>::handle_left_btn_up (os::key_state, const core::point& pt) {
      if (is_enabled()) {
        auto geo = get_geometry();
        switch (get_state()) {
        case scrollbar_state::up_button:
          if (up_button_place(geo).is_inside(pt)) {
            set_value(get_value() - 1, true);
          }
          break;
        case scrollbar_state::down_button:
          if (down_button_place(geo).is_inside(pt)) {
            set_value(get_value() + 1, true);
          }
          break;
        case scrollbar_state::page_up:
          if (page_up_place(geo).is_inside(pt)) {
            set_value(get_value() - get_step(), true);
          }
          break;
        case scrollbar_state::page_down:
          if (page_down_place(geo).is_inside(pt)) {
            set_value(get_value() + get_step(), true);
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
        }
        redraw_later();
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation::vertical>::init ();

    template<>
    void basic_scroll_bar<orientation::vertical>::handle_paint (const draw::graphics& g);

    template<>
    void basic_scroll_bar<orientation::vertical>::handle_mouse_move (os::key_state, const core::point&);

    template<>
    void basic_scroll_bar<orientation::vertical>::handle_any_key_up (os::key_state, os::key_symbol key);

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation::horizontal>::init ();

    template<>
    void basic_scroll_bar<orientation::horizontal>::handle_paint (const draw::graphics& g);

    template<>
    void basic_scroll_bar<orientation::horizontal>::handle_mouse_move (os::key_state, const core::point&);

    template<>
    void basic_scroll_bar<orientation::horizontal>::handle_any_key_up (os::key_state, os::key_symbol key);

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

    // --------------------------------------------------------------------------
    using vertical_scroll_bar = basic_scroll_bar<orientation::vertical>;
    using horizontal_scroll_bar = basic_scroll_bar<orientation::horizontal>;

  } // win

} // gui

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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>


namespace gui {

#ifdef WIN32
  namespace win {

    template<>
    GUIPP_CTRL_EXPORT float get_param<0, float>(const core::event& e);

  } // namespace win
#endif //WIN32

  namespace ctrl {

#ifdef WIN32
    // --------------------------------------------------------------------------

    using scroll_event = core::event_handler<detail::SCROLLBAR_MESSAGE, 0,
                                       core::params<core::point::type>::getter<win::get_param<0, core::point::type> > >;
    // --------------------------------------------------------------------------
#endif //WIN32
#ifdef X11
    // --------------------------------------------------------------------------
    using scroll_event = core::event_handler<ClientMessage, 0,
                                       core::params<core::point::type>::getter<win::get_client_data<0, core::point::type> >,
                                       0, win::client_message_matcher<detail::SCROLLBAR_MESSAGE> >;
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
    struct GUIPP_CTRL_EXPORT scroll_bar_data {
      typedef core::point::type type;

      scroll_bar_data ();

      scrollbar_state state;
      scrollbar_state hilite;

      type min;
      type max;
      type step;
      type page;
      type value;

      type last_value;
      core::point last_mouse_point;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT scroll_bar : public control {
    public:
      typedef control super;
      typedef scroll_bar_data::type type;

      static constexpr int scroll_bar_width = 17;

      type get_min () const;
      type get_max () const;
      type get_step () const;
      type get_page () const;
      type get_value () const;
      type get_range () const;

      void set_min (type);
      void set_max (type);
      void set_min_max (type, type);
      void set_step (type);
      void set_page (type);
      void set_value(type, bool notify = true);

      void set_min_max_step (type, type, type);
      void set_min_max_step_page (type, type, type, type);
      void set_min_max_step_value (type, type, type, type);

      void handle_wheel (const core::point::type delta, const core::point&);

      static constexpr int get_scroll_bar_width () {
        return scroll_bar_width;
      }

      scrollbar_state get_state () const;
      scrollbar_state get_hilite () const;

      void send_notify ();

      void on_scroll (std::function<void(core::point::type)>&& f);

    protected:
      scroll_bar ();
      scroll_bar (const scroll_bar&);
      scroll_bar (scroll_bar&&);

      void create (const win::class_info& type,
                   const win::container& parent,
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
    namespace paint {
      GUIPP_CTRL_EXPORT void scrollbar (const draw::graphics &g,
                                        scrollbar_state state,
                                        scrollbar_state hilite,
                                        bool is_enabled,
                                        bool horizontal,
                                        bool has_focus,
                                        const core::rectangle& up,
                                        const core::rectangle& down,
                                        const core::rectangle& thumb,
                                        const core::rectangle& page_up,
                                        const core::rectangle& page_down);

      GUIPP_CTRL_EXPORT void scrollbar_w95 (const draw::graphics &g,
                                            scrollbar_state state,
                                            scrollbar_state hilite,
                                            bool is_enabled,
                                            bool horizontal,
                                            bool has_focus,
                                            const core::rectangle& up,
                                            const core::rectangle& down,
                                            const core::rectangle& thumb,
                                            const core::rectangle& page_up,
                                            const core::rectangle& page_down);
    }

    using scrollbar_drawer = void (*)(const draw::graphics &,
                                      scrollbar_state,
                                      scrollbar_state,
                                      bool,
                                      bool,
                                      bool,
                                      const core::rectangle&,
                                      const core::rectangle&,
                                      const core::rectangle&,
                                      const core::rectangle&,
                                      const core::rectangle&);
    // --------------------------------------------------------------------------
    template<orientation H, os::platform P = os::system_platform>
    struct scroll_bar_traits {};

    template<orientation H>
    struct scroll_bar_traits<H, os::platform::win32> {
      static constexpr os::style style = win::window_class_defaults<>::style;
    };

    template<>
    struct scroll_bar_traits<orientation::horizontal, os::platform::x11> {
      static constexpr os::style style = IF_X11_ELSE(SouthWestGravity, 0);
    };

    template<>
    struct scroll_bar_traits<orientation::vertical, os::platform::x11> {
      static constexpr os::style style = IF_X11_ELSE(NorthEastGravity, 0);
    };

    // --------------------------------------------------------------------------
    template<orientation H>
    class basic_scroll_bar : public scroll_bar {
    public:
      typedef scroll_bar super;
      typedef no_erase_window_class<basic_scroll_bar,
                                    win::window_class_defaults<>::cursor,
                                    scroll_bar_traits<H>::style> clazz;

      basic_scroll_bar (bool grab_focus = true);
      basic_scroll_bar (const basic_scroll_bar& rhs);
      basic_scroll_bar (basic_scroll_bar&& rhs);

      void create (const win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void handle_left_btn_down (os::key_state, const core::point&);
      void handle_left_btn_up (os::key_state, const core::point&);
      void handle_mouse_move (os::key_state, const core::point&);
      void handle_any_key_up (os::key_state, os::key_symbol key);

    protected:
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

      core::rectangle up_button_place (const geometry& m) const;
      core::rectangle down_button_place (const geometry& m) const;
      core::rectangle page_up_place (const geometry& m) const;
      core::rectangle page_down_place (const geometry& m) const;
      core::rectangle thumb_button_place (const geometry& m) const;

    private:
      void init ();

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

    };

    // --------------------------------------------------------------------------
    template<orientation H,
#ifdef BUILD_FOR_ARM
             scrollbar_drawer D = paint::scrollbar_w95>
#else
             scrollbar_drawer D = paint::scrollbar>
#endif
    class scroll_bar_base : public basic_scroll_bar<H> {
    public:
      typedef basic_scroll_bar<H> super;

      scroll_bar_base (bool grab_focus = true);
      scroll_bar_base (const scroll_bar_base& rhs);
      scroll_bar_base (scroll_bar_base&& rhs);

      void handle_paint (const draw::graphics&);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    using vertical_scroll_bar = scroll_bar_base<orientation::vertical>;
    using horizontal_scroll_bar = scroll_bar_base<orientation::horizontal>;

  } // ctrl

} // gui

#include <gui/ctrl/scroll_bar.inl>

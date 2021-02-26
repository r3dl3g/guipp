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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/orientation_traits.h>
#include <gui/ctrl/control.h>
#include <gui/ctrl/scrollbar_item.h>
#include <gui/ctrl/look/scroll_bar.h>


namespace gui {

#ifdef GUIPP_WIN
  namespace win {

    template<>
    GUIPP_CTRL_EXPORT float get_param<0, float>(const core::event& e);

  } // namespace win
#endif //GUIPP_WIN

  namespace ctrl {

    // --------------------------------------------------------------------------
#ifdef GUIPP_WIN
    using scroll_event = core::event_handler<detail::SCROLLBAR_MESSAGE, 0,
                                             core::params<core::point::type>::
                                             getter<win::get_param<0, core::point::type>>>;

#endif //GUIPP_WIN

#ifdef GUIPP_X11
    using scroll_event = core::event_handler<ClientMessage, 0,
                                             core::params<core::point::type>::
                                             getter<win::get_client_data<0, core::point::type>>,
                                             0,
                                             win::event::functor<win::client_message_matcher<detail::SCROLLBAR_MESSAGE>>>;
#endif // GUIPP_X11
#ifdef GUIPP_QT
    GUIPP_CTRL_EXPORT core::point::type get_scroll_value (const core::event&);

    using scroll_event = core::event_handler<detail::SCROLLBAR_MESSAGE, 0,
                                             core::params<core::point::type>::
                                             getter<get_scroll_value>>;
#endif // GUIPP_QT

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT scroll_bar_data {
      typedef core::point::type type;

      scroll_bar_data ();

      scrollbar_item selection;
      scrollbar_item hilite;

      type min;
      type max;
      type step;
      type page;
      type value;

      type last_value;
      core::native_point last_mouse_point;
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
      void set_min_max_page (type, type, type);
      void set_min_max_step_page (type, type, type, type);
      void set_min_max_step_value (type, type, type, type);
      void set_min_max_page_value (type, type, type, type);

      void handle_wheel (core::point::type delta, const core::native_point&);

      static constexpr int get_scroll_bar_width () {
        return scroll_bar_width;
      }

      scrollbar_item get_selection () const;
      scrollbar_item get_hilite () const;

      void send_notify ();

      void on_scroll (std::function<void(core::point::type)>&& f);

    protected:
      scroll_bar ();
      scroll_bar (const scroll_bar&);
      scroll_bar (scroll_bar&&) noexcept ;

      void create (const win::class_info& type,
                   win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void set_selection (scrollbar_item);
      void set_hilite (scrollbar_item);

      type get_last_value () const;
      void set_last_value (type last_value);

      core::native_point get_last_mouse_point () const;
      void set_last_mouse_point (core::native_point last_mouse_point);

    private:
      void init ();

      scroll_bar_data data;

    };

    // --------------------------------------------------------------------------
    using scrollbar_drawer = void (*)(draw::graphics& ,
                                      scrollbar_item,
                                      scrollbar_item,
                                      bool,
                                      bool,
                                      bool,
                                      const core::rectangle&,
                                      const core::rectangle&,
                                      const core::rectangle&,
                                      const core::rectangle&,
                                      const core::rectangle&);
    // --------------------------------------------------------------------------
    template<orientation_t H, core::os::platform_t P = core::os::system_platform>
    struct scroll_bar_traits {};

    template<orientation_t H>
    struct scroll_bar_traits<H, core::os::platform_t::win32> {
      static constexpr os::style style = win::window_class_defaults<>::style;
    };

    template<orientation_t H>
    struct scroll_bar_traits<H, core::os::platform_t::qt> {
      static constexpr os::style style = win::window_class_defaults<>::style;
    };

    template<>
    struct scroll_bar_traits<orientation_t::horizontal, core::os::platform_t::x11> {
      static constexpr os::style style = IF_X11_ELSE(SouthWestGravity, static_cast<os::style>(0));
    };

    template<>
    struct scroll_bar_traits<orientation_t::vertical, core::os::platform_t::x11> {
      static constexpr os::style style = IF_X11_ELSE(NorthEastGravity, static_cast<os::style>(0));
    };

    // --------------------------------------------------------------------------
    template<orientation_t H>
    class basic_scroll_bar : public scroll_bar {
    public:
      typedef scroll_bar super;
      typedef no_erase_window_class<basic_scroll_bar,
                                    win::window_class_defaults<>::cursor,
                                    scroll_bar_traits<H>::style> clazz;

      explicit basic_scroll_bar (bool grab_focus = true);
      basic_scroll_bar (const basic_scroll_bar& rhs);
      basic_scroll_bar (basic_scroll_bar&& rhs) noexcept ;

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void handle_left_btn_down (os::key_state, const core::native_point&);
      void handle_left_btn_up (os::key_state, const core::native_point&);
      void handle_mouse_move (os::key_state, const core::native_point&);
      void handle_any_key_up (os::key_state, os::key_symbol key);
      void handle_paint (draw::graphics&);

    protected:
      struct metric {
        type length;
        type thickness;
        type button_size;
        type space_size;
        type thumb_size;
        type scale;
        type thumb_top;
        type pos;
        type other;
      };

      metric get_metric () const;

      core::rectangle up_button_geometry (const metric& m) const;
      core::rectangle down_button_geometry (const metric& m) const;
      core::rectangle page_up_geometry (const metric& m) const;
      core::rectangle page_down_geometry (const metric& m) const;
      core::rectangle thumb_button_geometry (const metric& m) const;

    private:
      void init ();

      type get_scale () const;
      type get_scale (type spc_size, type tmb_size) const;

      static core::size build_size (type pos, type thickness);
      static core::point build_pos (type pos, type other);

      static type length (const core::size& sz);
      static type thickness (const core::size& sz);

      static type button_size (type length, type thickness);
      static type space_size (type length, type btn_size);

      type thumb_size (type spc_size, type btn_size) const;
      type thumb_top (type btn_size, type scale) const;

    };

    // --------------------------------------------------------------------------
    using vertical_scroll_bar = basic_scroll_bar<orientation_t::vertical>;
    using horizontal_scroll_bar = basic_scroll_bar<orientation_t::horizontal>;

  } // ctrl

} // gui

#include <gui/ctrl/scroll_bar.inl>

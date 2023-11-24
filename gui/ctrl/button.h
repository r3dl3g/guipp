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
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window_event_handler.h>
#include <gui/win/background_repeater.h>
#include <gui/ctrl/look/control.h>
#include <gui/ctrl/look/button.h>
#include <gui/ctrl/control.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/layout_container.h>
#include <gui/draw/pen.h>
#include <gui/draw/icons.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
#ifdef GUIPP_WIN
    using button_clicked_event = core::event_handler<detail::BN_CLICKED_MESSAGE, 0,
                                                     core::params<>::getter<> >;
    using button_pushed_event = core::event_handler<detail::BN_PUSHED_MESSAGE, 0,
                                                     core::params<>::getter<> >;
    using button_released_event = core::event_handler<detail::BN_UNPUSHED_MESSAGE, 0,
                                                     core::params<>::getter<> >;
    using button_state_event = core::event_handler<detail::BN_STATE_MESSAGE, 0,
                                                   core::params<bool>::
                                                   getter<win::get_param<0, bool> > >;
#endif //GUIPP_WIN

#ifdef GUIPP_X11
    using button_clicked_event = core::event_handler<ClientMessage, 0,
                                                     core::params<>::getter<>, 0,
                                                     win::event::functor<win::client_message_matcher<detail::BN_CLICKED_MESSAGE>>>;
    using button_pushed_event = core::event_handler<ClientMessage, 0,
                                                    core::params<>::getter<>, 0,
                                                    win::event::functor<win::client_message_matcher<detail::BN_PUSHED_MESSAGE>>>;
    using button_released_event = core::event_handler<ClientMessage, 0,
                                                      core::params<>::getter<>, 0,
                                                      win::event::functor<win::client_message_matcher<detail::BN_UNPUSHED_MESSAGE>>>;
    using button_state_event = core::event_handler<ClientMessage, 0,
                                                   core::params<bool>::getter<win::get_client_data<0, bool> >, 0,
                                                   win::event::functor<win::client_message_matcher<detail::BN_STATE_MESSAGE>>>;
#endif // GUIPP_X11

#ifdef GUIPP_QT
    GUIPP_CTRL_EXPORT bool get_button_state (const core::event&);

    using button_clicked_event = core::event_handler<detail::BN_CLICKED_MESSAGE>;
    using button_pushed_event = core::event_handler<detail::BN_PUSHED_MESSAGE>;
    using button_released_event = core::event_handler<detail::BN_UNPUSHED_MESSAGE>;
    using button_state_event = core::event_handler<detail::BN_STATE_MESSAGE, 0,
                                                   core::params<bool>::
                                                   getter<get_button_state>>;
#endif // GUIPP_QT

    // --------------------------------------------------------------------------
    typedef void (button_drawer) (draw::graphics&,
                                  const core::rectangle&,
                                  const core::button_state::is&,
                                  os::color,  // foreground
                                  os::color); // background
    // --------------------------------------------------------------------------
    typedef void (*text_button_drawer) (draw::graphics&,
                                       const core::rectangle&,
                                       const std::string&,
                                       const core::button_state::is&,
                                       os::color,  // foreground
                                       os::color); // background

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT button_base : public colored_control {
    public:
      typedef colored_control super;
      typedef win::no_erase_window_class<button_base> clazz;

      button_base (os::color foreground = color::windowTextColor());
      button_base (const button_base&) = default;
      button_base (button_base&&) noexcept ;

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      core::button_state::is get_state () const;
      core::button_state::set set_state ();

      bool is_hilited () const;
      bool is_pushed () const;
      bool is_checked () const;

      void set_hilited (bool b);
      void set_pushed (bool b);
      void set_checked (bool b);

      void on_clicked (std::function<void()>&& f);
      void on_pushed (std::function<void()>&& f);
      void on_released (std::function<void()>&& f);
      void on_state_changed (std::function<void(bool)>&& f);

    };

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT push_button_traits {
      push_button_traits (button_base&);
    };

    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    struct GUIPP_CTRL_EXPORT toggle_button_traits {
      toggle_button_traits (button_base&);
    };

    // --------------------------------------------------------------------------
    template<class T, button_drawer BG>
    class basic_button : public button_base {
    public:
      typedef button_base super;
      typedef T traits_type;

      basic_button (os::color foreground = color::black);
      basic_button (const basic_button& rhs);
      basic_button (basic_button&& rhs) noexcept ;

    protected:
      traits_type traits;

      void init ();

    };
    // --------------------------------------------------------------------------
    template<class T, button_drawer BG, text_button_drawer D>
    class basic_text_button : public basic_button<T, BG> {
    public:
      typedef basic_button<T, BG> super;

      explicit basic_text_button (const core::text_source& t = core::const_text());
      explicit basic_text_button (const std::string& t);
      basic_text_button (const basic_text_button& rhs);
      basic_text_button (basic_text_button&& rhs) noexcept ;

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);
      void create (win::container& parent,
                   const std::string& txt,
                   const core::rectangle& place = core::rectangle::def);
      void create (win::container& parent,
                   const core::text_source& txt,
                   const core::rectangle& place = core::rectangle::def);

      void set_text (const std::string& t);
      void set_text (const core::text_source& t);

      std::string get_text () const;

    private:
      void init ();

      core::text_source text;
    };
    // --------------------------------------------------------------------------
    using push_button = basic_button<push_button_traits, look::button_frame>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using toggle_button = basic_button<toggle_button_traits<keep_state>,
                                                            look::button_frame>;
    // --------------------------------------------------------------------------
    using text_button = basic_text_button<push_button_traits,
                                          look::button_frame,
                                          look::button_text>;
    // --------------------------------------------------------------------------
    using flat_button = basic_text_button<push_button_traits,
                                          look::flat_button_frame,
                                          look::flat_button_text>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using radio_button = basic_text_button<toggle_button_traits<keep_state>,
                                           look::radio_button_frame,
                                           look::check_button_text>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    class check_box : public basic_text_button<toggle_button_traits<keep_state>,
                                               look::check_box_frame,
                                               look::check_button_text> {
    public:
      typedef basic_text_button<toggle_button_traits<keep_state>,
                                                     look::check_box_frame,
                                                     look::check_button_text> super;

      explicit check_box (const core::text_source& t = core::const_text())
        : super(t)
      {}

      explicit check_box (const std::string& t)
        : super(t)
      {}

      inline void set (bool b) {
        super::set_checked(b);
      }

      inline bool get () const {
        return super::is_checked();
      }

    };

    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    class switch_button : public basic_text_button<toggle_button_traits<keep_state>,
                                                    look::switch_button,
                                                    look::switch_button_text> {
    public:
      typedef basic_text_button<toggle_button_traits<keep_state>,
                                                     look::switch_button,
                                                     look::switch_button_text> super;

      switch_button (const core::text_source& t = core::const_text())
        : super(t) {
        super::set_foreground(color::highLightColor());
        super::set_background(color::buttonColor());
      }
    };
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using flat_toggle_button = basic_text_button<toggle_button_traits<keep_state>,
                                                                      look::flat_button_frame,
                                                                      look::flat_button_text>;
    // --------------------------------------------------------------------------
    template<alignment_t align = alignment_t::top, bool keep_state = false>
    using tab_button = basic_text_button<toggle_button_traits<keep_state>,
                                                              look::aligned_tab_button<align>,
                                                              look::tab_button_text>;
    // --------------------------------------------------------------------------
    template<class T, draw::icon_type I, button_drawer BG = look::flat_button_frame>
    struct icon_button_t : public basic_button<T, BG> {
      typedef basic_button<T, BG> super;

      icon_button_t (os::color foreground = color::light_gray);
      icon_button_t (const icon_button_t& rhs);
      icon_button_t (icon_button_t&& rhs) noexcept ;

    private:
      void init ();
    };
    // --------------------------------------------------------------------------
    template<draw::icon_type I, button_drawer BG = look::flat_button_frame>
    using icon_push_button_t = icon_button_t<push_button_traits, I, BG>;
    // --------------------------------------------------------------------------
    template<draw::icon_type I, bool keep_state = false, button_drawer BG = look::flat_button_frame>
    using icon_toggle_button_t = icon_button_t<toggle_button_traits<keep_state>, I, BG>;

    // --------------------------------------------------------------------------
    template<class T, button_drawer BG = look::flat_button_frame>
    struct icon_button : public basic_button<T, BG> {
      typedef basic_button<T, BG> super;

      icon_button (draw::icon_type icon = draw::icon_type::none, os::color foreground = color::light_gray);
      icon_button (const icon_button& rhs);
      icon_button (icon_button&& rhs) noexcept ;

      void set_icon (draw::icon_type icon);
      draw::icon_type get_icon () const;

    private:
      void init ();

      draw::icon_type icon;
    };
    // --------------------------------------------------------------------------
    template<button_drawer BG = look::flat_button_frame>
    using icon_push_button = icon_button<push_button_traits, BG>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false, button_drawer BG = look::flat_button_frame>
    using icon_toggle_button = icon_button<toggle_button_traits<keep_state>, BG>;

    // --------------------------------------------------------------------------
    template<class T, button_drawer BG>
    class custom_button : public basic_button<T, BG> {
    public:
      typedef basic_button<T, BG> super;

      custom_button ();
      custom_button (const custom_button& rhs);
      custom_button (custom_button&& rhs) noexcept ;

      void set_drawer (std::function<button_drawer> d);

    private:
      void init ();

      std::function<button_drawer> drawer;
    };
    // --------------------------------------------------------------------------
    template<button_drawer BG = look::flat_button_frame>
    using custom_push_button = custom_button<push_button_traits, BG>;

    // --------------------------------------------------------------------------
    template<bool keep_state = false, button_drawer BG = look::flat_button_frame>
    using custom_toggle_button = custom_button<toggle_button_traits<keep_state>, BG>;

    // --------------------------------------------------------------------------
    typedef void (animated_button_drawer) (draw::graphics&,
                                           const core::rectangle&,
                                           const core::button_state::is&,
                                           os::color,  // foreground
                                           os::color, // background
                                           float); // animation_step
    // --------------------------------------------------------------------------
    typedef void (animated_text_button_drawer) (draw::graphics&,
                                                const core::rectangle&,
                                                const std::string&,
                                                const core::button_state::is&,
                                                os::color,  // foreground
                                                os::color, // background
                                                float); // animation_step

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT basic_animated_button_traits {

      basic_animated_button_traits (button_base&);
      ~basic_animated_button_traits ();

      template<animated_text_button_drawer& D>
      void draw (draw::graphics& g,
                 const core::rectangle& r,
                 const std::string& t,
                 const core::button_state::is& s,
                 os::color foreground,
                 os::color background) {
        D(g, r, t, s, foreground, background, animation_step);
      }

      template<animated_button_drawer& D>
      void draw (draw::graphics& g,
                 const core::rectangle& r,
                 const core::button_state::is& s,
                 os::color foreground,
                 os::color background) {
        D(g, r, s, foreground, background, animation_step);
      }

      void start_animation ();

    protected:
      win::background_repeater repeater;
      float animation_step;

    };

    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    struct animated_button_traits : public basic_animated_button_traits {
      animated_button_traits (button_base&);
    };

//    // --------------------------------------------------------------------------
//    template<bool keep_state = false>
//    class animated_switch_button : public basic_text_button<animated_button_traits<keep_state>,
//                                                            animated_text_button_drawer&,
//                                                            look::animated_switch_button> {
//    public:
//      typedef basic_text_button<animated_button_traits<keep_state>,
//                                animated_text_button_drawer&, look::animated_switch_button> super;

//      animated_switch_button (const core::text_source& t = core::const_text())
//        : super(t) {
//        super::set_foreground(color::highLightColor());
//        super::set_background(color::buttonColor());
//      }
//    };

    // --------------------------------------------------------------------------
    namespace detail {

      template<orientation_t H>
      struct button_pair_traits {};

      template<>
      struct button_pair_traits<orientation_t::vertical> {
        typedef layout::vertical_adaption<> layout;
        static constexpr draw::icon_type plus_icon = draw::icon_type::up;
        static constexpr draw::icon_type minus_icon = draw::icon_type::down;
        static inline void add (layout& l, std::reference_wrapper<win::window> plus,
                                std::reference_wrapper<win::window> minus) {
          l.add({plus, minus});
        }
      };

      template<>
      struct button_pair_traits<orientation_t::horizontal> {
        typedef layout::horizontal_adaption<> layout;
        static constexpr draw::icon_type plus_icon = draw::icon_type::right;
        static constexpr draw::icon_type minus_icon = draw::icon_type::left;
        static inline void add (layout& l, std::reference_wrapper<win::window> plus,
                                std::reference_wrapper<win::window> minus) {
          l.add({minus, plus});
        }
      };

      struct plus_minus_traits {
        typedef layout::vertical_adaption<> layout;
        static constexpr draw::icon_type plus_icon = draw::icon_type::add;
        static constexpr draw::icon_type minus_icon = draw::icon_type::remove;
        static inline void add (layout& l, std::reference_wrapper<win::window> plus,
                                std::reference_wrapper<win::window> minus) {
          l.add({plus, minus});
        }
      };
    }

    // --------------------------------------------------------------------------
    template<orientation_t H, typename T = detail::button_pair_traits<H>>
    class button_pair : public win::group_window<typename T::layout> {
    public:
      typedef T traits;
      typedef win::group_window<typename traits::layout> super;
      typedef std::function<void(int)> sign_fn;

      button_pair ();

      void on_change (sign_fn f);

      icon_push_button_t<traits::plus_icon, look::button_frame> plus;
      icon_push_button_t<traits::minus_icon, look::button_frame> minus;
    };

    // --------------------------------------------------------------------------
  } // ctrl

} // gui

#include <gui/ctrl/button.inl>

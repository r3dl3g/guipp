/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>
#include <gui/win/window_event_handler.h>


namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    using button_clicked_event = core::event_handler<detail::BN_CLICKED_MESSAGE, 0,
                                                     core::params<>::getter<> >;
    using button_pushed_event = core::event_handler<detail::BN_PUSHED_MESSAGE, 0,
                                                     core::params<>::getter<> >;
    using button_released_event = core::event_handler<detail::BN_UNPUSHED_MESSAGE, 0,
                                                     core::params<>::getter<> >;
    using button_state_event = core::event_handler<detail::BN_STATE_MESSAGE, 0,
                                                   core::params<bool>::
                                                   getter<get_param<0, bool> > >;
// --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    using button_clicked_event = core::event_handler<ClientMessage, 0,
                                                     core::params<>::getter<>, 0,
                                                     client_message_matcher<detail::BN_CLICKED_MESSAGE> >;
    using button_pushed_event = core::event_handler<ClientMessage, 0,
                                                    core::params<>::getter<>, 0,
                                                    client_message_matcher<detail::BN_PUSHED_MESSAGE> >;
    using button_released_event = core::event_handler<ClientMessage, 0,
                                                      core::params<>::getter<>, 0,
                                                      client_message_matcher<detail::BN_UNPUSHED_MESSAGE> >;
    using button_state_event = core::event_handler<ClientMessage, 0,
                                                   core::params<bool>::getter<get_client_data<0, bool> >, 0,
                                                   client_message_matcher<detail::BN_STATE_MESSAGE> >;
    // --------------------------------------------------------------------------
#endif // X11
    class button_base;

     // --------------------------------------------------------------------------
    struct button_state : public window_state {
      typedef window_state super;

      button_state (const button_base& win);

      bool is_pushed () const;
      bool is_hilited () const;
      bool is_checked () const;

      bool set_hilited (bool b);
      bool set_pushed (bool b);
      bool set_checked (bool b);

    protected:
      struct flags : public window_state::flags {
        enum {
          button_hilited = last_window_state_enum,
          button_pushed,
          button_checked,
          last_button_state_enum
        };
      };

    };

    // --------------------------------------------------------------------------
    namespace paint {
      void button_frame (const draw::graphics& graph,
                         const core::rectangle& r,
                         const button_state& state);

      void button_frame_w95 (const draw::graphics& graph,
                             const core::rectangle& r,
                             const button_state& state);

      void simple_frame (const draw::graphics& graph,
                         const core::rectangle& r,
                         bool hilite = false);

      void flat_button (const draw::graphics& g,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state,
                        os::color foreground = color::white,
                        os::color background = color::dark_gray);

      template<os::color foreground,
               os::color background>
      void color_flat_button (const draw::graphics& g,
                              const core::rectangle& r,
                              const std::string& text,
                              const button_state& state) {
        flat_button(g, r, text, state, foreground, background);
      }

      void push_button (const draw::graphics& graph,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state);

      void radio_button (const draw::graphics& graph,
                         const core::rectangle& area,
                         const std::string& text,
                         const button_state& state);

      void check_box (const draw::graphics& graph,
                      const core::rectangle& area,
                      const std::string& text,
                      const button_state& state);

      void switch_button (const draw::graphics& graph,
                          const core::rectangle& rect,
                          const std::string& text,
                          const button_state& state);

      void tab_button (const draw::graphics& g,
                       const core::rectangle& r,
                       const std::string& text,
                       const button_state& state,
                       os::color foreground,
                       alignment a);

      template<os::color foreground,
               alignment align>
      void aligned_tab_button (const draw::graphics& g,
                               const core::rectangle& r,
                               const std::string& text,
                               const button_state& state) {
        tab_button(g, r, text, state, foreground, align);
      }

    }

    // --------------------------------------------------------------------------
    typedef void (button_drawer)(const draw::graphics&,
                                 const core::rectangle&,
                                 const button_state&);
    // --------------------------------------------------------------------------
    typedef void (text_button_drawer)(const draw::graphics&,
                                      const core::rectangle&,
                                      const std::string&,
                                      const button_state&);

    // --------------------------------------------------------------------------
    class button_base : public window {
    public:
      typedef window super;
      typedef no_erase_window_class<button_base> clazz;

      button_base ();
      button_base (const button_base&);
      button_base (button_base&&);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      const button_state get_state () const;
      button_state get_state ();

      bool is_hilited () const;
      bool is_pushed () const;
      bool is_checked () const;

      void set_hilited (bool b);
      void set_pushed (bool b);
      void set_checked (bool b);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    struct push_button_traits {
      static void init (button_base&);
    };

    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    struct toggle_button_traits {
      static void init (button_base&);
    };

    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<false>::init (button_base&);

    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<true>::init (button_base&);

    // --------------------------------------------------------------------------
    template<class T>
    class basic_button : public button_base {
    public:
      typedef button_base super;
      typedef T traits;

      basic_button ();
      basic_button (const basic_button& rhs);
      basic_button (basic_button&& rhs);

    };
    // --------------------------------------------------------------------------
    template<class T, text_button_drawer D>
    class basic_text_button : public basic_button<T> {
    public:
      typedef basic_button<T> super;
      typedef T traits;

      basic_text_button (const text_source& t = const_text());
      basic_text_button (const basic_text_button& rhs);
      basic_text_button (basic_text_button&& rhs);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);
      void create (const container& parent,
                   const std::string& txt,
                   const core::rectangle& place = core::rectangle::def);
      void create (const container& parent,
                   const text_source& txt,
                   const core::rectangle& place = core::rectangle::def);

      void set_text (const std::string& t);
      void set_text (const text_source& t);

      std::string get_text () const;

    private:
      void init ();

      text_source text;
    };
    // --------------------------------------------------------------------------
    using push_button = basic_button<push_button_traits>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using toggle_button = basic_button<toggle_button_traits<keep_state> >;
    // --------------------------------------------------------------------------
    using text_button = basic_text_button<push_button_traits,
                                          paint::push_button>;
    // --------------------------------------------------------------------------
    template<os::color foreground = color::light_gray, os::color background = color::dark_gray>
    using flat_button = basic_text_button<push_button_traits,
                                          paint::color_flat_button<foreground, background> >;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using radio_button = basic_text_button<toggle_button_traits<keep_state>,
                                           paint::radio_button>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using check_box = basic_text_button<toggle_button_traits<keep_state>,
                                        paint::check_box>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using switch_button = basic_text_button<toggle_button_traits<keep_state>,
                                            paint::switch_button>;
    // --------------------------------------------------------------------------
    template<os::color foreground = color::light_gray,
             os::color background = color::dark_gray,
             bool keep_state = false>
    using flat_toggle_button = basic_text_button<toggle_button_traits<keep_state>,
                                                 paint::color_flat_button<foreground, background> >;
    // --------------------------------------------------------------------------
    template<os::color foreground = color::light_gray,
             alignment align = alignment::top,
             bool keep_state = false>
    using tab_button = basic_text_button<toggle_button_traits<keep_state>,
                                         paint::aligned_tab_button<foreground, align> >;
    // --------------------------------------------------------------------------
    template<class T>
    class custom_button : public basic_button<T> {
    public:
      typedef basic_button<T> super;

      custom_button ();
      custom_button (const custom_button& rhs);
      custom_button (custom_button&& rhs);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);
      void set_drawer (std::function<button_drawer> d);

    private:
      void init ();

      std::function<button_drawer> drawer;
    };
    // --------------------------------------------------------------------------
    using custom_push_button = custom_button<push_button_traits>;

    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using custom_toggle_button = custom_button<toggle_button_traits<keep_state> >;

    // --------------------------------------------------------------------------
  } // win

} // gui

#include <gui/ctrl/button.inl>

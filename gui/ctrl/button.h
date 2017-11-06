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
    using button_clicked_event = event_handler<detail::BN_CLICKED_MESSAGE, 0,
                                               params<>::getter<> >;
    using button_pushed_event = event_handler<detail::BN_PUSHED_MESSAGE, 0,
                                              params<>::getter<> >;
    using button_released_event = event_handler<detail::BN_UNPUSHED_MESSAGE, 0,
                                                params<>::getter<> >;
    using button_state_event = event_handler<detail::BN_STATE_MESSAGE, 0,
                                             params<bool>::
                                             getter<get_param<0, bool> > >;
// --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    using button_clicked_event = event_handler<ClientMessage, 0,
                                               params<>::getter<>, 0,
                                               client_message_matcher<detail::BN_CLICKED_MESSAGE> >;
    using button_pushed_event = event_handler<ClientMessage, 0,
                                              params<>::getter<>, 0,
                                              client_message_matcher<detail::BN_PUSHED_MESSAGE> >;
    using button_released_event = event_handler<ClientMessage, 0,
                                                params<>::getter<>, 0,
                                                client_message_matcher<detail::BN_UNPUSHED_MESSAGE> >;
    using button_state_event = event_handler<ClientMessage, 0,
                                             params<bool>::getter<get_client_data<0, bool> >, 0,
                                             client_message_matcher<detail::BN_STATE_MESSAGE> >;
    // --------------------------------------------------------------------------
#endif // X11
       // --------------------------------------------------------------------------
    struct button_state {
      button_state (bool pushed = false,
                    bool hilited = false,
                    bool checked = false);

      bool pushed;
      bool hilited;
      bool checked;
    };

    // --------------------------------------------------------------------------
    namespace paint {
      void button_frame (const draw::graphics& graph,
                         const core::rectangle& r,
                         const button_state& state,
                         bool focused,
                         bool enabled);

      void simple_frame (const draw::graphics& graph,
                         const core::rectangle& r);

      void flat_button (const draw::graphics& g,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state,
                        bool focused,
                        bool enabled = true,
                        os::color foreground = color::white,
                        os::color background = color::dark_gray);

      template<os::color foreground,
               os::color background>
      void color_flat_button (const draw::graphics& g,
                              const core::rectangle& r,
                              const std::string& text,
                              const button_state& state,
                              bool focused,
                              bool enabled) {
        flat_button(g, r, text, state, focused, enabled, foreground, background);
      }

      void push_button (const draw::graphics& graph,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state,
                        bool focused,
                        bool enabled);

      void radio_button (const draw::graphics& graph,
                         const core::rectangle& area,
                         const std::string& text,
                         const button_state& state,
                         bool focused,
                         bool enabled);

      void check_box (const draw::graphics& graph,
                      const core::rectangle& area,
                      const std::string& text,
                      const button_state& state,
                      bool focused,
                      bool enabled);

      void tab_button (const draw::graphics& g,
                       const core::rectangle& r,
                       const std::string& text,
                       alignment a,
                       const button_state& state,
                       bool focused,
                       bool enabled,
                       os::color foreground);

      template<os::color foreground,
               alignment align>
      void aligned_tab_button (const draw::graphics& g,
                               const core::rectangle& r,
                               const std::string& text,
                               const button_state& state,
                               bool focused,
                               bool enabled) {
        tab_button(g, r, text, align, state, focused, enabled, foreground);
      }

    }

    // --------------------------------------------------------------------------
    typedef void (button_drawer)(const draw::graphics&,
                                 const core::rectangle&,
                                 const button_state&,
                                 bool,
                                 bool);
    // --------------------------------------------------------------------------
    typedef void (text_button_drawer)(const draw::graphics&,
                                      const core::rectangle&,
                                      const std::string&,
                                      const button_state&,
                                      bool,
                                      bool);

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

      const button_state& get_state () const;

      bool is_hilited () const;
      bool is_pushed () const;
      bool is_checked () const;

      void set_hilited (bool b);
      void set_pushed (bool b);
      void set_checked (bool b);

    private:
      void init ();

      button_state data;
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
    // inlines
    inline void button_base::create (const container& parent,
                                     const core::rectangle& place) {
      window::create(clazz::get(), parent, place);
    }

    inline const button_state& button_base::get_state () const {
      return data;
    }

    inline bool button_base::is_hilited () const {
      return data.hilited;
    }

    inline bool button_base::is_pushed () const {
      return data.pushed;
    }

    inline bool button_base::is_checked () const {
      return data.checked;
    }

    // --------------------------------------------------------------------------
    template<class T>
    inline basic_button<T>::basic_button () {
      traits::init(*this);
    }

    template<class T>
    inline basic_button<T>::basic_button (const basic_button& rhs)
      : super(rhs) {
      traits::init(*this);
    }

    template<class T>
    inline basic_button<T>::basic_button (basic_button&& rhs)
      : super(std::move(rhs)) {
      traits::init(*this);
    }

    // --------------------------------------------------------------------------
    template<class T, text_button_drawer D>
    inline basic_text_button<T, D>::basic_text_button (const text_source& t)
      : text(t)
    {
      init();
    }

    template<class T, text_button_drawer D>
    inline basic_text_button<T, D>::basic_text_button (const basic_text_button& rhs)
      : super(rhs) {
      init();
    }

    template<class T, text_button_drawer D>
    inline basic_text_button<T, D>::basic_text_button (basic_text_button&& rhs)
      : super(std::move(rhs)) {
      init();
    }

    template<class T, text_button_drawer D>
    inline void basic_text_button<T, D>::create (const container& parent,
                                                 const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T, text_button_drawer D>
    inline void basic_text_button<T, D>::create (const container& parent,
                                                 const std::string& txt,
                                                 const core::rectangle& place) {
      create(parent, const_text(txt), place);
    }

    template<class T, text_button_drawer D>
    inline void basic_text_button<T, D>::create (const container& parent,
                                                 const text_source& txt,
                                                 const core::rectangle& place) {
      super::create(parent, place);
      set_text(txt);
    }

    template<class T, text_button_drawer D>
    inline void basic_text_button<T, D>::set_text (const std::string& t) {
      set_text(const_text(t));
    }

    template<class T, text_button_drawer D>
    inline void basic_text_button<T, D>::set_text (const text_source& t) {
      text = t;
      super::redraw_later();
    }

    template<class T, text_button_drawer D>
    inline std::string basic_text_button<T, D>::get_text () const {
      return text();
    }

    template<class T, text_button_drawer D>
    void basic_text_button<T, D>::init () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics & graph) {
        D(graph, super::client_area(), get_text(), super::get_state(), super::has_focus(), super::is_enabled());
      }));
    }

    // --------------------------------------------------------------------------
    template<class T>
    inline custom_button<T>::custom_button () {
      init();
    }

    template<class T>
    inline custom_button<T>::custom_button (const custom_button& rhs)
      : super(rhs)
    {
      init();
    }

    template<class T>
    inline custom_button<T>::custom_button (custom_button&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<class T>
    inline void custom_button<T>::create (const container& parent,
                                          const core::rectangle& place) {
      super::create(parent, place);
    }

    template<class T>
    inline void custom_button<T>::set_drawer (std::function<button_drawer> d) {
      drawer = d;
    }

    template<class T>
    void custom_button<T>::init () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event([&] (const draw::graphics & graph) {
        if (drawer) {
          drawer(graph, super::client_area(), super::get_state(), super::has_focus(), super::is_enabled());
        }
      }));
    }

    // --------------------------------------------------------------------------

  } // win

} // gui

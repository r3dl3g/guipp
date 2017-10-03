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
#include "control.h"
#include "window_event_handler.h"

namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    using button_clicked_event = event_handler<detail::BN_CLICKED_MESSAGE, 0,
                                               params<>::getter<>>;
    using button_pushed_event = event_handler<detail::BN_PUSHED_MESSAGE, 0,
                                              params<>::getter<>>;
    using button_released_event = event_handler<detail::BN_UNPUSHED_MESSAGE, 0,
                                                params<>::getter<>>;
    using button_state_event = event_handler<detail::BN_STATE_MESSAGE, 0,
                                             params<bool>::
                                             getter<get_param<0, bool>>>;
// --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    using button_clicked_event = event_handler<ClientMessage, 0,
                                               params<>::getter<>, 0,
                                               client_message_matcher<detail::BN_CLICKED_MESSAGE>>;
    using button_pushed_event = event_handler<ClientMessage, 0,
                                              params<>::getter<>, 0,
                                              client_message_matcher<detail::BN_PUSHED_MESSAGE>>;
    using button_released_event = event_handler<ClientMessage, 0,
                                                params<>::getter<>, 0,
                                                client_message_matcher<detail::BN_UNPUSHED_MESSAGE>>;
    using button_state_event = event_handler<ClientMessage, 0,
                                             params<bool>::getter<get_client_data<0, bool>>, 0,
                                             client_message_matcher<detail::BN_STATE_MESSAGE>>;
    // --------------------------------------------------------------------------
#endif // X11
    // --------------------------------------------------------------------------
    struct button_state {
      button_state ();

      bool hilited;
      bool pushed;
      bool checked;
    };
    // --------------------------------------------------------------------------
    class button : public window {
    public:
      typedef window super;

      button ();
      button (const button&);
      button (button&&);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        window::create(clazz, parent, place);
      }

      inline const button_state& get_state () const {
        return data;
      }

      inline bool is_hilited () const {
        return data.hilited;
      }

      inline bool is_pushed () const {
        return data.pushed;
      }

      inline bool is_checked () const {
        return data.checked;
      }

      void set_hilited (bool b);
      void set_pushed (bool b);
      void set_checked (bool b);

    private:
      void init ();

      button_state data;

      static no_erase_window_class clazz;
    };

    // --------------------------------------------------------------------------
    namespace paint {
      void button_frame (const draw::graphics& graph,
                         const core::rectangle& r,
                         const button_state& state,
                         bool focused,
                         bool enabled);

      void flat_button (const draw::graphics& g,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state,
                        bool focused,
                        bool enabled = true,
                        os::color foreground = color::white,
                        os::color background = color::dark_gray);

      template<os::color foreground = color::light_gray, os::color background = color::dark_gray>
      void flat_button (const draw::graphics& g,
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
    }

    // --------------------------------------------------------------------------
    typedef void(button_drawer) (const draw::graphics&,
                                 const core::rectangle&,
                                 const button_state&,
                                 bool,
                                 bool);
    // --------------------------------------------------------------------------
    typedef void(text_button_drawer) (const draw::graphics&,
                                      const core::rectangle&,
                                      const std::string&,
                                      const button_state&,
                                      bool,
                                      bool);
    // --------------------------------------------------------------------------
    struct push_button_traits {
      static void init (button&);
    };
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    struct toggle_button_traits {
      static void init (button&);
    };
    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<false>::init (button&);
    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<true>::init (button&);
    // --------------------------------------------------------------------------
    template<class T>
    class button_t : public button {
    public:
      typedef button super;
      typedef T traits;

      inline button_t () {
        traits::init(*this);
      }

      inline button_t (const button_t& rhs)
        : super(rhs) {
        traits::init(*this);
      }

      inline button_t (button_t&& rhs)
        : super(std::move(rhs)) {
        traits::init(*this);
      }
    };
    // --------------------------------------------------------------------------
    template<class T, text_button_drawer D>
    class text_button_t : public button_t<T> {
    public:
      typedef button_t<T> super;
      typedef T traits;

      inline text_button_t (const text_source& t = const_text())
        : text(t)
      {
        init();
      }

      inline text_button_t (const text_button_t& rhs)
        : super(rhs) {
        init();
      }

      inline text_button_t (text_button_t&& rhs)
        : super(std::move(rhs)) {
        init();
      }

      inline void create (const container& parent,
                          const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      inline void create (const container& parent,
                          const std::string& txt,
                          const core::rectangle& place = core::rectangle::def) {
        create(parent, const_text(txt), place);
      }

      inline void create (const container& parent,
                          const text_source& txt,
                          const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
        set_text(txt);
      }

      inline void set_text (const std::string& t) {
        set_text(const_text(t));
      }

      void set_text (const text_source& t) {
        text = t;
        super::redraw_later();
      }

      inline std::string get_text () const {
        return text();
      }

    private:
      void init () {
        super::register_event_handler(REGISTER_FUNCTION, paint_event([&] (const draw::graphics& graph) {
          D(graph, super::client_area(), get_text(), super::get_state(), super::has_focus(), super::is_enabled());
        }));
      }

      text_source text;
    };
    // --------------------------------------------------------------------------
    using push_button = button_t<push_button_traits>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using toggle_button = button_t<toggle_button_traits<keep_state>>;
    // --------------------------------------------------------------------------
    using text_button = text_button_t<push_button_traits,
                                      paint::push_button>;
    // --------------------------------------------------------------------------
    template<os::color foreground = color::light_gray, os::color background = color::dark_gray>
    using flat_button = text_button_t<push_button_traits,
                                      paint::flat_button<foreground, background>>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using radio_button = text_button_t<toggle_button_traits<keep_state>,
                                       paint::radio_button>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using check_box = text_button_t<toggle_button_traits<keep_state>,
                                    paint::check_box>;
    // --------------------------------------------------------------------------
    template<os::color foreground = color::light_gray,
             os::color background = color::dark_gray,
             bool keep_state = false>
    using flat_toggle_button = text_button_t<toggle_button_traits<keep_state>,
                                             paint::flat_button<foreground, background>>;
    // --------------------------------------------------------------------------
    template<class T>
    class custom_button : public button_t<T> {
    public:
      typedef button_t<T> super;

      custom_button () {
        init();
      }

      custom_button (const custom_button& rhs)
        : super(rhs)
      {
        init();
      }

      custom_button (custom_button&& rhs)
        : super(std::move(rhs))
      {
        init();
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      void set_drawer (std::function<button_drawer> d) {
        drawer = d;
      }

    private:
      void init () {
        super::register_event_handler(REGISTER_FUNCTION, paint_event([&] (const draw::graphics& graph) {
          if (drawer) {
            drawer(graph, super::client_area(), super::get_state(), super::has_focus(), super::is_enabled());
          }
        }));
      }

      std::function<button_drawer> drawer;
    };
    // --------------------------------------------------------------------------
    using custom_push_button = custom_button<push_button_traits>;
    // --------------------------------------------------------------------------
    template<bool keep_state = false>
    using custom_toggle_button = custom_button<toggle_button_traits<keep_state>>;
    // --------------------------------------------------------------------------

  } // win

} // gui

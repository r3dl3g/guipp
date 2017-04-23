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
    typedef event_handler<detail::BN_CLICKED_MESSAGE, 0,
                          Params<>::caller<>>         button_clicked_event;
    typedef event_handler<detail::BN_PUSHED_MESSAGE, 0,
                          Params<>::caller<>>         button_pushed_event;
    typedef event_handler<detail::BN_UNPUSHED_MESSAGE, 0,
                          Params<>::caller<>>         button_released_event;
    typedef event_handler<detail::BN_STATE_MESSAGE, 0,
                          Params<bool>::
                          caller<get_param<0, bool>>> button_state_event;
// --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::BN_CLICKED_MESSAGE>>
                          button_clicked_event;
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::BN_PUSHED_MESSAGE>>
                          button_pushed_event;
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::BN_UNPUSHED_MESSAGE>>
                          button_released_event;
    typedef event_handler<ClientMessage, 0,
                           Params<bool>::caller<get_client_data<0, bool>>, 0,
                           client_message_matcher<detail::BN_STATE_MESSAGE>>
                           button_state_event;
    // --------------------------------------------------------------------------
#endif // X11
    // --------------------------------------------------------------------------
    class button : public window {
    public:
      typedef window super;

      button ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        window::create(clazz, parent, place);
      }

      inline bool is_hilited () const {
        return hilited;
      }

      inline bool is_pushed () const {
        return pushed;
      }

      inline bool is_checked () const {
        return checked;
      }

      void set_hilited (bool b);
      void set_pushed (bool b);
      void set_checked (bool b);

    private:
      bool hilited;
      bool pushed;
      bool checked;

      static no_erase_window_class clazz;
    };

    // --------------------------------------------------------------------------
    namespace paint {
      void push_button (const draw::graphics& graph,
                        const core::rectangle& r,
                        const std::string& text,
                        bool enabled = true,
                        bool focused = false,
                        bool hilited = false,
                        bool pushed = false);

      void push_button (const draw::graphics& graph,
                        const win::button& btn,
                        const std::string& text);

      void flat_button (const draw::graphics& g,
                        const core::rectangle& r,
                        const std::string& text,
                        bool enabled = true,
                        bool pushed = false,
                        bool hilited = false,
                        os::color foreground = color::white,
                        os::color background = color::dark_gray);

      void flat_button (const draw::graphics& graph,
                        const win::button& btn,
                        const std::string& text,
                        os::color foreground = color::white,
                        os::color background = color::dark_gray);

      void radio_button (const draw::graphics& graph,
                         const core::rectangle& area,
                         const std::string& text,
                         bool enabled = true,
                         bool focused = false,
                         bool pushed = false,
                         bool checked = false);

      void radio_button (const draw::graphics& graph,
                         const win::button& btn,
                         const std::string& text);

      void check_box (const draw::graphics& graph,
                      const core::rectangle& area,
                      const std::string& text,
                      bool enabled = true,
                      bool focused = false,
                      bool pushed = false,
                      bool checked = false);

      void check_box (const draw::graphics& graph,
                      const win::button& btn,
                      const std::string& text);
    }

    // --------------------------------------------------------------------------
    class push_button : public button {
    public:
      typedef button super;

      push_button ();

    };

    // --------------------------------------------------------------------------
    template<bool Keep = false>
    class toggle_button : public button {
    public:
      typedef button super;

      toggle_button ();
    };

    template<>
    toggle_button<false>::toggle_button ();

    template<>
    toggle_button<true>::toggle_button ();

    // --------------------------------------------------------------------------
    class text_push_button : public push_button {
    public:
      typedef push_button super;

      text_push_button (const std::string& t = std::string())
        : text(t)
      {}

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      void create (const container& parent,
                   const std::string& txt,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
        set_text(txt);
      }

      void set_text (const std::string& t) {
        text = t;
        redraw_later();
      }

      const std::string& get_text () const {
        return text;
      }

    private:
      std::string text;
    };

    // --------------------------------------------------------------------------
    class text_button : public text_push_button {
    public:
      typedef text_push_button super;

      text_button (const std::string& t = std::string()) {
        register_event_handler(paint_event([&] (const draw::graphics& graph) {
          paint::push_button(graph, *this, get_text());
        }));
      }
    };

    // --------------------------------------------------------------------------
    template<os::color C = color::light_gray, os::color B = color::dark_gray>
    class flat_button : public text_push_button {
    public:
      typedef text_push_button super;

      flat_button (const std::string& t = std::string())
        : super(t)
      {
        register_event_handler(paint_event([&] (const draw::graphics& graph) {
          paint::flat_button(graph, *this, get_text(), C, B);
        }));
      }

    };

    // --------------------------------------------------------------------------
    template<bool Keep = false>
    class text_toggle_button : public toggle_button<Keep> {
    public:
      typedef toggle_button<Keep> super;

      text_toggle_button (const std::string& t = std::string())
        : text(t)
      {}

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      void create (const container& parent,
                   const std::string& txt,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
        set_text(txt);
      }

      void set_text (const std::string& t) {
        text = t;
        super::redraw_later();
      }

      const std::string& get_text () const {
        return text;
      }

    private:
      std::string text;
    };

    // --------------------------------------------------------------------------
    template<bool Keep = false>
    class radio_button : public text_toggle_button<Keep> {
    public:
      typedef text_toggle_button<Keep> super;

      radio_button (const std::string& t = std::string()) {
        super::register_event_handler(win::paint_event([&] (const draw::graphics& graph) {
          paint::radio_button(graph, *this, super::get_text());
        }));
      }

    };

    // --------------------------------------------------------------------------
    template<bool Keep = false>
    class check_box : public text_toggle_button<Keep> {
    public:
      typedef text_toggle_button<Keep> super;

      check_box (const std::string& t = std::string()) {
        super::register_event_handler(win::paint_event([&] (const draw::graphics& graph) {
          paint::check_box(graph, *this, super::get_text());
        }));
      }

    };

    // --------------------------------------------------------------------------
    template<os::color C = color::light_gray, os::color B = color::dark_gray, bool Keep = false>
    class flat_toggle_button : public text_toggle_button<Keep> {
    public:
      typedef text_toggle_button<Keep> super;

      flat_toggle_button (const std::string& t = std::string())
        : super(t)
      {
        super::register_event_handler(paint_event([&] (const draw::graphics& graph) {
          paint::flat_button(graph, super::client_area(), super::get_text(), super::is_enabled(), super::is_checked(), super::is_hilited(), C, B);
        }));
      }
    };

    // --------------------------------------------------------------------------
    template<class super>
    class custom_button : public super {
    public:
      typedef void (button_drawer)(const draw::graphics&, const custom_button&);

      custom_button () {
        super::register_event_handler(win::paint_event([&] (const draw::graphics& graph) {
          if (drawer) {
            drawer(graph, *this);
          }
        }));
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      void set_drawer (std::function<button_drawer> d) {
        drawer = d;
      }

    private:
      std::function<button_drawer> drawer;
    };

    typedef custom_button<push_button> custom_push_button;
    typedef custom_button<toggle_button<>> custom_toggle_button;

  } // win

} // gui

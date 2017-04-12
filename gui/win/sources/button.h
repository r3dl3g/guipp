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
#include "owner_draw.h"
#include "window_event_handler.h"

namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    typedef event_handler<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_CLICKED>>
                           button_clicked_event;
    typedef event_handler<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_PUSHED>>
                           button_pushed_event;
    typedef event_handler<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_UNPUSHED>>
                           button_released_event;
    typedef event_handler<BM_SETCHECK,
                           one_param_caller<bool>>
                           button_state_event;
// --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
// --------------------------------------------------------------------------
    namespace detail {
      extern Atom BN_CLICKED_MESSAGE;
      extern Atom BN_PUSHED_MESSAGE;
      extern Atom BN_UNPUSHED_MESSAGE;
      extern Atom BN_STATE_MESSAGE;
    }

// --------------------------------------------------------------------------
    typedef event_handler<ClientMessage, Params<>::caller<>, 0,
                           client_message_matcher<detail::BN_CLICKED_MESSAGE>>
                           button_clicked_event;
    typedef event_handler<ClientMessage, Params<>::caller<>, 0,
                           client_message_matcher<detail::BN_PUSHED_MESSAGE>>
                           button_pushed_event;
    typedef event_handler<ClientMessage, Params<>::caller<>, 0,
                           client_message_matcher<detail::BN_UNPUSHED_MESSAGE>>
                           button_released_event;
    typedef event_handler<ClientMessage,
                           Params<bool>::caller<get_client_data<bool, 0>>, 0,
                           client_message_matcher<detail::BN_STATE_MESSAGE>>
                           button_state_event;
    // --------------------------------------------------------------------------
#endif // X11

    namespace detail {
      // --------------------------------------------------------------------------
      class button_info {
      public:
        button_info (window& w);

        bool is_checked (const window& w) const;
        void set_checked (window& w, bool b);

        bool is_hilited (const window& w) const;
        void set_hilited (window& w, bool b);

#ifdef X11
      private:
        bool checked;
        bool hilited;
#endif // X11
      };
    }

    // --------------------------------------------------------------------------
    template<class super>
    class button : public super {
    public:
      button ()
        : bi(*this)
      {}

      inline bool is_checked () const {
        return bi.is_checked(*this);
      }

      inline void set_checked (bool b) {
        bi.set_checked(*this, b);
      }

      inline bool is_hilited () const {
        return bi.is_hilited(*this);
      }

      inline void set_hilited (bool b) {
        bi.set_hilited(*this, b);
      }

    protected:
      detail::button_info bi;
    };

    // --------------------------------------------------------------------------
    namespace paint {
      void push_button (draw::graphics& graph, const win::window& btn, bool is_checked, const std::string& text);
      void radio_button (draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text);
      void check_box (draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text);
    }

    // --------------------------------------------------------------------------
    namespace detail {
      void register_push_button_handler (window& win, button_info& bi);
      void register_toggle_button_handler (window& win, button_info& bi);
    }

    // --------------------------------------------------------------------------
    template<class super>
    class push_button : public super {
    public:
      push_button () {
        register_push_button_handler(*this, super::bi);
      }
    };

    // --------------------------------------------------------------------------
    template<class super>
    class toggle_button : public super {
    public:
      toggle_button () {
        register_toggle_button_handler(*this, super::bi);
      }
    };

    // --------------------------------------------------------------------------
    class text_button : public push_button<button<window_with_text>> {
    public:
      typedef push_button<button<window_with_text>> super;

      text_button ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }

    private:
      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    class radio_button : public toggle_button<button<window_with_text>> {
    public:
      typedef toggle_button<button<window_with_text>> super;

      radio_button ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }

    private:
      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    class check_box : public toggle_button<button<window_with_text>> {
    public:
      typedef toggle_button<button<window_with_text>> super;

      check_box ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }

    private:
      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    class owner_draw_button : public button<owner_draw> {
    public:
      typedef button<owner_draw> super;

      typedef void(draw_button_item) (draw::graphics&, const owner_draw_button& btn);
      typedef std::function<draw_button_item> function;

      owner_draw_button ();

      void set_drawer (function drawer) {
        this->drawer = drawer;
      }

      void draw_item (draw::graphics& g) {
        if (drawer) {
          drawer(g, *this);
        }
      }

    private:
      function drawer;

      bool handle_event (const core::event& e,
                         os::event_result& result);
    };

    // --------------------------------------------------------------------------
    class custom_push_button : public push_button<owner_draw_button> {
    public:
      typedef push_button<owner_draw_button> super;

      custom_push_button ()
      {}

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(clazz, parent, place);
      }

    private:
      static window_class clazz;
    };

  } // win

} // gui

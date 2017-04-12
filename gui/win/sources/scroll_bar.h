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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "control.h"


namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    core::point::type get_scroll_pos(const core::event& e);

    struct scroll_matcher {
      bool operator() (const core::event& e);
    };

    typedef event_handler<WM_COMMAND,
                          Params<core::point::type>::
                          caller<get_scroll_pos>,
                          0, scroll_matcher>                  scroll_event;
    // --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    namespace detail {
      extern Atom SCROLLBAR_MESSAGE;
    }
    // --------------------------------------------------------------------------
    typedef event_handler<ClientMessage,
                           Params<core::point::type>::caller<get_client_data<core::point::type, 0>>, 0,
                           client_message_matcher<detail::SCROLLBAR_MESSAGE>>
            scroll_event;
    // --------------------------------------------------------------------------
#endif // X11

    class scroll_bar : public window {
    public:
      typedef window super;
      typedef core::point::type type;

      type get_min () const;
      type get_max () const;
      type get_step () const;
      type get_value () const;

      void set_min (type);
      void set_max (type);
      void set_min_max (type, type);
      void set_step (type);
      void set_value (type);

      void set_min_max_step (type, type, type);
      void set_min_max_step_value (type, type, type, type);

      static type get_scroll_bar_width ();

      enum State {
        Nothing_pressed = 0,
        Up_button_pressed,
        Page_up_pressed,
        Thumb_button_pressed,
        Page_down_pressed,
        Down_button_pressed
      };

      State get_state ();

    protected:
      scroll_bar ();

      void set_value (type v, bool notify);

      void create (const window_class& type,
                   const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      bool scroll_handle_event (const core::event& e, os::event_result& result);

#ifdef X11
      void set_state (State);

      void draw_scrollbar (draw::graphics &g,
                           const std::string& up_char,
                           const std::string& down_char,
                           const core::rectangle& up,
                           const core::rectangle& down,
                           const core::rectangle& thumb,
                           const core::rectangle& page_up,
                           const core::rectangle& page_down) const;

    private:
      State state;
      type min;
      type max;
      type step;
      type value;
#endif // X11

    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<bool H>
      class scroll_bar_class : public window_class {
      public:
        scroll_bar_class ();

        virtual void prepare (window*) const;
      };

      // --------------------------------------------------------------------------
      template<bool H>
      class scroll_barT : public scroll_bar {
      public:
        typedef scroll_bar super;

        scroll_barT ();

        ~scroll_barT ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(clazz, parent, place);
        }

      private:
        static scroll_bar_class<H> clazz;

#ifdef X11
        bool scroll_handle_eventT (const core::event& e,
                                   os::event_result& result);

        struct geometry {
          type length;
          type thickness;
          type button_size;
          type space_size;
          type thumb_size;
          type scale;
          type thumb_top;
        };

        geometry get_geometry () const {
          core::size sz = client_size();
          type l = length(sz);
          type t = thickness(sz);
          type b = button_size(l, t);
          type s = space_size(l, b);
          type th = thumb_size(s, b);
          type sc = get_scale(s, th);
          type tt = thumb_top(b, sc);
          return { l, t, b, s, th, sc, tt };
        }

        type get_range () const {
          return get_max() + get_step() - get_min();
        }

        type get_scale () const {
          return get_geometry().scale;
        }

        type get_scale (type spc_size, type tmb_size) const {
          return (spc_size - tmb_size) / (get_max() - get_min());
        }

        static core::size build_size (type pos,
                                      type thickness);

        static core::point build_pos (type pos);

        static type length (const core::size& sz);

        static type thickness (const core::size& sz);

        static type button_size (type length, type thickness) {
          return std::min(thickness, length / type(2));
        }

        static type space_size (type length, type btn_size) {
          return std::max(length - btn_size * 2 - 1, type(0));
        }

        type thumb_size (type spc_size, type btn_size) const {
          return std::max(get_step() * spc_size / get_range(), std::min(btn_size, spc_size));
        }

        type thumb_top (type btn_size, type scale) const {
          return btn_size + (get_value() - get_min()) * scale;
        }

        core::rectangle up_button_place (const geometry& m) const {
          return core::rectangle(core::point::zero, build_size(m.button_size, m.thickness));
        }

        core::rectangle down_button_place (const geometry& m) const {
          return core::rectangle(build_pos(m.length - m.button_size), build_size(m.button_size, m.thickness));
        }

        core::rectangle page_up_place (const geometry& m) const {
          return core::rectangle(build_pos(m.button_size), build_size(m.thumb_top - m.button_size, m.thickness));
        }

        core::rectangle page_down_place (const geometry& m) const {
          type tmb_bottom = m.thumb_top + m.thumb_size;
          return core::rectangle(build_pos(tmb_bottom), build_size(m.length - m.button_size - tmb_bottom, m.thickness));
        }

        core::rectangle thumb_button_place (const geometry& m) const {
          return core::rectangle(build_pos(m.thumb_top), build_size(m.thumb_size, m.thickness));
        }

        core::point last_mouse_point;
        type last_position;
        os::graphics gc;
#endif // X11
      };

      template<bool H>
      scroll_bar_class<H> scroll_barT<H>::clazz;

      template<>
      scroll_barT<false>::scroll_barT ();

      template<>
      scroll_barT<true>::scroll_barT ();

      template<>
      scroll_barT<false>::~scroll_barT ();

      template<>
      scroll_barT<true>::~scroll_barT ();

#ifdef X11
      template<>
      bool scroll_barT<false>::scroll_handle_eventT (const core::event& e,
                                                     os::event_result& result);

      template<>
      bool scroll_barT<true>::scroll_handle_eventT (const core::event& e,
                                                    os::event_result& result);

      template<>
      inline scroll_bar::type scroll_barT<true>::length (const core::size& sz) {
        return sz.width();
      }

      template<>
      inline scroll_bar::type scroll_barT<false>::length (const core::size& sz) {
        return sz.height();
      }

      template<>
      inline scroll_bar::type scroll_barT<true>::thickness  (const core::size& sz) {
        return sz.height();
      }

      template<>
      inline scroll_bar::type scroll_barT<false>::thickness  (const core::size& sz) {
        return sz.width();
      }

      template<>
      inline core::size scroll_barT<true>::build_size (type pos,
                                                       type thickness) {
        return core::size(pos, thickness);
      }

      template<>
      inline core::size scroll_barT<false>::build_size (type pos,
                                                        type thickness) {
        return core::size(thickness, pos);
      }

      template<>
      inline core::point scroll_barT<true>::build_pos (type pos) {
        return { pos, 0 };
      }

      template<>
      inline core::point scroll_barT<false>::build_pos (type pos) {
        return { 0, pos };
      }

#endif // X11
    }

    typedef detail::scroll_barT<false> vscroll_bar;
    typedef detail::scroll_barT<true> hscroll_bar;

  } // win

} // gui

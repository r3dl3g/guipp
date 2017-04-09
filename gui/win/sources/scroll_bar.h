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
#include "window.h"
#include "window_event_handler.h"


namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    core::point::type get_scroll_pos(const core::event& e);

    struct scroll_matcher {
      bool operator() (const core::event& e);
    };

    typedef event_handlerT<WM_COMMAND,
                           one_param_caller<core::point::type, get_scroll_pos>,
                           0, scroll_matcher>                  scroll_event;
    // --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    namespace detail {
      extern Atom SCROLLBAR_MESSAGE;
    }
    // --------------------------------------------------------------------------
    typedef event_handlerT<ClientMessage,
                           one_param_caller<core::point::type, get_client_data<core::point::type, 0>>, 0,
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

      static core::size::type get_scroll_bar_width ();

    protected:
      scroll_bar ();

      void set_value (type v, bool notify);

      void create (const window_class& type,
                   const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      bool scroll_handle_event (const core::event& e, os::event_result& result);

    private:
#ifdef X11
      type min;
      type max;
      type step;
      type value;
#endif // X11

    };

    namespace detail {
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
        static window_class clazz;

#ifdef X11
        bool scroll_handle_eventT (const core::event& e,
                                   os::event_result& result);

        struct geometry {
          core::size::type length;
          core::size::type thickness;
          core::size::type button_size;
          core::size::type space_size;
          core::size::type thumb_size;
        };

        geometry get_geometry () const {
          core::size sz = client_size();
          core::size::type l = length(sz);
          core::size::type t = thickness(sz);
          core::size::type b = button_size(l, t);
          core::size::type s = space_size(l, b);
          core::size::type th = thumb_size(s, b);
          return { l, t, b, s, th };
        }

        double get_scale () const {
          return get_scale(get_geometry());
        }

        double get_scale (const geometry& m) const {
          return (double)(m.space_size - m.thumb_size) / (double)(get_max() - get_min());
        }

        core::size::type thumb_size (core::size::type spc_size, core::size::type btn_size) const {
          return core::size::type(std::max(spc_size - (get_max() - get_min()), (type)std::min(btn_size, spc_size)));
        }

        core::size::type thumb_top (const geometry& m) const {
          return m.button_size + 1 + (get_value() - get_min()) * get_scale(m);
        }

        static core::size::type length (const core::size& sz);

        static core::size::type thickness (const core::size& sz);

        static core::size build_size (core::size::type pos,
                                      core::size::type thickness);

        static core::point build_pos (core::size::type pos);


        static core::size::type button_size (core::size::type length, core::size::type thickness) {
          return std::min(thickness, length / type(2));
        }

        static core::size::type space_size (core::size::type length, core::size::type btn_size) {
          return std::max(length - btn_size * 2 - 1, type(0));
        }

        core::rectangle up_button_place (const geometry& m) const {
          return core::rectangle(core::point::zero, build_size(m.button_size, m.thickness));
        }

        core::rectangle down_button_place (const geometry& m) const {
          return core::rectangle(build_pos(m.length - m.button_size), build_size(m.button_size, m.thickness));
        }

        core::rectangle page_up_place (const geometry& m) const {
          core::size::type bottom = thumb_top(m);
          return core::rectangle(build_pos(m.button_size), build_size(bottom - m.button_size, m.thickness));
        }

        core::rectangle page_down_place (const geometry& m) const {
          core::size::type tmb_top = thumb_top(m) + m.thumb_size;
          return core::rectangle(build_pos(tmb_top), build_size(m.length - m.button_size - tmb_top, m.thickness));
        }

        core::rectangle thumb_button_place (const geometry& m) const {
          core::size::type tmp_top = thumb_top(m);
          return core::rectangle(build_pos(tmp_top), build_size(m.thumb_size, m.thickness));
        }

        enum State {
          Nothing_pressed,
          Up_button_pressed,
          Down_button_pressed,
          Thumb_button_pressed,
          Page_up_pressed,
          Page_down_pressed
        };

        core::point last_mouse_point;
        State state;
        type last_position;
        os::graphics gc;
#endif // X11
      };

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
      inline core::size::type scroll_barT<true>::length (const core::size& sz) {
        return sz.width();
      }

      template<>
      inline core::size::type scroll_barT<false>::length (const core::size& sz) {
        return sz.height();
      }

      template<>
      inline core::size::type scroll_barT<true>::thickness  (const core::size& sz) {
        return sz.height();
      }

      template<>
      inline core::size::type scroll_barT<false>::thickness  (const core::size& sz) {
        return sz.width();
      }

      template<>
      inline core::size scroll_barT<true>::build_size (core::size::type pos,
                                                       core::size::type thickness) {
        return core::size(pos, thickness);
      }

      template<>
      inline core::size scroll_barT<false>::build_size (core::size::type pos,
                                                        core::size::type thickness) {
        return core::size(thickness, pos);
      }

      template<>
      inline core::point scroll_barT<true>::build_pos (core::size::type pos) {
        return { pos, 0 };
      }

      template<>
      inline core::point scroll_barT<false>::build_pos (core::size::type pos) {
        return { 0, pos };
      }

#endif // X11
    }

    typedef detail::scroll_barT<false> vscroll_bar;
    typedef detail::scroll_barT<true> hscroll_bar;

  } // win

} // gui

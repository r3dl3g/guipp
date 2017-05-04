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
    template<>
    float get_param<0, float>(const core::event& e);

    typedef event_handler<detail::SCROLLBAR_MESSAGE, 0,
                          Params<core::point_type>::caller<get_param<0, core::point_type>>>
            scroll_event;
    // --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    typedef event_handler<ClientMessage, 0,
                          Params<core::point::type>::caller<get_client_data<0, core::point::type>>, 0,
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

      type get_range () const {
        return get_max() + get_step() - get_min();
      }

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

      void set_state (State);

    private:
      State state;
      type min;
      type max;
      type step;
      type value;

    };

    namespace paint {
      void scrollbar (const draw::graphics &g,
                      scroll_bar::State state,
                      bool is_enabled,
                      bool horizontal,
                      const core::rectangle& up,
                      const core::rectangle& down,
                      const core::rectangle& thumb,
                      const core::rectangle& page_up,
                      const core::rectangle& page_down);
    }

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation H>
      class scroll_bar_class : public no_erase_window_class {
      public:
        scroll_bar_class ();
      };

      // --------------------------------------------------------------------------
      template<orientation H>
      class scroll_barT : public scroll_bar {
      public:
        typedef scroll_bar super;

        scroll_barT ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(clazz, parent, place);
        }

      private:
        static scroll_bar_class<H> clazz;

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
          return std::max(length - btn_size * 2, type(0));
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
      };

      template<orientation H>
      scroll_bar_class<H> scroll_barT<H>::clazz;

      template<>
      scroll_barT<orientation::vertical>::scroll_barT ();

      template<>
      scroll_barT<orientation::horizontal>::scroll_barT ();

      template<>
      inline scroll_bar::type scroll_barT<orientation::horizontal>::length (const core::size& sz) {
        return sz.width();
      }

      template<>
      inline scroll_bar::type scroll_barT<orientation::vertical>::length (const core::size& sz) {
        return sz.height();
      }

      template<>
      inline scroll_bar::type scroll_barT<orientation::horizontal>::thickness  (const core::size& sz) {
        return sz.height();
      }

      template<>
      inline scroll_bar::type scroll_barT<orientation::vertical>::thickness  (const core::size& sz) {
        return sz.width();
      }

      template<>
      inline core::size scroll_barT<orientation::horizontal>::build_size (type pos,
                                                       type thickness) {
        return core::size(pos, thickness);
      }

      template<>
      inline core::size scroll_barT<orientation::vertical>::build_size (type pos,
                                                        type thickness) {
        return core::size(thickness, pos);
      }

      template<>
      inline core::point scroll_barT<orientation::horizontal>::build_pos (type pos) {
        return core::point(pos, 0);
      }

      template<>
      inline core::point scroll_barT<orientation::vertical>::build_pos (type pos) {
        return core::point(0, pos);
      }

    }

    typedef detail::scroll_barT<orientation::vertical> vscroll_bar;
    typedef detail::scroll_barT<orientation::horizontal> hscroll_bar;

  } // win

} // gui

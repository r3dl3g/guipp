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
* @brief     C++ API: label
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

    // --------------------------------------------------------------------------
    namespace detail {
      class label_base : public gui::win::window {
      public:
        typedef window super;

        label_base (const std::string& = std::string());
        label_base (const text_source&);

        label_base (const label_base& rhs);
        label_base (label_base&& rhs);

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(clazz, parent, place);
        }

        void set_text (const text_source&);
        void set_text (const std::string&);

        std::string get_text () const;

        template<typename T>
        void operator<< (const T& t) {
          set_text(convert_to_string(t));
        }

        template<typename T>
        void operator>> (T& t) const {
          t = get_text();
        }

        template<typename T>
        void operator>> (T&& t) const {
          t = get_text();
        }

      protected:
        text_source text;

      private:
        static no_erase_window_class clazz;
      };

    }

    // --------------------------------------------------------------------------
    namespace paint {

      void label (const draw::graphics& graph,
                  const core::rectangle& area,
                  const std::string& text,
                  os::color foreground,
                  os::color background,
                  text_origin origin);

    }

    template<text_origin alignment,
             draw::frame::drawer F  = draw::frame::no_frame,
             os::color foreground = color::black,
             os::color background = color::very_light_gray>
    class label_t : public detail::label_base {
    public:
      typedef detail::label_base super;

      label_t () {
        init();
      }

      label_t (const label_t& rhs)
        : super(rhs)
      {
        init();
      }

      label_t (label_t&& rhs)
        : super(std::move(rhs))
      {
        init();
      }

      void paint (const draw::graphics& graph) {
        gui::core::rectangle place = client_area();
        paint::label(graph, client_area(), get_text(), foreground, background, alignment);
        F(graph, place);
      }

      inline void create (const container& parent,
                          const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      inline void create (const container& parent,
                          const text_source& txt,
                          const core::rectangle& place = core::rectangle::def) {
        create(parent, place);
        set_text(txt);
      }

      inline void create (const container& parent,
                          const std::string& txt,
                          const core::rectangle& place = core::rectangle::def) {
        create(parent, const_text(txt), place);
      }

    private:
      void init () {
        register_event_handler(REGISTER_FUNCTION, paint_event(this, &label_t::paint));
      }


    };

    // --------------------------------------------------------------------------
    using label_left = label_t<text_origin::vcenter_left,
                              draw::frame::no_frame,
                              color::black,
                              color::very_light_gray>;

    using label = label_left;

    using label_right = label_t<text_origin::vcenter_right,
                               draw::frame::no_frame,
                               color::black,
                               color::very_light_gray>;

    using label_center = label_t<text_origin::center,
                                draw::frame::no_frame,
                                color::black,
                                color::very_light_gray>;

  } // win

} // gui

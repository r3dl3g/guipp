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
* @brief     C++ API: editbox controls
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
#include "textbox.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      class editbox_base : public textbox_base {
      public:
        typedef textbox_base super;

        editbox_base ();
        ~editbox_base ();

        void handle_key (os::key_state, os::key_symbol, const std::string&);

      private:
        void init ();

        void handle_create (window*, const core::rectangle&);

        std::string& current_line ();

        position find_prev_word (const position& pos);
        position find_next_word (const position& pos);

      };

      // --------------------------------------------------------------------------
      class editbox : public editbox_base {
      public:
        typedef editbox_base super;

        void create (const container& parent,
                     const core::rectangle& r = core::rectangle::def) {
          window::create(clazz, parent, r);
        }

      private:
        static const window_class clazz;
      };
      // --------------------------------------------------------------------------

    } // namespace detail

    // --------------------------------------------------------------------------
    template<void(F)(const draw::graphics&, const core::rectangle&) = draw::frame::no_frame,
             os::color foreground = color::black,
             os::color background = color::white>
    class editbox_t : public detail::editbox {
    public:
      typedef detail::editbox super;

      editbox_t () {
        register_event_handler(REGISTER_FUNCTION, paint_event(this, &editbox_t::handle_paint));
      }

      void handle_paint (const draw::graphics& graph) {
        const auto area = client_area();
        paint::text_box(graph, area, data.lines, data.font,
                        foreground, background, text_origin::vcenter_left,
                        data.selection, data.cursor_pos, data.offset, has_focus());
        F(graph, area);
      }

    };

    typedef editbox_t<> editbox;

  } // win

} // gui

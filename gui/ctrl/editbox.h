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
// Library includes
//
#include <gui/ctrl/textbox.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      class editbox_base : public textbox_base {
      public:
        typedef textbox_base super;
        typedef no_erase_window_class<editbox_base, cursor_type::ibeam> clazz;

        editbox_base ();
        ~editbox_base ();

        void create (const container& parent,
                     const core::rectangle& r = core::rectangle::def);

        void handle_key (os::key_state, os::key_symbol, const std::string&);

      private:
        void init ();

        void handle_create (window*, const core::rectangle&);

        std::string& current_line ();

      };

      // --------------------------------------------------------------------------

    } // namespace detail

    // --------------------------------------------------------------------------
    template<draw::frame::drawer frame = draw::frame::no_frame,
             os::color foreground = color::black,
             os::color background = color::white>
    class basic_editbox : public detail::editbox_base {
    public:
      typedef detail::editbox_base super;

      basic_editbox ();

      void handle_paint (const draw::graphics& graph);

    };

    typedef basic_editbox<> editbox;

    // --------------------------------------------------------------------------
    // inlines
    template<draw::frame::drawer frame, os::color foreground, os::color background>
    basic_editbox<frame, foreground, background>::basic_editbox () {
      register_event_handler(REGISTER_FUNCTION, paint_event(draw::buffered_paint(this, &basic_editbox::handle_paint)));
    }

    template<draw::frame::drawer frame, os::color foreground, os::color background>
    void basic_editbox<frame, foreground, background>::handle_paint (const draw::graphics& graph) {
      const auto area = client_area();
      paint::text_box(graph, area, data.lines, data.font,
                      foreground, background, text_origin::vcenter_left,
                      data.selection, data.cursor_pos, data.offset, has_focus());
      frame(graph, area);
    }

    // --------------------------------------------------------------------------

  } // win

} // gui

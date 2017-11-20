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
 * @brief     C++ API: textbox controls
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

    namespace detail {

      inline void textbox_base::create (const container& parent,
                                        const core::rectangle& r) {
        window::create(clazz::get(), parent, r);
      }

      inline void textbox_base::create (const container& parent,
                                        const std::string& txt,
                                        const core::rectangle& place) {
        create(parent, place);
        set_text(txt);
      }

      inline auto textbox_base::row_count() const-> size_type {
        return data.lines.size();
      }

      inline const core::point& textbox_base::get_scroll_pos () const {
        return data.offset;
      }

      inline auto textbox_base::get_selection() const->const range& {
        return data.selection;
      }

      inline auto textbox_base::get_cursor_pos() const->const position& {
        return data.cursor_pos;
      }

      inline textbox_base::data::data ()
        : font(draw::font::system())
        , last_mouse_point(core::point::undefined)
      {}

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin align, draw::frame::drawer frame, os::color fg, os::color bg>
    inline basic_textbox<align, frame, fg, bg>::basic_textbox () {
      register_event_handler(REGISTER_FUNCTION, paint_event(draw::buffered_paint(this, &basic_textbox::handle_paint)));
    }

    template<text_origin align, draw::frame::drawer frame, os::color fg, os::color bg>
    inline void basic_textbox<align, frame, fg, bg>::handle_paint (const draw::graphics& graph) {
      const auto area = client_area();
      paint::text_box(graph, area, data.lines, data.font,
                      fg, bg, align,
                      data.selection, data.cursor_pos, data.offset, has_focus());
      frame(graph, area);
    }

  } // win

} // gui

/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     multiline textbox control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace ctrl {

    namespace detail {

      inline void textbox_base::create (win::container& parent,
                                        const core::rectangle& r) {
        create(clazz::get(), parent, r);
      }

      inline void textbox_base::create (win::container& parent,
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

      inline void textbox_base::set_font (const draw::font& f) {
        data.font = f;
      }

      inline const draw::font& textbox_base::get_font () const {
        return data.font;
      }

      inline textbox_base::data::data ()
        : font(draw::font::system())
        , last_mouse_point(core::native_point::undefined)
      {}

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin_t align, draw::frame::drawer frame>
    inline basic_textbox<align, frame>::basic_textbox () {
      on_paint(draw::paint(this, &basic_textbox::handle_paint));
      enable_select_by_mouse();
    }

    template<text_origin_t align, draw::frame::drawer frame>
    inline void basic_textbox<align, frame>::handle_paint (draw::graphics& graph) {
      const auto area = client_geometry();
      gui::look::text_box(graph, area, data.lines, data.font,
                          get_foreground(), get_background(), align,
                          data.selection, data.cursor_pos, data.offset, is_focused(), is_enabled());
      frame(graph, area);
    }

    template<text_origin_t align, draw::frame::drawer frame>
    inline void basic_textbox<align, frame>::enable_select_by_mouse () {
      super::enable_select_by_mouse(align);
    }

  } // ctrl

} // gui

/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: editbox control
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    template<draw::frame::drawer frame>
    basic_editbox<frame>::basic_editbox (os::color fg, os::color bg)
      : foreground(fg)
    {
      super::set_background(bg);
      on_paint(draw::paint(this, &basic_editbox::handle_paint));
      enable_select_by_mouse();
    }

    template<draw::frame::drawer frame>
    void basic_editbox<frame>::handle_paint (draw::graphics& graph) {
      const auto area = client_geometry();
      gui::look::text_box(graph, area, data.lines, data.font,
                          foreground, super::get_background(), text_origin_t::vcenter_left,
                          data.selection, data.cursor_pos, data.offset, is_focused(), is_enabled());
      frame(graph, area);
    }

    template<draw::frame::drawer frame>
    inline void basic_editbox<frame>::enable_select_by_mouse () {
      super::enable_select_by_mouse(text_origin_t::top_left);
    }

  } // ctrl

} // gui

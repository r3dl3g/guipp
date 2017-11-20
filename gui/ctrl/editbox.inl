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
 * @brief     C++ API: editbox control
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

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

  } // win

} // gui

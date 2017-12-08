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
 * @brief     C++ API: edit controls
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

    template<text_origin A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_edit<A, D, F, B>::basic_edit () {
      init();
    }

    template<text_origin A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_edit<A, D, F, B>::basic_edit (const basic_edit& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_edit<A, D, F, B>::basic_edit (basic_edit&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin alignment, draw::frame::drawer frame, os::color foreground, os::color background>
    inline void basic_edit<alignment, frame, foreground, background>::paint (const draw::graphics& graph) {
      auto area = frame(graph, client_area());
      area.shrink(core::size::one);
      paint::edit_line(graph, area, data.text, draw::font::system(), foreground, background, alignment, data.selection, data.cursor_pos, data.scroll_pos, has_focus());
    }

    template<text_origin A, draw::frame::drawer D, os::color F, os::color B>
    inline void basic_edit<A, D, F, B>::init () {
      super::register_handler();
      super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &basic_edit::paint));
    }

  } // win

} // gui

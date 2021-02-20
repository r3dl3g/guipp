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
 * @brief     C++ API: pass controls
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    template<text_origin_t A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_edit<A, D, F, B>::basic_edit () {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_edit<A, D, F, B>::basic_edit (const basic_edit& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_edit<A, D, F, B>::basic_edit (basic_edit&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin_t origin, draw::frame::drawer frame, os::color foreground, os::color background>
    inline void basic_edit<origin, frame, foreground, background>::paint (draw::graphics& graph) {
      auto area = frame(graph, client_geometry());
      area.shrink(core::size::one);
      look::edit_line(graph, area, data.text, draw::font::system(), foreground, background, origin,
                      data.selection, data.cursor_pos, data.scroll_pos, is_focused(), is_enabled());
    }

    template<text_origin_t A, draw::frame::drawer D, os::color F, os::color B>
    inline void basic_edit<A, D, F, B>::init () {
      super::register_handler();
      super::on_paint(draw::paint(this, &basic_edit::paint));
    }

    // --------------------------------------------------------------------------
    template<text_origin_t A, char C, draw::frame::drawer D, os::color F, os::color B>
    inline basic_pass<A, C, D, F, B>::basic_pass () {
      init();
    }

    template<text_origin_t A, char C, draw::frame::drawer D, os::color F, os::color B>
    inline basic_pass<A, C, D, F, B>::basic_pass (const basic_pass& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin_t A, char C, draw::frame::drawer D, os::color F, os::color B>
    inline basic_pass<A, C, D, F, B>::basic_pass (basic_pass&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin_t origin, char character, draw::frame::drawer frame, os::color foreground, os::color background>
    inline void basic_pass<origin, character, frame, foreground, background>::paint (draw::graphics& graph) {
      auto area = frame(graph, client_geometry());
      area.shrink(core::size::one);
      std::string t(data.text.length(), character);
      look::edit_line(graph, area, t, draw::font::system(), foreground, background, origin,
                      data.selection, data.cursor_pos, data.scroll_pos, is_focused(), is_enabled());
    }

    template<text_origin_t A, char C, draw::frame::drawer D, os::color F, os::color B>
    inline void basic_pass<A, C, D, F, B>::init () {
      super::register_handler();
      super::on_paint(draw::paint(this, &basic_pass::paint));
    }

    template<text_origin_t A, char C, draw::frame::drawer D, os::color F, os::color B>
    std::string basic_pass<A, C, D, F, B>::get_selected_text () const {
      auto r = super::get_selection();
      return std::string(r.size(), C);
    }

  } // ctrl

} // gui

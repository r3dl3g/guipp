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
 * @brief     edit control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace ctrl {

    namespace detail {

      template<typename T>
      inline void edit_base::set (const T& t) {
        set_text(util::string::convert::from<T>(t));
      }

      template<typename T>
      inline T edit_base::get () {
        return util::string::convert::to<T>(get_text());
      }

    } // detail

    template<text_origin_t A, draw::frame::drawer D>
    inline basic_edit<A, D>::basic_edit () {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline basic_edit<A, D>::basic_edit (const basic_edit& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline basic_edit<A, D>::basic_edit (basic_edit&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D>
    template<typename T>
    inline basic_edit<A, D>::basic_edit (const T& t) {
      init();
      set(t);
    }


    template<text_origin_t origin, draw::frame::drawer frame>
    inline void basic_edit<origin, frame>::paint (draw::graphics& graph) {
      auto area = frame(graph, client_geometry());
      area.shrink(core::size::one);
      look::edit_line(graph, area, data.text, draw::font::system(), super::get_foreground(), super::get_background(), origin,
                      data.selection, data.cursor_pos, data.scroll_pos, is_focused(), is_enabled());
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline void basic_edit<A, D>::init () {
      super::register_handler();
      super::on_paint(draw::paint(this, &basic_edit::paint));
    }

    // --------------------------------------------------------------------------
    template<text_origin_t A, char C, draw::frame::drawer D>
    inline basic_pass<A, C, D>::basic_pass () {
      init();
    }

    template<text_origin_t A, char C, draw::frame::drawer D>
    inline basic_pass<A, C, D>::basic_pass (const basic_pass& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin_t A, char C, draw::frame::drawer D>
    inline basic_pass<A, C, D>::basic_pass (basic_pass&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin_t origin, char character, draw::frame::drawer frame>
    inline void basic_pass<origin, character, frame>::paint (draw::graphics& graph) {
      auto area = frame(graph, client_geometry());
      area.shrink(core::size::one);
      std::string t(data.text.length(), character);
      look::edit_line(graph, area, t, draw::font::system(), super::get_foreground(), super::get_background(), origin,
                      data.selection, data.cursor_pos, data.scroll_pos, is_focused(), is_enabled());
    }

    template<text_origin_t A, char C, draw::frame::drawer D>
    inline void basic_pass<A, C, D>::init () {
      super::register_handler();
      super::on_paint(draw::paint(this, &basic_pass::paint));
    }

    template<text_origin_t A, char C, draw::frame::drawer D>
    std::string basic_pass<A, C, D>::get_selected_text () const {
      auto r = super::get_selection();
      return std::string(r.size(), C);
    }

  } // ctrl

} // gui

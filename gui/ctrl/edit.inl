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

    // --------------------------------------------------------------------------
    template<typename T, typename F, typename L>
    inline edit_t<T, F, L>::edit_t (T v, limiter_type l)
      : value{std::forward<T>(v)}
      , limiter(l)
    {
      set_insert_mode(default_insert_mode<T>());
    }

    template<typename T, typename F, typename L>
    inline void edit_t<T, F, L>::set (const type& v) {
      type new_value = limiter(v);
      if (!(value == new_value)) {
        value = new_value;
        super::invalidate();
        super::notify_content_changed();
      }
    }

    template<typename T, typename F, typename L>
    inline auto edit_t<T, F, L>::get () const -> type {
      return value;
    }

    template<typename T, typename F, typename L>
    inline void edit_t<T, F, L>::set_text (const std::string& t) {
      set(F::parse(t));
    }

    template<typename T, typename F, typename L>
    inline std::string edit_t<T, F, L>::get_text () const {
      return F::format(get());
    }

    template<typename T, typename F, typename L>
    inline void edit_t<T, F, L>::set_limiter (limiter_type l) {
      limiter = l;
      set(get());
    }

    template<typename T, typename F, typename L>
    inline auto edit_t<T, F, L>::get_limiter () const -> limiter_type {
      return limiter;
    }

    // --------------------------------------------------------------------------
    template<typename T, typename F, typename L, text_origin_t A, draw::frame::drawer D>
    inline basic_edit<T, F, L, A, D>::basic_edit (T t, limiter_type l)
      : super(std::forward<T>(t), std::forward<limiter_type>(l)) {
      init();
    }

    template<typename T, typename F, typename L, text_origin_t A, draw::frame::drawer D>
    inline basic_edit<T, F, L, A, D>::basic_edit (const basic_edit& rhs)
      : super(rhs)
    {
      init();
    }

    template<typename T, typename F, typename L, text_origin_t A, draw::frame::drawer D>
    inline basic_edit<T, F, L, A, D>::basic_edit (basic_edit&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<typename T, typename F, typename L, text_origin_t origin, draw::frame::drawer frame>
    inline void basic_edit<T, F, L, origin, frame>::paint (draw::graphics& graph) {
      auto area = frame(graph, super::client_geometry());
      area.shrink(core::size::one);
      look::edit_line(graph, area, super::get_text(), draw::font::system(),
                      super::get_foreground(), super::get_background(), origin,
                      super::get_selection(), super::get_cursor_pos(),
                      super::get_scroll_pos(), super::is_focused(), super::is_enabled());
    }

    template<typename T, typename F, typename L, text_origin_t A, draw::frame::drawer D>
    inline void basic_edit<T, F, L, A, D>::init () {
      super::register_handler();
      super::on_paint(draw::paint(this, &basic_edit::paint));
    }

    // --------------------------------------------------------------------------
    template<text_origin_t A, char C, draw::frame::drawer D>
    inline basic_pass<A, C, D>::basic_pass (const std::string& t)
      : super(t)
    {
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
      auto area = frame(graph, super::client_geometry());
      area.shrink(core::size::one);
      std::string t(get_text().length(), character);
      look::edit_line(graph, area, t, draw::font::system(), super::get_foreground(),
                      super::get_background(), origin, super::get_selection(),
                      super::get_cursor_pos(), super::get_scroll_pos(),
                      super::is_focused(), super::is_enabled());
    }

    template<text_origin_t A, char C, draw::frame::drawer D>
    inline void basic_pass<A, C, D>::init () {
      super::register_handler();
      super::on_paint(draw::paint(this, &basic_pass::paint));
    }

    template<text_origin_t A, char C, draw::frame::drawer D>
    std::string basic_pass<A, C, D>::get_selected_text () const {
      auto r = super::get_selection();
      return std::string(r.end() - r.begin(), C);
    }

  } // ctrl

} // gui

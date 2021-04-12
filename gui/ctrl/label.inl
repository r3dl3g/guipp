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
 * @brief     C++ API: label
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      template<typename T>
      inline void label_base::operator<< (const T& t) {
        set_text(util::string::convert::from(t));
      }

      template<typename T>
      inline void label_base::operator>> (T& t) const {
        t = get_text();
      }

      template<typename T>
      inline void label_base::operator>> (T&& t) const {
        t = get_text();
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin_t A, draw::frame::drawer D>
    inline basic_label<A, D>::basic_label (const std::string& t)
      : super(t) {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline basic_label<A, D>::basic_label (const text_source& t)
      : super(t) {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline basic_label<A, D>::basic_label (const basic_label& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline basic_label<A, D>::basic_label (basic_label&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin_t align, draw::frame::drawer frame>
    inline void basic_label<align, frame>::paint (draw::graphics& graph) {
      gui::core::rectangle place = frame(graph, client_geometry());
      look::label(graph, place, get_text(), get_foreground(), get_background(), align);
    }

    template<text_origin_t A, draw::frame::drawer D>
    inline void basic_label<A, D>::init () {
      on_paint(draw::paint(this, &basic_label::paint));
    }

    // --------------------------------------------------------------------------

  } // namespace ctrl

} // namespace gui

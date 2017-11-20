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
 * @brief     C++ API: label
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

    namespace detail {

      inline void label_base::create (const container& parent,
                                      const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
      }

      inline void label_base::create (const container& parent,
                                      const text_source& txt,
                                      const core::rectangle& place) {
        create(parent, place);
        set_text(txt);
      }

      inline void label_base::create (const container& parent,
                                      const std::string& txt,
                                      const core::rectangle& place) {
        create(parent, const_text(txt), place);
      }

      // --------------------------------------------------------------------------
      template<typename T>
      inline void label_base::operator<< (const T& t) {
        set_text(convert_to_string(t));
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
    template<text_origin A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_label<A, D, F, B>::basic_label () {
      init();
    }

    template<text_origin A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_label<A, D, F, B>::basic_label (const basic_label& rhs)
      : super(rhs)
    {
      init();
    }

    template<text_origin A, draw::frame::drawer D, os::color F, os::color B>
    inline basic_label<A, D, F, B>::basic_label (basic_label&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<text_origin align, draw::frame::drawer frame, os::color foreground, os::color background>
    inline void basic_label<align, frame, foreground, background>::paint (const draw::graphics& graph) {
      gui::core::rectangle place = client_area();
      paint::label(graph, client_area(), get_text(), foreground, background, align);
      frame(graph, place);
    }

    template<text_origin A, draw::frame::drawer D, os::color F, os::color B>
    inline void basic_label<A, D, F, B>::init () {
      register_event_handler(REGISTER_FUNCTION, paint_event(this, &basic_label::paint));
    }

    // --------------------------------------------------------------------------

  } // win

} // gui

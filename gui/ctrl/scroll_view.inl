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
 * @brief     C++ API: scrolled views
 *
 * @file
 */

#pragma once


namespace gui {

  namespace layout {

    inline bool scroll_view::is_in_scroll_event () const {
      return in_scroll_event;
    }

    template<typename T>
    inline virtual_layout<T>::virtual_layout (win::container* main)
      : super(main)
    {
      super::init(core::bind_method(this, &virtual_layout::layout));
    }

    template<typename T>
    inline void virtual_layout<T>::init (win::vertical_scroll_bar* vscroll,
                                         win::horizontal_scroll_bar* hscroll,
                                         win::window* edge,
                                         view_type* client) {
      super::init(vscroll, hscroll, edge);
      this->client = client;
    }

    template<typename T>
    inline void virtual_layout<T>::layout (const core::size& new_size) {
      if (client) {
        auto available = super::layout(new_size, client->get_virtual_place());
        client->place(available);
      } else {
        super::layout(new_size, core::rectangle());
      }
    }

  } // namespace layout

  namespace win {

    template<typename T, os::color B>
    void virtual_view<T, B>::create (const container& parent,
                                     const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

    template<typename T, os::color B>
    virtual_view<T, B>::virtual_view () {
      super::register_event_handler(REGISTER_FUNCTION, create_event(this, &virtual_view::handle_create));
      super::get_layout().init(&vscroll, &hscroll, &edge, &view);

      vscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (core::point::type y) {
        view.set_scroll_pos(core::point(hscroll.get_value(), y));
      }));
      hscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (core::point::type x) {
        view.set_scroll_pos(core::point(x, vscroll.get_value()));
      }));
      view.register_event_handler(REGISTER_FUNCTION, content_changed_event([&] () {
        super::layout();
      }));
      view.register_event_handler(REGISTER_FUNCTION, selection_changed_event([&] (event_source) {
        view.make_selection_visible();
        const core::point& pos = view.get_scroll_pos();
        hscroll.set_value(pos.x(), false);
        vscroll.set_value(pos.y(), false);
        super::layout();
      }));
      view.register_event_handler(REGISTER_FUNCTION, wheel_x_event([&] (const core::point::type delta,
                                                                        const core::point & pt) {
        hscroll.handle_wheel(delta, pt);
      }));
      view.register_event_handler(REGISTER_FUNCTION, wheel_y_event([&] (const core::point::type delta,
                                                                        const core::point & pt) {
        vscroll.handle_wheel(delta, pt);
      }));
    }

    template<typename T, os::color B>
    void virtual_view<T, B>::handle_create (window*, const core::rectangle& r) {
      core::size sz = r.size();
      vscroll.create(*this, layout::scroll_view::get_vscroll_area(sz, true));
      hscroll.create(*this, layout::scroll_view::get_hscroll_area(sz, true));
      edge.create(*this, layout::scroll_view::get_edge_area(sz));
      view.create(*this, r);
      view.set_visible();
    }

  } // namespace win

} // gui

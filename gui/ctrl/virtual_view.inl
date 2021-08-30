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
 * @brief     C++ API: scrolled views
 *
 * @file
 */

#pragma once


namespace gui {

  namespace layout {

    template<typename T>
    inline virtual_layout<T>::virtual_layout ()
      : client(nullptr)
    {}

    template<typename T>
    inline void virtual_layout<T>::init (ctrl::vertical_scroll_bar* vscroll,
                                         ctrl::horizontal_scroll_bar* hscroll,
                                         win::window* edge,
                                         view_type* client) {
      super::init(vscroll, hscroll, edge);
      this->client = client;
    }

    template<typename T>
    inline void virtual_layout<T>::layout (const core::rectangle& r) const {
      logging::trace() << "virtual_layout::layout()";
      if (client) {
        auto available = super::layout(r, client->get_virtual_geometry(r));
        client->geometry(available);
      } else {
        super::layout(r, core::rectangle());
      }
    }

  } // namespace layout

  namespace ctrl {

    template<typename T, os::color B>
    void virtual_view<T, B>::create (win::container& parent,
                                     const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

    template<typename T, os::color B>
    template<typename ... Args>
    virtual_view<T, B>::virtual_view (const Args& ... args)
      : view(args...) {
      init();
    }

    template<typename T, os::color B>
    virtual_view<T, B>::virtual_view () {
      init();
    }

    template<typename T, os::color B>
    auto virtual_view<T, B>::operator-> () -> view_type* {
      return &view;
    }

    template<typename T, os::color B>
    auto virtual_view<T, B>::operator-> () const -> const view_type* {
      return &view;
    }

    template<typename T, os::color B>
    void virtual_view<T, B>::init () {
      super::on_create(util::bind_method(this, &virtual_view::handle_create));
      super::get_layout().init(&vscroll, &hscroll, &edge, &view);

      vscroll.on_scroll([&] (core::point::type y) {
        view.set_scroll_pos(core::point(hscroll.get_value(), y));
      });
      hscroll.on_scroll([&] (core::point::type x) {
        view.set_scroll_pos(core::point(x, vscroll.get_value()));
      });
      view.on_content_changed([&] () {
        super::layout();
      });
      super::on_show([&] () {
        super::layout();
      });
      view.on_wheel_x([&] (const core::point::type delta, const core::native_point& pt) {
        hscroll.handle_wheel(delta, pt);
      });
      view.on_wheel_y([&] (const core::point::type delta, const core::native_point& pt) {
        vscroll.handle_wheel(delta, pt);
      });
    }

    template<typename T, os::color B>
    void virtual_view<T, B>::handle_create () {
      core::rectangle r = super::client_geometry();
      vscroll.create(*this, layout::scroll_view::get_vscroll_area(r, true));
      hscroll.create(*this, layout::scroll_view::get_hscroll_area(r, true));
      edge.create(*this, layout::scroll_view::get_edge_area(r));
      view.create(*this, r);
      view.set_visible();
      auto steps = view.get_scroll_steps();
      vscroll.set_step(steps.height());
      hscroll.set_step(steps.width());
      super::layout();
    }

  } // namespace ctrl

} // gui

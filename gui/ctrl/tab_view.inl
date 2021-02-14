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
 * @brief     C++ API: list
 *
 * @file
 */


namespace gui {

  namespace ctrl {

    template<alignment_t A, os::color B, typename G, typename L>
    tab_view<A, B, G, L>::tab_view () {
      super::get_layout().set_header(layout::lay(buttons));

      buttons.on_selection_changed([&] (event_source) {
        show_page(buttons.get_selection_index());
      });

      super::on_create([&] () {
        buttons.create(*this);
      });
      super::on_paint(draw::paint([&] (draw::graphics&  graph) {
        graph.clear(B);
      }));
      super::on_show([& ]() {
        if (!pages.empty()) {
          show_page(0);
        }
      });
    }

    template<alignment_t A, os::color B, typename G, typename L>
    void tab_view<A, B, G, L>::show_page (int idx) {
      if ((idx >= 0) && (idx < pages.size())) {
        super::get_layout().set_body(layout::lay(pages[idx].get()));
        for (int i = 0; i < pages.size(); ++i) {
          pages[i].get().set_visible(i == idx);
        }
        super::layout();
      }
    }

    template<alignment_t A, os::color B, typename G, typename L>
    void tab_view<A, B, G, L>::add_page (const std::string& label, win::window& page) {
      buttons.add_button(label);
      if (pages.empty()) {
        super::get_layout().set_body(layout::lay(page));
        buttons.get_button(0).get()->set_checked(true);
      } else {
        page.set_visible(false);
      }

      pages.push_back(page);
    }

  } // ctrl

} // gui

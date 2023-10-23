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
 * @brief     multi page view with tab group control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */


namespace gui {

  namespace ctrl {

    template<alignment_t A, typename G, typename L>
    tab_view<A, G, L>::tab_view () {
      super::set_background(color::very_very_light_gray);
      super::get_layout().set_header(layout::lay(buttons));

      buttons.on_selection_changed([&] (event_source) {
        show_page(buttons.get_selection_index());
      });

      super::on_create([&] () {
        buttons.create(*this);
      });
      super::on_show([& ]() {
        if (!pages.empty()) {
          show_page(0);
        }
      });
    }

    template<alignment_t A, typename G, typename L>
    void tab_view<A, G, L>::show_page (int idx) {
      if ((idx >= 0) && (idx < pages.size())) {
        super::get_layout().set_body(layout::lay(pages[idx].get()));
        for (int i = 0; i < pages.size(); ++i) {
          pages[i].get().set_visible(i == idx);
        }
        super::layout();
      }
    }

    template<alignment_t A, typename G, typename L>
    void tab_view<A, G, L>::add_page (const std::string& label, win::window& page) {
      buttons.add_button(label);
      if (pages.empty()) {
        super::get_layout().set_body(layout::lay(page));
        buttons.get_button(0).get()->set_checked(true);
        buttons.set_selection_index(0);
      } else {
        page.set_visible(false);
      }

      pages.push_back(page);
    }

    template<alignment_t A, typename G, typename L>
    void tab_view<A, G, L>::on_selection_changed (std::function<void(event_source)>&& f) {
      buttons.on_selection_changed(std::move(f));
    }

    template<alignment_t A, typename G, typename L>
    int tab_view<A, G, L>::get_selection_index () const {
      return buttons.get_selection_index();
    }

  } // ctrl

} // gui

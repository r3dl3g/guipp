/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     basic layout definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window.h>
#include <gui/layout/layout.h>


namespace gui {

  namespace layout {

    layout_function lay (win::window& w) {
      return [&] (const core::rectangle& r) {
        w.geometry(r, true, false);
      };
    }

    layout_function lay (win::window* w) {
      return [=] (const core::rectangle& r) {
        w->geometry(r, true, false);
      };
    }

    // --------------------------------------------------------------------------
    layout_base::layout_base (const std::vector<layout_function>& list) {
      add(list);
    }

    auto layout_base::get_elements () const -> const element_list& {
      return elements;
    }

    void layout_base::add (const layout_function& e, bool is_separator) {
      elements.emplace_back(e, is_separator);
    }

    void layout_base::add (layout_function&& e, bool is_separator) {
      elements.emplace_back(std::move(e), is_separator);
    }

    void layout_base::add (win::window& w) {
      add(lay(w));
    }

    void layout_base::add (win::window* w) {
      add(lay(w));
    }

    void layout_base::add (std::vector<layout_function> list) {
      for (auto& l : list) {
        add(l);
      }
    }

    void layout_base::add (std::vector<win::window*> list) {
      for (win::window* w : list) {
        add(lay(w));
      }
    }

    void layout_base::add (std::vector<std::reference_wrapper<win::window>> list) {
      for (auto& w : list) {
        add(lay(w.get()));
      }
    }

    void layout_base::remove_all () {
      elements.clear();
    }

  } // layout

} // gui

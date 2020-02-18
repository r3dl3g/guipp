/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: layout
 *
 * @file
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
        w.place(r);
      };
    }

    layout_function lay (win::window* w) {
      return [=] (const core::rectangle& r) {
        w->place(r);
      };
    }

    // --------------------------------------------------------------------------
    layout_base::layout_base (std::initializer_list<layout_function> list) {
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

    void layout_base::add (std::initializer_list<layout_function> list) {
      for (auto l : list) {
        add(l);
      }
    }

    void layout_base::add (std::initializer_list<win::window*> list) {
      for (win::window* w : list) {
        add(lay(w));
      }
    }

    void layout_base::remove_all () {
      elements.clear();
    }

  } // layout

} // gui

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
#include "gui/win/window.h"
#include "gui/layout/layout.h"


namespace gui {

  namespace layout {

    layout_function::layout_function ()
    {}

    layout_function::layout_function (win::window* w, bool sep)
      : data(window(w, sep))
    {}

    layout_function::layout_function (function& f)
      : data(f)
    {}

    layout_function::layout_function (function&& f)
      : data(std::move(f))
    {}

    void layout_function::operator() (const core::rectangle& r) const {
      if (std::holds_alternative<window>(data)) {
        window w = std::get<window>(data);
        if (w.win->is_visible()) {
          w.win->geometry(r, true, false);
        }
      } else {
        std::get<function>(data)(r);
      }
    }

    layout_function::operator bool() const {
      if (std::holds_alternative<window>(data)) {
        return std::get<window>(data).win != nullptr;
      } else {
        return (bool)std::get<function>(data);
      }
    }

    bool layout_function::is_visible () const {
      if (std::holds_alternative<window>(data)) {
        return std::get<window>(data).win->is_visible();
      } else {
        return true;
      }
    }

    bool layout_function::is_separator () const {
      if (std::holds_alternative<window>(data)) {
        return std::get<window>(data).is_separator;
      } else {
        return false;
      }
    }

    layout_function::window::window ()
      : win(nullptr)
      , is_separator(false)
    {}
        
    layout_function::window::window (win::window* w, bool separator)
      : win(w)
      , is_separator(separator)
    {}

    // --------------------------------------------------------------------------
    layout_function lay (win::window& w) {
      return {&w};
    }

    layout_function lay (win::window* w) {
      return {w};
    }

    // --------------------------------------------------------------------------
    layout_base::layout_base (const std::vector<layout_function>& list) {
      add(list);
    }

    auto layout_base::get_elements () const -> const element_list& {
      return elements;
    }

    std::size_t layout_base::visible_count () const {
      return std::count_if(elements.begin(), elements.end(), [] (const layout_function& l) {
        return l.is_visible();
      });
    }

    std::size_t layout_base::separator_count () const {
      return std::count_if(elements.begin(), elements.end(), [] (const layout_function& l) {
        return l.is_separator();
      });
    }

    void layout_base::add (const layout_function& e) {
      elements.emplace_back(e);
    }

    void layout_base::add (layout_function&& e) {
      elements.emplace_back(std::move(e));
    }

    void layout_base::add (win::window& w, bool is_separator) {
      elements.emplace_back(&w, is_separator);
    }

    void layout_base::add (win::window* w, bool is_separator) {
      elements.emplace_back(w, is_separator);
    }

    void layout_base::add (std::vector<layout_function> list) {
      for (auto& l : list) {
        add(l);
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

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
 * @brief     C++ API: drop down list
 *
 * @file
 */

#pragma once


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    inline drop_down::data::data ()
      : main(nullptr)
      , button(nullptr)
    {}

  } // layout

  // --------------------------------------------------------------------------
  namespace ctrl {

    template<typename F>
    inline void drop_down_list::set_data (const std::vector<F>& data) {
      data.items.set_data(indirect_dropdown_data<F>(data));
    }

    template<typename F>
    inline void drop_down_list::set_data (std::initializer_list<F> args) {
      data.items.set_data(const_dropdown_data<F>(args));
    }

    inline void drop_down_list::set_data (std::function<list_data_provider> dta) {
      data.items.set_data(dta);
    }

  } // ctrl

} // gui

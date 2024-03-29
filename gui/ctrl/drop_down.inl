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
 * @brief     drop down list control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  // --------------------------------------------------------------------------
  namespace ctrl {

    template<typename F, list_item_drawer<F> D, typename V>
    inline void drop_down_list::set_data (const V& dt) {
      data.items->set_data(indirect_dropdown_data<F, D, V>(dt));
    }

    template<typename F, list_item_drawer<F> D>
    inline void drop_down_list::set_data (std::initializer_list<F> args) {
      data.items->set_data(const_dropdown_data<F, D>(args));
    }

    inline void drop_down_list::set_data (std::function<list_data_provider> dta) {
      data.items->set_data(dta);
    }

  } // ctrl

} // gui

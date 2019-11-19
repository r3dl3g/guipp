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
 * @brief     C++ API: separator view
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/column_list.h>
#include <gui/ctrl/file_tree.h>
#include <util/tuple_sort.h>

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename Layout, typename ... Arguments>
    class sorted_column_list_t : public gui::ctrl::column_list_t<Layout, Arguments ...> {
    public:
      typedef gui::ctrl::column_list_t<Layout, Arguments ...> super;
      typedef std::tuple<Arguments ...> data_t;
      typedef bool (sort_callback) (util::sort::order o, int column);

      sorted_column_list_t (gui::core::size::type item_size = 20,
                            gui::os::color background = gui::color::white,
                            bool grab_focus = true);
      sorted_column_list_t (const sorted_column_list_t& rhs);
      sorted_column_list_t (sorted_column_list_t&& rhs);

      void on_sort (std::function<sort_callback> sorter);
      void sort ();

      std::vector<std::string> header_label;
      std::function<sort_callback> sorter;

    private:
      gui::core::point mouse_down_point;
      int sort_column;
      util::sort::order sort_dir;

      void init ();
    };

  } // ctrl

} // gui

#include <gui/ctrl/sorted_column_list.inl>

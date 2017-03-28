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
* @brief     C++ API: basic controls
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "list.h"


namespace gui {

  namespace win {

    namespace detail {
    }

    class column_list : public owner_draw {
    public:
      struct column {
        column (core::size::type width, float weight = 0.0F)
          : min_width(width)
          , width(width)
          , weight(weight)
        {}

        std::string       title;
        core::size::type  min_width;
        core::size::type  width;
        float		          weight;
      };

      typedef std::vector<column> columns_t;

      typedef void(cell_draw)(draw::graphics&,
                              int idx, int idy,
                              const core::rectangle& place,
                              bool selected);


      // --------------------------------------------------------------------------
      // static data for list.
      // --------------------------------------------------------------------------
      template<typename T>
      struct data : public std::vector<std::vector<T>> {
        typedef std::vector<std::vector<T>> super;

        typedef typename super::iterator iterator;

        data()
        {}

        data (std::initializer_list<std::initializer_list<T>> args)
          : super(args)
        {}

        data (iterator b, iterator e)
          : super(b, e)
        {}

        template<size_t N>
        data (const T(&t) [N])
          : super(t, t + N)
        {}

        template<typename L>
        void update_list (L& l) {
          l.set_count(super::size());
        }

        void operator() (draw::graphics& g,
                         int idx, int idy,
                         const core::rectangle& place,
                         bool selected) {
          draw_text_item(g, ostreamfmt(super::at(idx).at(idy)), place, selected);
          g.draw_relief(place, true);
        }

      };

      column_list();
      ~column_list();

      void set_drawer (std::function<cell_draw> drawer,
                       int item_height = 20);

      const columns_t& get_columns () const {
        return columns;
      }

      void set_columns (const columns_t& val) {
        columns = val;
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      template<typename T>
      void create (const container& parent,
                   const core::rectangle& place,
                   const columns_t& val,
                   data<T> data,
                   int item_height = 20) {
        create(parent, place);
        set_columns(val);
        set_data(data, item_height);
      }

      template<typename T>
      void set_data (data<T> data,
                     int item_height = 20) {
        set_drawer(data, item_height);
        set_count(data.size());
      }

      inline void set_count (size_t count) {
        list.set_count(count);
      }

      inline size_t get_count () const {
        return list.get_count();
      }

      inline void set_selection (int count) {
        list.set_selection(count);
      }

      inline int get_selection () const {
        return list.get_selection();
      }

      inline void set_scroll_pos (int pos) {
        list.set_scroll_pos(pos);
      }

      inline int get_scroll_pos () const {
        return list.get_scroll_pos();
      }

      inline void enable_vscroll_bar (bool enable) {
        list.enable_vscroll_bar(enable);
      }

      inline bool is_vscroll_bar_enabled () const {
        return list.is_vscroll_bar_enabled();
      }

    private:
      void draw_line (draw::graphics& g,
                      int idx,
                      const core::rectangle& place,
                      bool selected);

      //bool column_list_handle_event (const core::event& e,
      //                               core::event_result& result);

      win::list list;
      columns_t columns;
      std::function<cell_draw> drawer;

      static window_class clazz;
    };

  } // win

} // gui



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
* @brief     C++ API: basic window
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include<ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    class owner_draw : public gui::win::window {
    public:
      typedef gui::win::window super;

      owner_draw ()
        : owner_draw_id(++next_owner_draw_id)
      {}

      int get_owner_draw_id () const {
        return owner_draw_id;
      }

      void set_item_size (const gui::core::size&);

      const gui::core::size& get_item_size () const;

      inline gui::core::size::type get_item_width () const {
        return get_item_size().width();
      }

      inline gui::core::size::type get_item_height () const {
        return get_item_size().height();
      }

      static const gui::core::size& get_item_size (int id);

    protected:
      void create (const gui::win::window_class& clazz,
                   const gui::win::window& parent,
                   const gui::core::rectangle& place = gui::core::rectangle::default_rectangle) {
        super::create(clazz, parent, place, (gui::core::menu_id)(long long)(get_owner_draw_id()));
      }

    private:
      gui::core::size item_size;

      const int owner_draw_id;

      static int next_owner_draw_id;

      typedef std::map<int, gui::core::size> measure_item_map;
      static measure_item_map measure_item_size;
    };

  } // win

} // gui

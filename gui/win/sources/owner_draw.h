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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"
#include "graphics.h"


namespace gui {

  namespace win {

    namespace paint {
      void text_item (const std::string& text,
                      draw::graphics&,
                      const core::rectangle& place,
                      const draw::brush& background,
                      bool selected,
                      draw::text_origin origin = draw::vcenter_left);
    }

    // --------------------------------------------------------------------------
    class owner_draw : public window {
    public:
      typedef window super;

      owner_draw ();
      ~owner_draw ();

#ifdef WIN32
      int get_owner_draw_id () const {
        return owner_draw_id;
      }
#endif // WIN32

      void set_item_size (const gui::core::size&);

      const gui::core::size& get_item_size () const;

      inline gui::core::size::type get_item_width () const {
        return get_item_size().width();
      }

      inline gui::core::size::type get_item_height () const {
        return get_item_size().height();
      }

#ifdef WIN32
      static const gui::core::size& get_item_size (int id);
#endif // WIN32
#ifdef X11
      os::graphics get_graphics (const core::event& e);
#endif // X11

    protected:
      void create (const gui::win::window_class& clazz,
                   const gui::win::container& parent,
                   const gui::core::rectangle& place = gui::core::rectangle::def) {
#ifdef WIN32
        super::create(clazz, parent, place, (os::menu)(long long)(get_owner_draw_id()));
#else
        super::create(clazz, parent, place);
#endif
      }

    private:
      gui::core::size item_size;

#ifdef WIN32
      const int owner_draw_id;

      static int next_owner_draw_id;

      typedef std::map<int, gui::core::size> measure_item_map;
      static measure_item_map s_measure_item_size;
#endif // WIN32
#ifdef X11
      os::graphics gc;
#endif // X11
    };

  } // win

} // gui

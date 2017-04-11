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

#include <map>
#include <algorithm>
#include "owner_draw.h"

namespace gui {

  namespace win {
    // --------------------------------------------------------------------------
#ifdef WIN32
    owner_draw::measure_item_map owner_draw::s_measure_item_size;
    int owner_draw::next_owner_draw_id = 0;
#endif // WIN32

    namespace paint {
      // --------------------------------------------------------------------------
      void text_item (const std::string& text,
                      draw::graphics& g,
                      const core::rectangle& place,
                      const draw::brush& background,
                      bool selected,
                      draw::text_origin origin) {
        using namespace draw;
        g.fill(rectangle(place), selected ? color::highLightColor() : background);
        g.text(text_box(text, place, origin), font::system(),
               selected ? color::highLightTextColor() : color::windowTextColor());
      }
    }

#ifdef WIN32
    owner_draw::owner_draw ()
      : item_size(1, 1)
      , owner_draw_id(++next_owner_draw_id)
    {}

    owner_draw::~owner_draw ()
    {}

#endif // WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    owner_draw::owner_draw ()
      : item_size(1, 1)
      , gc(0)
    {}

    owner_draw::~owner_draw () {
      if (gc) {
        if (core::global::get_instance()) {
          XFreeGC(core::global::get_instance(), gc);
        }
        gc = 0;
      }
    }

    os::graphics owner_draw::get_graphics (const core::event& e) {
      if (!gc) {
        gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
      }
      return gc;
    }

#endif // X11

    void owner_draw::set_item_size (const gui::core::size& sz) {
      item_size = sz;
#ifdef WIN32
      s_measure_item_size[owner_draw_id] = sz;
#endif // WIN32
    }

    const gui::core::size& owner_draw::get_item_size () const {
      return item_size;
    }

#ifdef WIN32
    const gui::core::size& owner_draw::get_item_size (int id) {
      measure_item_map::iterator i = s_measure_item_size.find(id);
      if (i != s_measure_item_size.end()) {
        return i->second;
      }
      return gui::core::size::zero;
    }
#endif // WIN32

  } // win

} // gui

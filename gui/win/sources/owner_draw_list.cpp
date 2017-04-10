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
* @brief     C++ API: list
*
* @file
*/

#include "owner_draw_list.h"

namespace gui {

  namespace win {

#ifdef X11
    namespace detail {
      Atom SELECTION_CHANGE_MESSAGE = 0;
    }
#endif // X11

    // --------------------------------------------------------------------------
#ifdef WIN32
    owner_draw_list::owner_draw_list ()
    {}

    owner_draw_list::~owner_draw_list ()
    {}

#endif // WIN32

#ifdef X11

    // --------------------------------------------------------------------------
    owner_draw_list::owner_draw_list ()
      : gc(0)
    {
      if (!detail::SELECTION_CHANGE_MESSAGE) {
        detail::SELECTION_CHANGE_MESSAGE = XInternAtom(core::global::get_instance(), "SELECTION_CHANGE_MESSAGE", False);
      }
    }

    owner_draw_list::~owner_draw_list () {
      if (gc) {
        if (core::global::get_instance()) {
          XFreeGC(core::global::get_instance(), gc);
        }
        gc = 0;
      }
    }

    os::graphics owner_draw_list::get_graphics (const core::event& e) {
      if (!gc) {
        gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
      }
      return gc;
    }

#endif // X11

    void owner_draw_list::set_drawer (std::function<draw_list_item> drawer,
                                      const core::size& sz) {
      this->drawer = drawer;
      set_item_size(sz);
    }

    void owner_draw_list::draw_item (int idx,
                                     draw::graphics& g,
                                     const core::rectangle& place,
                                     bool selected) {
      if (drawer) {
        drawer(idx, g, place, draw::color::windowColor(), selected);
      }
    }

  }
}

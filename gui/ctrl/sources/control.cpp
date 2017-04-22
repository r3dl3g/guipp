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


// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "control.h"

namespace gui {

#ifdef WIN32
#endif // WIN32

#ifdef X11
#endif // X11

  namespace layout {
  }

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {
#ifdef X11
      Atom SCROLLBAR_MESSAGE = 0;
      Atom SELECTION_CHANGE_MESSAGE = 0;
      Atom SELECTION_COMMIT_MESSAGE = 0;
      Atom SLIDER_MESSAGE = 0;
      Atom BN_CLICKED_MESSAGE = 0;
      Atom BN_PUSHED_MESSAGE = 0;
      Atom BN_UNPUSHED_MESSAGE = 0;
      Atom BN_STATE_MESSAGE = 0;
#endif // X11
    } // detail

    void no_erase_window_class::prepare (window* win) const {
      window_class::prepare(win);
#ifdef X11
      unsigned long mask = CWBackPixmap;
      XSetWindowAttributes wa;
      wa.background_pixmap = None;
      XChangeWindowAttributes(core::global::get_instance(), win->get_id(), mask, &wa);
#endif // X11
    }

    os::brush no_erase_window_class::get_background_brush () const {
      return (os::brush)NULL;
    }

#ifdef WIN32

    // --------------------------------------------------------------------------
    template<>
    draw::graphics get_param<0, draw::graphics>(const core::event& e) {
      return draw::graphics(e.id, (os::graphics)e.param_1);
    }

    // --------------------------------------------------------------------------
    void paint_caller::operator()(const core::event& e) {
      if (f) {
        PAINTSTRUCT ps;
        os::graphics id = BeginPaint(e.id, &ps);
        f(draw::graphics(e.id, id));
        EndPaint(e.id, &ps);
      }
    }

#endif // WIN32
#ifdef X11

    template<>
    draw::graphics get_param<0, draw::graphics>(const core::event& e) {
      return draw::graphics(e.xany.window, get_param<0, os::graphics>(e));
    }

#endif // X11

    namespace paint {
      // --------------------------------------------------------------------------
      void text_item (const std::string& text,
                      const draw::graphics& g,
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

  } // win

} // gui

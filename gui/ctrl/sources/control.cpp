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
#include "bitmap.h"


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
      Atom HILITE_CHANGE_MESSAGE = 0;
      Atom SLIDER_MESSAGE = 0;
      Atom BN_CLICKED_MESSAGE = 0;
      Atom BN_PUSHED_MESSAGE = 0;
      Atom BN_UNPUSHED_MESSAGE = 0;
      Atom BN_STATE_MESSAGE = 0;
      Atom SELECTION_CANCEL_MESSAGE = 0;
      Atom CONTENT_CHANGED_MESSAGE = 0;

      void init_control_messages () {
        init_atom(SCROLLBAR_MESSAGE, "SCROLLBAR_MESSAGE");
        init_atom(SELECTION_CHANGE_MESSAGE, "SELECTION_CHANGE_MESSAGE");
        init_atom(SELECTION_COMMIT_MESSAGE, "SELECTION_COMMIT_MESSAGE");
        init_atom(HILITE_CHANGE_MESSAGE, "HILITE_CHANGE_MESSAGE");
        init_atom(SLIDER_MESSAGE, "SLIDER_MESSAGE");
        init_atom(BN_CLICKED_MESSAGE, "BN_CLICKED_MESSAGE");
        init_atom(BN_PUSHED_MESSAGE, "BN_PUSHED_MESSAGE");
        init_atom(BN_UNPUSHED_MESSAGE, "BN_UNPUSHED_MESSAGE");
        init_atom(BN_STATE_MESSAGE, "BN_STATE_MESSAGE");
        init_atom(SELECTION_CANCEL_MESSAGE, "SELECTION_CANCEL_MESSAGE");
        init_atom(CONTENT_CHANGED_MESSAGE, "CONTENT_CHANGED_MESSAGE");
      }

#endif // X11
    } // detail

    void no_erase_window_class::prepare (window* win, os::window parent_id) const {
      window_class::prepare(win, parent_id);
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
      return draw::graphics(e.id, (os::graphics)e.wParam);
    }

    // --------------------------------------------------------------------------
    template<>
    event_source get_param<0, event_source>(const core::event& e) {
      return event_source(e.wParam);
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
      void text_item (const draw::graphics& g,
                      const core::rectangle& place,
                      const draw::brush& background,
                      const std::string& text,
                      bool selected,
                      text_origin origin) {
        using namespace draw;
        g.fill(rectangle(place), selected ? color::highLightColor() : background);
        g.text(text_box(text, place, origin), font::system(),
               selected ? color::highLightTextColor() : color::windowTextColor());
      }
    }

  } // win

} // gui

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
// Library includes
//
#include <gui/draw/bitmap.h>
#include <gui/ctrl/control.h>


namespace gui {

  namespace layout {}

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
#endif // X11

      int init_control_messages () {
#ifdef X11
        x11::init_atom(SCROLLBAR_MESSAGE, "SCROLLBAR_MESSAGE");
        x11::init_atom(SELECTION_CHANGE_MESSAGE, "SELECTION_CHANGE_MESSAGE");
        x11::init_atom(SELECTION_COMMIT_MESSAGE, "SELECTION_COMMIT_MESSAGE");
        x11::init_atom(HILITE_CHANGE_MESSAGE, "HILITE_CHANGE_MESSAGE");
        x11::init_atom(SLIDER_MESSAGE, "SLIDER_MESSAGE");
        x11::init_atom(BN_CLICKED_MESSAGE, "BN_CLICKED_MESSAGE");
        x11::init_atom(BN_PUSHED_MESSAGE, "BN_PUSHED_MESSAGE");
        x11::init_atom(BN_UNPUSHED_MESSAGE, "BN_UNPUSHED_MESSAGE");
        x11::init_atom(BN_STATE_MESSAGE, "BN_STATE_MESSAGE");
        x11::init_atom(SELECTION_CANCEL_MESSAGE, "SELECTION_CANCEL_MESSAGE");
        x11::init_atom(CONTENT_CHANGED_MESSAGE, "CONTENT_CHANGED_MESSAGE");
#endif // X11
        return 1;
      }

    } // detail

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
    void paint_caller::operator() (const core::event& e) {
      if (f) {
        PAINTSTRUCT ps;
        os::graphics id = BeginPaint(e.id, &ps);
        f(draw::graphics(e.id, id));
        EndPaint(e.id, &ps);
      }
    }

#endif // WIN32
#ifdef X11

    draw::graphics get_draw_graphics (const core::event& e) {
      return draw::graphics(e.xany.window, get_graphics(e));
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
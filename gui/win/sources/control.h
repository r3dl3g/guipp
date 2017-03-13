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
#include "label.h"
#include "button.h"
#include "owner_draw.h"
#include "list.h"


namespace gui {

  namespace win {

    class scroll_bar : public window {
    public:
      typedef window super;

      scroll_bar ();
      ~scroll_bar ();

      void create (const window& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle,
                   bool horizontal = false) {
        super::create(clazz, parent, place);
      }

      int get_min () const;
      int get_max () const;
      int get_step () const;
      int get_current () const;

      void set_min (int);
      void set_max (int);
      void set_step (int);
      void set_current (int);

    private:
#ifdef X11
      bool scroll_handle_event (const core::event& e,
                                core::event_result& result);
      int min;
      int max;
      int step;
      int current;
      core::point last_mouse_point;
      core::graphics_id gc;
      bool horizontal;
#endif // X11
      static window_class clazz;
    };

  } // win

} // gui

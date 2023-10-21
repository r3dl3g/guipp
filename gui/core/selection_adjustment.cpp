/**
 * @copyright (c) 2016-2023 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     selection adjustment
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/selection_adjustment.h>


namespace gui {

  namespace core {

    int get_adjusted_scroll_position (float space,
                                      float item_size,
                                      float scroll_pos,
                                      float sel_pos,
                                      selection_adjustment adjust) {
      if (selection_adjustment::center_always == adjust) {
        return (sel_pos - (space / 2) + item_size);
      } else if (sel_pos < scroll_pos) {
        switch (adjust) {
          case selection_adjustment::start:
          case selection_adjustment::next:
            return (sel_pos);
            break;
          case selection_adjustment::center:
            return (sel_pos - (space / 2) + item_size);
            break;
          case selection_adjustment::end:
            return (sel_pos - space + item_size);
            break;
        }
      } else if (sel_pos + item_size - scroll_pos > space) {
        switch (adjust) {
          case selection_adjustment::start:
            return (sel_pos);
            break;
          case selection_adjustment::center:
            return (sel_pos - (space / 2) + item_size);
            break;
          case selection_adjustment::end:
          case selection_adjustment::next:
            return (sel_pos - space + item_size);
            break;
        }
      }
      return scroll_pos;
    }


  } // core

} // gui

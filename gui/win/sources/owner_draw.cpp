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
    owner_draw::measure_item_map owner_draw::s_measure_item_size;
    int owner_draw::next_owner_draw_id = 0;

// --------------------------------------------------------------------------
    void owner_draw::draw_text_item (const std::string& text,
                                     draw::graphics& g,
                                     const core::rectangle& place,
                                     bool selected,
                                     draw::text_origin origin) {
      using namespace draw;
      g.fill(rectangle(place), selected ? color::highLightColor : color::white);
      g.text(text_box(text, place, origin), font::system(),
             selected ? color::highLightTextColor : color::windowTextColor);
    }

    void owner_draw::set_item_size (const gui::core::size& sz) {
      item_size = sz;
      s_measure_item_size[owner_draw_id] = sz;
    }

    const gui::core::size& owner_draw::get_item_size () const {
      return item_size;
    }

    const gui::core::size& owner_draw::get_item_size (int id) {
      measure_item_map::iterator i = s_measure_item_size.find(id);
      if (i != s_measure_item_size.end()) {
        return i->second;
      }
      return gui::core::size::zero;
    }

  } // win

} // gui

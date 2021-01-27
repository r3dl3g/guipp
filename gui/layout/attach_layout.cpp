/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: layout
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
#include <gui/layout/attach_layout.h>

namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    void attach::layout (const core::rectangle&) const {

      struct place_and_size {
        place_and_size ()
          : attach(nullptr)
        {}

        core::rectangle place;
        core::size size;
        const attachment* attach;
      };

      using source_places_t = std::map<const void*, place_and_size>;
      using target_places_t = std::map<const void*, place_and_size>;

      source_places_t splaces;
      target_places_t tplaces;

      for (const auto& a : attachments) {
        auto s_key = a->source_key();
        auto s_it = splaces.find(s_key);

        place_and_size source;
        if (s_it != splaces.end()) {
          source = s_it->second;
        } else {
          source.place = a->source_place();
          source.size = source.place.size();
          source.attach = a.get();
          splaces[s_key] = source;
        }

        auto t_key = a->target_key();
        auto t_it = tplaces.find(t_key);
        if (t_it != tplaces.end()) {
          a->adjust(t_it->second.place, source.size, source.place);
        } else {
          place_and_size& r = tplaces[t_key];
          r.place = a->target_place();
          r.size = r.place.size();
          r.attach = a.get();
          core::size diff = r.place.size() - r.size;
          a->adjust(r.place, source.size, source.place);
          r.size = r.place.size() - diff;
        }
      }
      for (auto& i : tplaces) {
        i.second.attach->place(i.second.place);
      }
    }

    // --------------------------------------------------------------------------
  } // layout

} // gui

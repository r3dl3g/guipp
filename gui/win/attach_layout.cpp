/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
#include <map>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/attach_layout.h>
#include <gui/win/container.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    void attach::init (win::container* main) {
#ifdef NDEBUG
      main->register_event_handler(REGISTER_FUNCTION, win::layout_event(this, &attach::layout));
#else
      main->register_event_handler(REGISTER_FUNCTION, win::layout_event([&, main] (const core::size & sz) {
//        LogDebug << "attach size_event " << main->get_class_name() << " " << sz;
        layout(sz);
      }));
#endif
    }

    void attach::layout (const core::size&) {
      typedef std::pair<core::rectangle, core::size> place_and_size;
      typedef std::map<win::window*, place_and_size> window_places;
      typedef window_places::iterator iterator;
      window_places places;

      for (detail::attachment a : attachments) {
        iterator i = places.find(a.target);
        iterator j = places.find(a.source);

        place_and_size source;
        if (j != places.end()) {
          source = j->second;
        } else {
          source.first = a.source->place();
          source.second = a.source->client_size();
        }

        if (i != places.end()) {
          a.adjust(i->second.first, source.second, source.first);
        } else {
          place_and_size& r = places[a.target];
          r.first = a.target->place();
          r.second = a.target->client_size();
          core::size diff = r.first.size() - r.second;
          a.adjust(r.first, source.second, source.first);
          r.second = r.first.size() - diff;
        }
      }
      for (auto i : places) {
        i.first->place(i.second.first);
      }
    }

  } // layout

} // gui

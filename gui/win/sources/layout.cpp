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
#include "layout.h"
#include "window.h"

namespace gui {

  namespace layout {

    std::vector<win::window*> layout_base::get_children (win::container* main) {
      return main->get_children();
    }

    void layout_base::place_child (win::window* win, const core::rectangle& area) {
      win->place(area);
      win->set_visible();
    }

    void layout_base::hide_children (std::vector<win::window*>& children) {
      std::for_each(children.begin(), children.end(), [](win::window* win){
        win->set_visible(false);
      });
    }

    void layout_base::init(callback_function callback) {
      main->register_event_handler(win::size_event(callback));
    }

    attach::attach (win::container* main) {
      main->register_event_handler(win::size_event(core::bind_method(this, &attach::layout)));
    }

    void attach::layout (const core::size& sz) {
      typedef std::pair<core::rectangle, core::size> place_and_size;
      typedef std::map<win::window*, place_and_size> window_places;
      typedef window_places::iterator iterator;
      window_places places;

      for(detail::attachment a : attachments) {
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

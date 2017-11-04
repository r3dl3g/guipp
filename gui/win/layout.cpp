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
#include <gui/win/layout.h>
#include <gui/win/container.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    std::vector<win::window*> layout_base::get_children () {
      return main->get_children();
    }

    void layout_base::place_child (win::window* win, const core::rectangle& area) {
      win->place(area);
      win->set_visible();
    }

    bool layout_base::is_child_visible (win::window* win) {
      return win->is_visible();
    }

    void layout_base::hide_children (std::vector<win::window*>& children) {
      std::for_each(children.begin(), children.end(), [] (win::window*win) {
        win->set_visible(false);
      });
    }

    core::size layout_base::get_main_size () const {
      return main->client_size();
    }

    void layout_base::init (std::function<size_callback> f1) {
#ifdef NDEBUG
      main->register_event_handler(REGISTER_FUNCTION, win::size_event(f1));
      main->register_event_handler(REGISTER_FUNCTION, win::show_event([&, f1]() {
        f1(main->client_size());
      }));
#else
      main->register_event_handler(REGISTER_FUNCTION, win::size_event([&, f1](const core::size & sz) {
        LogDebug << "layout_base size_event " << main->get_class_name() << " " << sz;
        f1(sz);
      }));
      main->register_event_handler(REGISTER_FUNCTION, win::show_event([&, f1]() {
        core::size sz = main->client_size();
        LogDebug << "layout_base show_event " << main->get_class_name() << " " << sz;
        f1(sz);
      }));
#endif
    }

    void layout_base::init (std::function<size_callback> f1, std::function<show_callback> f2) {
#ifdef NDEBUG
      main->register_event_handler(REGISTER_FUNCTION, win::size_event(f1));
      main->register_event_handler(REGISTER_FUNCTION, win::show_event(f2));
#else
      main->register_event_handler(REGISTER_FUNCTION, win::size_event([&, f1](const core::size & sz) {
        LogDebug << "layout_base size_event " << main->get_class_name() << " " << sz;
        f1(sz);
      }));
      main->register_event_handler(REGISTER_FUNCTION, win::show_event([&, f2]() {
        core::size sz = main->client_size();
        LogDebug << "layout_base show_event " << main->get_class_name() << " " << sz;
        f2();
      }));
#endif
    }

    void layout_base::update () {
      main->redraw_later();
    }

    // --------------------------------------------------------------------------
    void attach::init (win::container* main) {
#ifdef NDEBUG
      main->register_event_handler(REGISTER_FUNCTION, win::size_event(this, &attach::layout));
#else
      main->register_event_handler(REGISTER_FUNCTION, win::size_event([&, main] (const core::size & sz) {
        LogDebug << "attach size_event " << main->get_class_name() << " " << sz;
        layout(sz);
      }));
#endif
    }

    void attach::layout (const core::size& sz) {
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

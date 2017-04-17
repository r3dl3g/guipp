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
* @brief     C++ API: list
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
#include "list.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {
      list::list ()
        : item_count(0)
        , selection(-1)
      {
#ifdef X11
        if (!detail::SELECTION_CHANGE_MESSAGE) {
          detail::SELECTION_CHANGE_MESSAGE = XInternAtom(core::global::get_instance(), "SELECTION_CHANGE_MESSAGE", False);
        }
#endif // X11
        register_event_handler(left_btn_down_event([&](const core::point& pt) {
          last_mouse_point = pt;
          moved = false;
        }));
      }

      size_t list::get_count () const {
        return item_count;
      }

      int list::get_selection () const {
        return selection;
      }

      list::~list () {
      }

      void list::set_drawer (std::function<draw_list_item> drawer,
                             const core::size& sz) {
        this->drawer = drawer;
        item_size = sz;
      }

      void list::draw_item (int idx,
                            const draw::graphics& g,
                            const core::rectangle& place,
                            bool selected) const {
        if (drawer) {
          drawer(idx, g, place, draw::color::windowColor(), selected);
        }
      }

    }

#ifdef WIN32
    // --------------------------------------------------------------------------
    template<>
    window_class listT<false>::clazz(win::window_class::custom_class("HLISTBOX++",
                                                                     CS_DBLCLKS,
                                                                     WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                                                     WS_EX_NOPARENTNOTIFY));
    // --------------------------------------------------------------------------
    template<>
    window_class listT<true>::clazz(win::window_class::custom_class("VLISTBOX++",
                                                                    CS_DBLCLKS,
                                                                    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                                                    WS_EX_NOPARENTNOTIFY));

#endif // WIN32

    // --------------------------------------------------------------------------

#ifdef X11
    template<>
    window_class listT<false>::clazz(window_class::custom_class("HLISTBOX++",
                                                                draw::color::white()));
    template<>
    window_class listT<true>::clazz(window_class::custom_class("VLISTBOX++",
                                                               draw::color::white()));
#endif // X11

    template<>
    void listT<false>::paint (const draw::graphics& graph) {
      core::rectangle place = client_area();
      const core::point_type max_x = place.x2();
      const int max_idx = (int)get_count();
      const int first = int(get_scroll_pos() / get_item_width());

      place.top_left({ get_item_width() * first - get_scroll_pos(), place.y()});
      place.width(get_item_width() - 1);

      for(int idx = first; (idx < max_idx) && (place.x() < max_x); ++idx, place.move_x(get_item_width())) {
        draw_item(idx, graph, place, get_selection() == idx);
      }
      graph.flush();
    }

    template<>
    void listT<true>::paint (const draw::graphics& graph) {
      core::rectangle place = client_area();
      const core::point_type max_y  = place.y2();
      const int max_idx = (int)get_count();
      const int first = int(get_scroll_pos() / get_item_height());

      place.top_left({place.x(), get_item_height() * first - get_scroll_pos()});
      place.height(get_item_height() - 1);

      for(int idx = first; (idx < max_idx) && (place.y() < max_y); ++idx, place.move_y(get_item_height())) {
        draw_item(idx, graph, place, get_selection() == idx);
      }
      graph.flush();
    }

    template<>
    core::size listT<false>::client_size () const {
      core::size sz = super::client_size();
      if (is_scroll_bar_visible()) {
        sz.height(sz.height() - scroll_bar::get_scroll_bar_width());
      }
      return sz;
    }

    template<>
    core::size listT<true>::client_size() const {
      core::size sz = super::client_size();
      if (is_scroll_bar_visible()) {
        sz.width(sz.width() - scroll_bar::get_scroll_bar_width());
      }
      return sz;
    }

    template<>
    void listT<false>::set_count (size_t count) {
      typedef core::size::type type;
      const type zero = type(0);

      item_count = count;

      const type iw = get_item_width();
      const type pw = size().width();
      const type w = (iw * (int)item_count) - pw;


      scrollbar.set_min_max_step(zero, std::max(w, zero), pw);
      scrollbar.set_visible((w > zero) && is_scroll_bar_enabled());

      redraw_later();
    }

    template<>
    void listT<true>::set_count (size_t count) {
      typedef core::size::type type;
      const type zero = type(0);

      item_count = count;

      const type ih = get_item_height();
      const type ph = size().height();
      const type h = (ih * (int)item_count) - ph;

      scrollbar.set_min_max_step(zero, std::max(h, zero), ph);
      scrollbar.set_visible((h > zero) && is_scroll_bar_enabled());

      redraw_later();
    }

    template<>
    void listT<false>::set_selection (int sel) {
      selection = std::min(std::max(0, sel), (int)get_count() - 1);
      // Make selection visible
      const core::point_type sel_pos = get_item_width() * selection;
      const core::size sz = size();

      if (sel_pos < get_scroll_pos()) {
        set_scroll_pos(sel_pos);
      } else if (sel_pos + get_item_width() - get_scroll_pos() > sz.width()) {
        set_scroll_pos(sel_pos + get_item_width() - sz.width());
      }
      send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
      redraw_later();
    }

    template<>
    void listT<true>::set_selection (int sel) {
      selection = std::min(std::max(0, sel), (int)get_count() - 1);
      // Make selection visible
      const core::point_type sel_pos = get_item_height() * selection;
      const core::size sz = size();

      if (sel_pos < get_scroll_pos()) {
        set_scroll_pos(sel_pos);
      } else if (sel_pos + get_item_height() - get_scroll_pos() > sz.height()) {
        set_scroll_pos(sel_pos + get_item_height() - sz.height());
      }
      send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
      redraw_later();
    }

    template<>
    void listT<false>::create (const container& parent,
                               const core::rectangle& place) {
      super::create(clazz, parent, place);
      scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area());
      adjust_scroll_bar();
    }

    template<>
    void listT<true>::create (const container& parent,
                              const core::rectangle& place) {
      super::create(clazz, parent, place);
      scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area());
      adjust_scroll_bar();
    }

    template<>
    void listT<false>::set_scroll_pos (core::point::type pos) {
      const core::point::type max_delta = std::max(0.0F, (get_item_width() * (core::point::type)get_count()) - size().width());
      scrollbar.set_value(std::min(std::max(0.0F, pos), max_delta));
      redraw_later();
    }

    template<>
    void listT<true>::set_scroll_pos (core::point::type pos) {
      const core::point::type max_delta = std::max(0.0F, (get_item_height() * (core::point::type)get_count()) - size().height());
      scrollbar.set_value(std::min(std::max(0.0F, pos), max_delta));
      redraw_later();
    }

    template<>
    core::point::type listT<false>::get_scroll_pos () const {
      return scrollbar.get_value();
    }

    template<>
    core::point::type listT<true>::get_scroll_pos () const {
      return scrollbar.get_value();
    }

    template<>
    core::rectangle listT<false>::get_scroll_bar_area () {
      core::rectangle r(size());
      r.y(r.y2() - scroll_bar::get_scroll_bar_width());
      r.height(scroll_bar::get_scroll_bar_width());
      return r;
    }

    template<>
    core::rectangle listT<true>::get_scroll_bar_area () {
      core::rectangle r(size());
      r.x(r.x2() - core::point::type(scroll_bar::get_scroll_bar_width()));
      r.width(scroll_bar::get_scroll_bar_width());
      return r;
    }

    template<>
    void listT<false>::enable_scroll_bar (bool enable) {
      scrollbar.enable(enable);
      scrollbar.set_visible(enable && scrollbar.get_max());
    }

    template<>
    void listT<true>::enable_scroll_bar (bool enable) {
      scrollbar.enable(enable);
      scrollbar.set_visible(enable && scrollbar.get_max());
    }

    template<>
    bool listT<false>::is_scroll_bar_enabled () const {
      return scrollbar.is_enabled();
    }

    template<>
    bool listT<true>::is_scroll_bar_enabled () const {
      return scrollbar.is_enabled();
    }

    template<>
    bool listT<false>::is_scroll_bar_visible () const {
      return scrollbar.is_visible();
    }

    template<>
    bool listT<true>::is_scroll_bar_visible () const {
      return scrollbar.is_visible();
    }

    template<>
    core::size listT<false>::calc_item_size(core::size::type width) const {
      return { width, client_size().height() };
    }

    template<>
    core::size listT<true>::calc_item_size(core::size::type item_height) const {
      return{ client_size().width(), item_height };
    }

    template<>
    void listT<false>::adjust_scroll_bar () {
      core::size::type iw = get_item_width();
      scroll_bar::type w = (iw * item_count) - size().width();

      scrollbar.set_max(std::max(w, 0.0F));
      scrollbar.set_visible((w > 0.0F) && is_scroll_bar_enabled());
    }

    template<>
    void listT<true>::adjust_scroll_bar () {
      core::size::type ih = get_item_height();
      scroll_bar::type h = (ih * item_count) - size().height();

      scrollbar.set_max(std::max(h, 0.0F));
      scrollbar.set_visible((h > 0.0F) && is_scroll_bar_enabled());
    }

    template<>
    listT<false>::listT () {
      scrollbar.register_event_handler(win::scroll_event([&] (core::point::type) {
        redraw_later();
      }));
      register_event_handler(paint_event([&](const draw::graphics& g) {
        paint(g);
      }));
      register_event_handler(left_btn_up_event([&](const core::point& pt) {
        if (!moved) {
          const int new_selection = int((pt.x() + get_scroll_pos()) / get_item_width());
          set_selection(new_selection == get_selection() ? -1 : new_selection);
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
          redraw_later();
        }
        last_mouse_point = core::point::undefined;
      }));
      register_event_handler(wheel_x_event([&](const core::point::type dx, const core::point&){
        set_scroll_pos(get_scroll_pos() - get_item_width() * dx);
        moved = true;
      }));
      register_event_handler(mouse_move_event([&](unsigned int keys, const core::point& pt) {
        if (left_button_bit_mask::is_set(keys)) {
          if (last_mouse_point != core::point::undefined) {
            core::point::type dx = last_mouse_point.x() - pt.x();
            set_scroll_pos(get_scroll_pos() + dx);
            moved = true;
          }
          last_mouse_point = pt;
        }
      }));
      register_event_handler(size_event([&](const core::size&){
        scrollbar.place(get_scroll_bar_area());
        adjust_scroll_bar();
      }));
      register_event_handler(key_up_event([&](os::key_state, os::key_symbol key){
        switch (key) {
          case keys::left:
          case keys::numpad::left:
            set_selection(get_selection() - 1);
            break;
          case keys::right:
          case keys::numpad::right:
            set_selection(get_selection() + 1);
            break;
          case keys::page_up:
          case keys::numpad::page_up:
            set_selection(get_selection() - static_cast<int>(size().width() / get_item_width()));
            break;
          case keys::page_down:
          case keys::numpad::page_down:
            set_selection(get_selection() + static_cast<int>(size().width() / get_item_width()));
            break;
          case keys::home:
          case keys::numpad::home:
            set_selection(0);
            break;
          case keys::end:
          case keys::numpad::end:
            set_selection((int)get_count() - 1);
            break;
        }
      }));
    }

    // --------------------------------------------------------------------------
    template<>
    listT<true>::listT () {
      scrollbar.register_event_handler(win::scroll_event([&] (core::point::type) {
        redraw_later();
      }));
      register_event_handler(paint_event([&](const draw::graphics& g) {
        paint(g);
      }));
      register_event_handler(left_btn_up_event([&](const core::point& pt) {
        if (!moved) {
          const int new_selection = int((pt.y() + get_scroll_pos()) / get_item_height());
          set_selection(new_selection == get_selection() ? -1 : new_selection);
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
          redraw_later();
        }
        last_mouse_point = core::point::undefined;
      }));
      register_event_handler(wheel_y_event([&](const core::point::type dy, const core::point&){
        set_scroll_pos(get_scroll_pos() - get_item_height() * dy);
        moved = true;
      }));
      register_event_handler(mouse_move_event([&](unsigned int keys, const core::point& pt) {
        if (left_button_bit_mask::is_set(keys)) {
          if (last_mouse_point != core::point::undefined) {
              core::point_type dy = last_mouse_point.y() - pt.y();
              set_scroll_pos(get_scroll_pos() + dy);
              moved = true;
          }
          last_mouse_point = pt;
        }
      }));
      register_event_handler(size_event([&](const core::size&){
        scrollbar.place(get_scroll_bar_area());
        adjust_scroll_bar();
      }));
      register_event_handler(key_up_event([&](os::key_state, os::key_symbol key){
        switch (key) {
          case keys::up:
          case keys::numpad::up:
            set_selection(get_selection() - 1);
            break;
          case keys::down:
          case keys::numpad::down:
            set_selection(get_selection() + 1);
            break;
          case keys::page_up:
          case keys::numpad::page_up:
            set_selection(get_selection() - static_cast<int>(size().height() / get_item_height()));
            break;
          case keys::page_down:
          case keys::numpad::page_down:
            set_selection(get_selection() + static_cast<int>(size().height() / get_item_height()));
            break;
          case keys::home:
          case keys::numpad::home:
            set_selection(0);
            break;
          case keys::end:
          case keys::numpad::end:
            set_selection((int)get_count() - 1);
            break;
        }
      }));
    }

  } // win

} // gui

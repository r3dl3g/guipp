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

    namespace detail {

      // --------------------------------------------------------------------------
      list::list ()
        : item_count(0)
        , selection(-1)
        , hilite(-1)
        , moved(false)
        , scroll_bar_enabled(true)
        , last_mouse_point(core::point::undefined)
      {
#ifdef X11
        detail::init_control_messages();
#endif // X11
        register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
          last_mouse_point = pt;
          moved = false;
        }));
      }

      void list::set_drawer (const std::function<draw_list_item>& drawer) {
        this->drawer = drawer;
      }

      void list::set_drawer (std::function<draw_list_item>&& drawer) {
        this->drawer = std::move(drawer);
      }

      void list::draw_item (std::size_t idx,
                            const draw::graphics& g,
                            const core::rectangle& place,
                            const draw::brush& background,
                            bool selected,
                            bool hilited) const {
        if (drawer) {
          drawer(idx, g, place, background, selected, hilited);
        }
      }

      // --------------------------------------------------------------------------
      template<>
      list::pos_t list_t<orientation::horizontal>::get_dimension (const core::point& pt) const {
        return pt.x();
      }

      template<>
      void list_t<orientation::horizontal>::set_dimension (core::rectangle& r, list::pos_t v, list::pos_t s) const {
        r.x(v);
        r.width(s);
      }

      template<>
      list::pos_t list_t<orientation::vertical>::get_dimension (const core::point& pt) const {
        return pt.y();
      }

      template<>
      void list_t<orientation::vertical>::set_dimension (core::rectangle& r, list::pos_t v, list::pos_t s) const {
        r.y(v);
        r.height(s);
      }

      template<>
      list::pos_t list_t<orientation::horizontal>::get_list_size () const {
        return size().width();
      }

      template<>
      list::pos_t list_t<orientation::vertical>::get_list_size () const {
        return size().height();
      }

      template<>
      core::size list_t<orientation::horizontal>::client_size () const {
        core::size sz = super::client_size();
        if (is_scroll_bar_visible()) {
          sz.height(sz.height() - scroll_bar::get_scroll_bar_width());
        }
        return sz;
      }

      template<>
      core::size list_t<orientation::vertical>::client_size() const {
        core::size sz = super::client_size();
        if (is_scroll_bar_visible()) {
          sz.width(sz.width() - scroll_bar::get_scroll_bar_width());
        }
        return sz;
      }

      template<>
      core::rectangle list_t<orientation::horizontal>::get_scroll_bar_area () const {
        core::rectangle r(super::client_size());
        r.y(r.y2() - scroll_bar::get_scroll_bar_width());
        r.height(scroll_bar::get_scroll_bar_width());
        return r;
      }

      template<>
      core::rectangle list_t<orientation::vertical>::get_scroll_bar_area () const {
        core::rectangle r(super::client_size());
        r.x(r.x2() - pos_t(scroll_bar::get_scroll_bar_width()));
        r.width(scroll_bar::get_scroll_bar_width());
        return r;
      }

    }

  } // win

} // gui

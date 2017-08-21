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

      list::data::data ()
        : item_count(0)
        , selection(-1)
        , hilite(-1)
        , moved(false)
        , scroll_bar_enabled(true)
        , last_mouse_point(core::point::undefined)
      {}

      // --------------------------------------------------------------------------
      list::list () {
        init();
      }

      list::list (const list& rhs)
        : super(rhs)
        , data(rhs.data)
      {
        init();
      }

      list::list (list&& rhs)
        : super(std::move(rhs))
        , data(std::move(rhs.data))
      {
        init();
      }

      void list::init () {
        set_accept_focus(true);
#ifdef X11
        detail::init_control_messages();
#endif // X11
        register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state, const core::point& pt) {
          data.last_mouse_point = pt;
          data.moved = false;
          take_focus();
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
        r.height(static_cast<core::size_type>(scroll_bar::get_scroll_bar_width()));
        return r;
      }

      template<>
      core::rectangle list_t<orientation::vertical>::get_scroll_bar_area () const {
        core::rectangle r(super::client_size());
        r.x(r.x2() - pos_t(scroll_bar::get_scroll_bar_width()));
        r.width(static_cast<core::size_type>(scroll_bar::get_scroll_bar_width()));
        return r;
      }

    }

    // --------------------------------------------------------------------------
    void edit_list::init() {
      super::register_event_handler(REGISTER_FUNCTION, win::selection_commit_event(this, &edit_list::enter_edit));

      data.editor.register_event_handler(REGISTER_FUNCTION,
                                         win::btn_down_event([&](os::key_state, const core::point& pt) {
        if(!data.editor.client_area().is_inside(pt)) {
          commit_edit();
        }
      }));

      data.editor.register_event_handler(REGISTER_FUNCTION, win::selection_cancel_event(this, &edit_list::cancel_edit));
      data.editor.register_event_handler(REGISTER_FUNCTION, win::selection_commit_event(this, &edit_list::commit_edit));

      super::scrollbar.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (core::point::type) {
        if (data.editor.is_visible()) {
          data.editor.place(super::get_place_of_index(super::get_selection()));
        }
      }));

      super::register_event_handler(REGISTER_FUNCTION, win::selection_changed_event(this, &edit_list::commit_edit));

    }

    void edit_list::set_enable_edit (bool enable) {
      data.enable_edit = enable;
    }

    bool edit_list::is_edit_enabled () const {
      return data.enable_edit;
    }

    void edit_list::enter_edit () {
      if (data.enable_edit && data.data_source) {
        auto cell = super::get_selection();
        auto area = super::get_place_of_index(cell);
        if (!data.editor.is_valid()) {
          data.editor.create(*reinterpret_cast<container*>(this), area);
        }
        data.editor.place(area);
        data.editor.set_text(data.data_source(cell));
        data.editor.set_visible();
        data.editor.take_focus();
      }
    }

    void edit_list::commit_edit () {
      if (data.data_target && data.editor.is_visible()) {
        auto pos = data.editor.position();
        auto cell = super::get_index_at_point(pos);
        data.data_target(cell, data.editor.get_text());
      }
      cancel_edit();
    }

    void edit_list::cancel_edit () {
      data.editor.set_visible(false);
      super::take_focus();
    }

    void edit_list::set_data_source_and_target (const std::function<source>& data_source,
                                                const std::function<target>& data_target) {
      data.data_source = data_source;
      data.data_target = data_target;
    }

  } // win

} // gui

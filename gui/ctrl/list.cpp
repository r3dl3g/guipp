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
// Library includes
//
#include <gui/ctrl/list.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      list_base::data::data (os::color background)
        : selection(-1)
        , hilite(-1)
        , last_mouse_point(core::point::undefined)
        , background(background)
      {}

      // --------------------------------------------------------------------------
      list_base::list_base (os::color background, bool grab_focus)
        : data(background)
      {
        get_state().set_grab_focus(grab_focus);
        init();
      }

      list_base::list_base (list_base&& rhs)
        : super(std::move(rhs))
        , data(std::move(rhs.data))
      {
        init();
      }

      void list_base::init () {
        list_state state = get_state();
        state.set_moved(false);
        state.set_scroll_bar_enabled(true);
        bool grab_focus = state.is_grab_focus();
#ifdef X11
        static int initialized = detail::init_control_messages();
        (void)initialized;
#endif // X11

        set_accept_focus(true);
        on_left_btn_down([&, grab_focus] (os::key_state, const core::point & pt) {
          data.last_mouse_point = pt;
          get_state().set_moved(false);
          if (grab_focus) {
            take_focus();
          }
        });
        on_set_focus([&] (win::window*) {
          invalidate();
        });
        on_lost_focus([&] (win::window*) {
          invalidate();
        });
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<>
    void linear_list<orientation_t::horizontal>::handle_direction_key (os::key_symbol key) {
      switch (key) {
      case win::keys::left:
      case win::keys::numpad::left:
        set_selection(super::get_selection() - 1, event_source::keyboard);
        break;
      case win::keys::right:
      case win::keys::numpad::right:
        set_selection(super::get_selection() + 1, event_source::keyboard);
        break;
      }
    }

    template<>
    void linear_list<orientation_t::vertical>::handle_direction_key (os::key_symbol key) {
      switch (key) {
      case win::keys::up:
      case win::keys::numpad::up:
        set_selection(super::get_selection() - 1, event_source::keyboard);
        break;
      case win::keys::down:
      case win::keys::numpad::down:
        set_selection(super::get_selection() + 1, event_source::keyboard);
        break;
      }
    }

    // --------------------------------------------------------------------------
    void edit_list::init () {
      super::on_selection_commit(util::bind_method(this, &edit_list::enter_edit));

      data.editor.on_btn_down([&](os::key_state, const core::point & pt) {
        if (!data.editor.client_area().is_inside(pt)) {
          commit_edit();
        }
      });

      data.editor.on_selection_cancel(util::bind_method(this, &edit_list::cancel_edit));
      data.editor.on_selection_commit(util::bind_method(this, &edit_list::commit_edit));

      super::scrollbar.on_scroll([&] (core::point::type) {
        if (data.editor.is_visible()) {
          data.editor.place(super::get_place_of_index(super::get_selection()));
        }
      });

      super::on_selection_changed([&](event_source) {
        commit_edit();
      });

    }

    void edit_list::set_enable_edit (bool enable) {
      data.enable_edit = enable;
    }

    bool edit_list::is_edit_enabled () const {
      return data.enable_edit;
    }

    void edit_list::enter_edit () {
      if (data.enable_edit&& data.data_source) {
        auto cell = super::get_selection();
        auto area = super::get_place_of_index(cell);
        if (!data.editor.is_valid()) {
          data.editor.create(*reinterpret_cast<win::container*>(this), area);
        }
        data.editor.place(area);
        data.editor.set_text(data.data_source(cell));
        data.editor.set_cursor_pos(data.editor.get_text_length());
        data.editor.set_visible();
        data.editor.take_focus();
      }
    }

    void edit_list::commit_edit () {
      if (data.data_target&& data.editor.is_visible()) {
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

  } // ctrl

} // gui

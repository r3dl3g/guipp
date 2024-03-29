/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     list control
 *
 * @license   MIT license. See accompanying file LICENSE.
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
      list_base::data::data ()
        : hilite(-1)
        , last_mouse_point(core::native_point::undefined)
      {}

      // --------------------------------------------------------------------------
      list_base::list_base (os::color background, bool grab_focus) {
        set_background(background);
        set_state().grab_focus(grab_focus);
        init();
      }

      list_base::list_base (list_base&& rhs) noexcept
        : super(std::move(rhs))
        , data(std::move(rhs.data))
      {
        init();
      }

      void list_base::init () {
        auto state = set_state();
        state.moved(false);
        state.scroll_bar_enabled(true);

#ifdef GUIPP_X11
        static int initialized = detail::init_control_messages();
        (void)initialized;
#endif // GUIPP_X11

        on_left_btn_down([&] (os::key_state, const core::native_point& pt) {
          data.last_mouse_point = pt;
          set_state().moved(false);
          if (get_state().grab_focus()) {
            take_focus();
          }
        });
        on_set_focus([&] () {
          invalidate();
        });
        on_lost_focus([&] () {
          invalidate();
        });
      }

      void list_base::set_scroll_pos (const core::point& pos) {
        if (data.offset != pos) {
          data.offset = pos;
          notify_selection_changed(event_source::logic);
          invalidate();
        }
      }

      void list_base::set_hilite (int sel, bool notify) {
        int new_hilite = std::max(-1, sel);
        if (new_hilite >= static_cast<int>(get_count())) {
          new_hilite = -1;
        }
        if (get_hilite() != new_hilite) {
          data.hilite = new_hilite;
          if (notify) {
            notify_hilite_changed(new_hilite != -1);
            invalidate();
          }
        }
      }

      void list_base::clear_hilite (bool notify) {
        if (get_hilite() != -1) {
          data.hilite = -1;
          if (notify) {
            notify_hilite_changed(false);
            invalidate();
          }
        }
      }

      void list_base::notify_scroll (pos_t pos) {
        notify_event_float(detail::SCROLLBAR_MESSAGE, pos);
      }

      void list_base::on_scroll (std::function<void(core::point::type)>&& f) {
        on<scroll_event>(std::move(f));
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<>
    int linear_list_traits<orientation_t::horizontal>::get_step (os::key_symbol key) {
      switch (key) {
      case core::keys::left:
      case core::keys::numpad::left:
        return -1;
        break;
      case core::keys::right:
      case core::keys::numpad::right:
        return 1;
        break;
      }
      return 0;
    }

    template<>
    int linear_list_traits<orientation_t::vertical>::get_step (os::key_symbol key) {
      switch (key) {
      case core::keys::up:
      case core::keys::numpad::up:
        return -1;
        break;
      case core::keys::down:
      case core::keys::numpad::down:
        return 1;
        break;
      }
      return 0;
    }

    // --------------------------------------------------------------------------
    void edit_list::init () {
      super::on_selection_commit(util::bind_method(this, &edit_list::enter_edit));

      data.editor.on_btn_down([&](os::key_state, const core::native_point& pt) {
        if (!data.editor.surface_geometry().is_inside(pt)) {
          commit_edit();
        }
      });

      data.editor.on_selection_cancel(util::bind_method(this, &edit_list::cancel_edit));
      data.editor.on_selection_commit(util::bind_method(this, &edit_list::commit_edit));

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
      if (data.enable_edit && data.data_source) {
        auto cell = get_selection().get_first_index();
        auto area = get_geometry_of_index(cell);
        area.set_position(get_parent()->surface_to_client(client_to_surface(area.position())));
        if (!data.editor.is_valid()) {
          data.editor.create(*get_parent(), area);
        }
        data.editor.geometry(area, true, false);
        data.editor.init_text(data.data_source(cell));
        data.editor.set_cursor_pos(data.editor.get_text_length());
        data.editor.set_visible();
        data.editor.take_focus();
      }
    }

    void edit_list::commit_edit () {
      if (data.data_target && data.editor.is_visible()) {
        auto pos = surface_to_client(data.editor.surface_position());
        auto cell = get_index_at_point(pos);
        if (cell > -1) {
          data.data_target(cell, data.editor.get_text());
        }
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

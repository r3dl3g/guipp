/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
        , last_mouse_point(core::native_point::undefined)
        , background(background)
      {}

      // --------------------------------------------------------------------------
      list_base::list_base (os::color background, bool grab_focus)
        : data(background)
      {
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

        set_accept_focus(true);
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
          invalidate();
        }
      }

      void list_base::on_selection_changed (selection_changed_event::function&& f) {
        on<selection_changed_event>(std::move(f));
      }

      void list_base::on_selection_commit (selection_commit_event::function&& f) {
        on<selection_commit_event>(std::move(f));
      }

      void list_base::on_hilite_changed (hilite_changed_event::function&& f) {
        on<hilite_changed_event>(std::move(f));
      }

      void list_base::on_content_changed (content_changed_event::function&& f) {
        on<content_changed_event>(std::move(f));
      }

      void list_base::notify_selection_changed (event_source src) {
        notify_event(detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(src));
      }

      void list_base::notify_selection_commit () {
        notify_event(detail::SELECTION_COMMIT_MESSAGE);
      }

      void list_base::notify_hilite_changed (bool b) {
        notify_event(detail::HILITE_CHANGE_MESSAGE, b);
      }

      void list_base::notify_content_changed () {
        notify_event(detail::CONTENT_CHANGED_MESSAGE);
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<>
    void linear_list<orientation_t::horizontal>::handle_direction_key (os::key_symbol key) {
      switch (key) {
      case core::keys::left:
      case core::keys::numpad::left:
        set_selection(super::get_selection() - 1, event_source::keyboard);
        break;
      case core::keys::right:
      case core::keys::numpad::right:
        set_selection(super::get_selection() + 1, event_source::keyboard);
        break;
      }
    }

    template<>
    void linear_list<orientation_t::vertical>::handle_direction_key (os::key_symbol key) {
      switch (key) {
      case core::keys::up:
      case core::keys::numpad::up:
        set_selection(super::get_selection() - 1, event_source::keyboard);
        break;
      case core::keys::down:
      case core::keys::numpad::down:
        set_selection(super::get_selection() + 1, event_source::keyboard);
        break;
      }
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
        auto cell = super::get_selection();
        auto area = super::get_geometry_of_index(cell);
        if (!data.editor.is_valid()) {
          data.editor.create(*this, area);
        }
        data.editor.geometry(area);
        data.editor.set_text(data.data_source(cell));
        data.editor.set_cursor_pos(data.editor.get_text_length());
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

  } // ctrl

} // gui

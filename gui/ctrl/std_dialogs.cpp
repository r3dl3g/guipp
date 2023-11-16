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
 * @brief     standard dialogs
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/std_dialogs.h>

namespace gui {

  namespace ctrl {

    namespace detail {

      inline int number_of_lines (const std::string& str) {
        return std::count(str.begin(), str.end(), '\n') + 1;
      }

      // --------------------------------------------------------------------------
      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_multi_input_dialog_size<core::os::ui_t::desktop> (const core::rectangle&, std::size_t n, unsigned dimension) {
        return core::rectangle(300, 200, 400, static_cast<core::size::type>(80 + n * dimension));
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_path_open_dialog_size<core::os::ui_t::desktop> (const core::rectangle&) {
        return win::window::screen_area() / 3 * 2;
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_message_dialog_size<core::os::ui_t::desktop> (const core::rectangle&, const std::string& message, unsigned dimension) {
        return core::rectangle(300, 200, 400, static_cast<core::size::type>(80 + std::max(number_of_lines(message), 5) * dimension));
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_yes_no_dialog_size<core::os::ui_t::desktop> (const core::rectangle& area, const std::string& message, unsigned dimension) {
        return std_message_dialog_size<core::os::ui_t::desktop>(area, message, dimension);
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_input_dialog_size<core::os::ui_t::desktop> (const core::rectangle&) {
        return core::rectangle(300, 200, 400, 125);
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_file_save_dialog_size<core::os::ui_t::desktop> (const core::rectangle& area) {
        return std_path_open_dialog_size<core::os::ui_t::desktop>(area);
      }

      //-----------------------------------------------------------------------------
      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_multi_input_dialog_size<core::os::ui_t::mobile> (const core::rectangle& /*area*/, std::size_t n, unsigned dimension) {
        return win::window::screen_area().shrinked({ 20, 20 }).with_height(static_cast<core::size::type>(80 + n * dimension));
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_path_open_dialog_size<core::os::ui_t::mobile> (const core::rectangle&) {
        return win::window::screen_area();
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_message_dialog_size<core::os::ui_t::mobile> (const core::rectangle&, const std::string& message, unsigned dimension) {
        return win::window::screen_area().shrinked({ 20, 20 }).with_height(static_cast<core::size::type>(80 + std::max(number_of_lines(message), 5) * dimension));
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_yes_no_dialog_size<core::os::ui_t::mobile> (const core::rectangle& area, const std::string& message, unsigned dimension) {
        return std_message_dialog_size<core::os::ui_t::desktop>(area, message, dimension);
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_input_dialog_size<core::os::ui_t::mobile> (const core::rectangle&) {
        return win::window::screen_area().shrinked({ 30, 100 });
      }

      template<>
      GUIPP_CTRL_EXPORT core::rectangle std_file_save_dialog_size<core::os::ui_t::mobile> (const core::rectangle& area) {
        return std_path_open_dialog_size<core::os::ui_t::mobile>(area);
      }

      //-----------------------------------------------------------------------------
    } // namespace detail

    //-----------------------------------------------------------------------------
    yes_no_dialog::yes_no_dialog () {
      content_view.get_layout().set_center(layout::lay(message_view));
    }

    void yes_no_dialog::create (win::overlapped_window& parent,
                                const std::string& title,
                                const std::string& message,
                                const std::string& yes_label,
                                const std::string& no_label,
                                const core::rectangle& rect,
                                std::function<yes_no_action> action) {
      super::create(parent, title, yes_label, no_label, rect, action);
      message_view.create(super::content_view, message);
    }

    void yes_no_dialog::show (win::overlapped_window& parent) {
      super::run_modal(parent, {
        win::hot_key_action{
          core::hot_key(core::keys::escape, core::state::none),
          [&] () {
            action(*this, false);
            super::end_modal();
          }
        },
        win::hot_key_action{
          core::hot_key(core::keys::right, core::state::none),
          [&] () { super::shift_focus(false); }
        },
        win::hot_key_action{
          core::hot_key(core::keys::left, core::state::none),
          [&] () { super::shift_focus(true); }
        }
      });
    }

    void yes_no_dialog::ask (win::overlapped_window& parent,
                             const std::string& title,
                             const std::string& message,
                             const std::string& yes_label,
                             const std::string& no_label,
                             std::function<yes_no_action> action) {
      yes_no_dialog dialog;
      dialog.create(parent, title, message, yes_label, no_label,
                    detail::std_yes_no_dialog_size<>(parent.geometry(), message),
                    action);
      dialog.show(parent);
    }

    //-----------------------------------------------------------------------------
    message_dialog::message_dialog () {
      content_view.get_layout().set_center(layout::lay(message_view));
    }

    void message_dialog::create (win::overlapped_window& parent,
                                const std::string& title,
                                const std::string& message,
                                const std::string& ok_label,
                                const core::rectangle& rect) {
      super::create(parent, title, rect, {ok_label},
                    [&] (win::overlapped_window&, int) {
        end_modal();
      });
      message_view.create(content_view, message);
    }

    void message_dialog::show (win::overlapped_window& parent,
                               const std::string& title,
                               const std::string& message,
                               const std::string& ok_label) {
      message_dialog dialog;
      dialog.create(parent, title, message, ok_label,
                    detail::std_message_dialog_size<>(parent.geometry(), message));
      dialog.super::show(parent);
    }

    //-----------------------------------------------------------------------------
    input_dialog::input_dialog () {
      super::content_view.get_layout().add(layout::lay(label));
      super::content_view.get_layout().add(layout::lay(edit));
    }

    void input_dialog::create (win::overlapped_window& parent,
                               const std::string& title,
                               const std::string& message,
                               const std::string& initial,
                               const std::string& ok_label,
                               const std::string& cancel_label,
                               const core::rectangle& rect,
                               std::function<input_action> action) {
      super::create(parent, title, rect, {cancel_label, ok_label},
                    [&, action] (win::overlapped_window& dlg, int i) {
        if (i == 1) {
          action(dlg, edit.get_text());
        }
      });
      label.create(super::content_view, message);
      edit.create(super::content_view, initial);
    }

    void input_dialog::ask (win::overlapped_window& parent,
                            const std::string& title,
                            const std::string& message,
                            const std::string& initial,
                            const std::string& ok_label,
                            const std::string& cancel_label,
                            std::function<input_action> action) {
      input_dialog dialog;
      dialog.create(parent, title, message, initial, ok_label, cancel_label,
                    detail::std_input_dialog_size<>(parent.geometry()),
                    action);
      dialog.show(parent);
    }

    //-----------------------------------------------------------------------------
    std::function<create_subdirectory>
    make_create_subdirectory_fn (const std::string& title,
                                 const std::string& message,
                                 const std::string& initial,
                                 const std::string& ok_label,
                                 const std::string& cancel_label) {
      return [=] (win::overlapped_window& parent, const sys_fs::path& parent_dir) -> bool {
        bool return_value = false;
        input_dialog::ask(parent, title, message, initial, ok_label, cancel_label,
                          [&] (win::overlapped_window&, const std::string& t) {
          return_value = sys_fs::create_directory(parent_dir / t);
        });
        return return_value;
      };
    }

    std::function<create_subdirectory> make_default_create_subdirectory_fn () {
      return make_create_subdirectory_fn("Create directory", "Create new sub-directory",
                                         "", "Create", "Cancel");
    }

    //-----------------------------------------------------------------------------
    dir_tree_view::dir_tree_view (create_subdirectory_fn fn)
      : fn(fn)
    {
      init();
    }

    dir_tree_view::dir_tree_view (dir_tree_view&& rhs)
      : super(std::move(rhs))
      , header(std::move(rhs.header))
      , view(std::move(rhs.view))
      , add_button(std::move(rhs.add_button))
      , fn(std::move(rhs.fn))
    {}

    void dir_tree_view::init () {
      get_layout().set_header_and_body(layout::lay(header), layout::lay(view));
      header.set_background(color::very_very_light_gray);
      header.on_paint(draw::paint([&] (draw::graphics& graph) {
        core::rectangle r = client_geometry();
        graph.fill(draw::rectangle(r), get_background());
        draw::frame::raised_relief(graph, r);
      }));
      if (fn) {
        add_button.set_foreground(color::black);
        header.get_layout().add(layout::lay(add_button));
        add_button.on_clicked([&] () {
          if (view->has_selection()) {
            auto parent_dir = view->get_item(view->get_selection().get_first_index()).path;
            if (fn(super::get_overlapped_window(), parent_dir)) {
              view->update_node_list();
            }
          }
        });
      }
    }

    void dir_tree_view::set_create_subdirectory_fn (create_subdirectory_fn fn_) {
      fn = fn_;
    }

    void dir_tree_view::create (win::container& parent, const core::rectangle& r) {
      init();
      super::create(clazz::get(), parent, r);
      header.create(*this);
      view.create(*this);
      add_button.create(header);
    }

    //-----------------------------------------------------------------------------
    file_save_dialog::file_save_dialog (create_subdirectory_fn fn)
      : super(dir_file_view(fn))
    {}

    void file_save_dialog::create (win::overlapped_window& parent,
                                   const std::string& title,
                                   const std::string& default_name,
                                   const std::string& name_label,
                                   const std::string& ok_label,
                                   const std::string& cancel_label,
                                   const core::rectangle& rect,
                                   std::function<file_selected> action) {

      auto& dir_tree = content_view.get<0>().view.view;
      auto& files = content_view.get<1>();

      content_view.init([&, action] (win::overlapped_window& dlg, const sys_fs::path& path) {
        set_visible(false);
        end_modal();
        action(dlg, path);
      });

      files.list->on_selection_changed([&] (event_source) {
        input_line.init_text(files.get_selected_path().filename().string());
      });

      top_view.get_layout().set_center(layout::lay(input_line));
      top_view.get_layout().set_left(layout::lay(input_label));
      get_layout().set_top(layout::lay(top_view));

      super::create(parent, title, rect, {cancel_label, ok_label},
                    [&, action] (win::overlapped_window& dlg, int btn) {
        if (1 == btn) {
          if (dir_tree.has_selection()) {
            int idx = dir_tree.get_selection().get_first_index();
            sys_fs::path path = dir_tree.get_item(idx).path;
            path /= input_line.get_text();
            action(dlg, path);
          }
        }
      });

      top_view.create(*this, core::rectangle(0, 0, 100, 100));
      input_label.create(top_view, name_label, core::rectangle(0, 0, 100, 100));
      input_line.create(top_view, default_name, core::rectangle(0, 0, 100, 100));
      content_view.set_split_pos(0.3);

      top_view.set_background(color::very_very_light_gray);

      sys_fs::path current = sys_fs::current_path();
      dir_tree.set_roots(fs::get_all_root_file_infos());
      for (auto next = current; next.has_root_path() && (next != next.root_path()); next = next.parent_path()) {
        dir_tree.add_open_node(next);
      }
      dir_tree.add_open_node(current.root_path());
      dir_tree.update_node_list();
      dir_tree.select_node(current);
      files.set_path(current);
    }

    void file_save_dialog::show (win::overlapped_window& parent,
                                 const std::string& title,
                                 const std::string& default_name,
                                 const std::string& name_label,
                                 const std::string& ok_label,
                                 const std::string& cancel_label,
                                 std::function<file_selected> action,
                                 create_subdirectory_fn fn) {
      file_save_dialog dialog(fn);
      dialog.create(parent, title, default_name, name_label, ok_label, cancel_label,
                    detail::std_file_save_dialog_size<>(parent.geometry()),
                    action);
      dialog.super::show(parent);
    }

    //-----------------------------------------------------------------------------

  } // ctrl

} // gui

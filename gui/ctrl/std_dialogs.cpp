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
 * @brief     C++ API: standard dialogs
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/std_dialogs.h>

namespace gui {

  namespace ctrl {

    //-----------------------------------------------------------------------------
    yes_no_dialog::yes_no_dialog () {
      content_view.get_layout().set_center(layout::lay(message_view));
    }

    void yes_no_dialog::create (win::container& parent,
                                const std::string& title,
                                const std::string& message,
                                const std::string& yes_label,
                                const std::string& no_label,
                                const core::rectangle& rect,
                                std::function<yes_no_action> action) {
      super::create(parent, title, rect, [action] (int i) {
        action(i == 1);
      }, {no_label, yes_label});
      message_view.create(content_view, message, rect);
    }

    void yes_no_dialog::ask (win::container& parent,
                             const std::string& title,
                             const std::string& message,
                             const std::string& yes_label,
                             const std::string& no_label,
                             std::function<yes_no_action> action) {
      yes_no_dialog dialog;
      dialog.create(parent, title, message, yes_label, no_label, core::rectangle(300, 200, 400, 170), action);
      dialog.show(parent, action);
    }

    void yes_no_dialog::show (win::container& parent,
                              std::function<yes_no_action> action) {
      run_modal(parent, {
        win::hot_key_action{
          win::hot_key(win::keys::escape, win::state::none),
          [&] () { action(false); }
        },
        win::hot_key_action{
          win::hot_key(win::keys::right, win::state::none),
          [&] () { forward_focus(false); }
        },
        win::hot_key_action{
          win::hot_key(win::keys::left, win::state::none),
          [&] () { forward_focus(true); }
        }
      });
    }

    //-----------------------------------------------------------------------------
    message_dialog::message_dialog () {
      content_view.get_layout().set_center(layout::lay(message_view));
    }

    void message_dialog::create (win::container& parent,
                                const std::string& title,
                                const std::string& message,
                                const std::string& ok_label,
                                const core::rectangle& rect) {
      super::create(parent, title, rect, [&] (int) {
        end_modal();
      }, {ok_label});
      message_view.create(content_view, message, rect);
    }

    void message_dialog::show (win::container& parent,
                               const std::string& title,
                               const std::string& message,
                               const std::string& ok_label) {
      message_dialog dialog;
      dialog.create(parent, title, message, ok_label, core::rectangle(300, 200, 400, 170));
      dialog.super::show(parent);
    }

    //-----------------------------------------------------------------------------
    input_dialog::input_dialog () {
      super::content_view.get_layout().add(layout::lay(label));
      super::content_view.get_layout().add(layout::lay(edit));
    }

    void input_dialog::create (win::container& parent,
                               const std::string& title,
                               const std::string& message,
                               const std::string& initial,
                               const std::string& ok_label,
                               const std::string& cancel_label,
                               const core::rectangle& rect,
                               std::function<input_action> action) {
      super::create(parent, title, rect, [&, action] (int i) {
        if (i == 1) {
          action(edit.get_text());
        }
      }, {cancel_label, ok_label});
      label.create(super::content_view, message);
      edit.create(super::content_view, initial);
    }

    void input_dialog::ask (win::container& parent,
                            const std::string& title,
                            const std::string& message,
                            const std::string& initial,
                            const std::string& ok_label,
                            const std::string& cancel_label,
                            std::function<input_action> action) {
      input_dialog dialog;
      dialog.create(parent, title, message, initial, ok_label, cancel_label, core::rectangle(300, 200, 400, 125), action);
      dialog.show(parent);
    }

    //-----------------------------------------------------------------------------
    file_save_dialog::file_save_dialog ()
    {}

    void file_save_dialog::create (win::container& parent,
                                   const std::string& title,
                                   const std::string& default_name,
                                   const std::string& name_label,
                                   const std::string& ok_label,
                                   const std::string& cancel_label,
                                   const core::rectangle& rect,
                                   std::function<file_selected> action) {

      auto& dir_tree = content_view.first;
      auto& files = content_view.second;

      content_view.init([&, action] (const sys_fs::path& path) {
        set_visible(false);
        end_modal();
        action(path);
      });

      files.list.on_selection_changed([&] (event_source) {
        input_line.set_text(files.get_selected_path().filename().string());
      });

      top_view.get_layout().set_center(layout::lay(input_line));
      top_view.get_layout().set_left(layout::lay(input_label));
      get_layout().set_top(layout::lay(top_view));

      super::create(parent, title, rect, [&, action] (int btn) {
        if (1 == btn) {
          int idx = dir_tree.get_selection();
          if (idx > -1) {
            sys_fs::path path = dir_tree.get_item(idx).path;
            path /= input_line.get_text();
            action(path);
          }
        }
      }, {cancel_label, ok_label});

      top_view.create(*this, core::rectangle(0, 0, 100, 100));
      input_label.create(top_view, name_label, core::rectangle(0, 0, 100, 100));
      input_line.create(top_view, default_name, core::rectangle(0, 0, 100, 100));
      content_view.set_split_pos(0.5);

      sys_fs::path current = sys_fs::current_path();
      dir_tree.set_roots(fs::get_all_root_file_infos());
      for (auto next = current; next.has_root_path() && (next != next.root_path()); next = next.parent_path()) {
        dir_tree.add_open_node(next);
      }
      dir_tree.add_open_node(current.root_path());
      dir_tree.update_node_list();
      dir_tree.select_node(current);
      files.set_path(current);

      top_view.set_children_visible();
    }

    void file_save_dialog::show (win::container& parent,
                                 const std::string& title,
                                 const std::string& default_name,
                                 const std::string& name_label,
                                 const std::string& ok_label,
                                 const std::string& cancel_label,
                                 std::function<file_selected> action) {
      file_save_dialog dialog;
      dialog.create(parent, title, default_name, name_label, ok_label, cancel_label, core::rectangle(200, 100, 800, 600), action);
      dialog.super::show(parent);
    }

    //-----------------------------------------------------------------------------

  } // ctrl

} // gui

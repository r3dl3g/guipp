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

  namespace win {

    //-----------------------------------------------------------------------------
    void standard_dialog_base::create (win::container& parent,
                                       const std::string& title,
                                       const std::string& yes_label,
                                       const std::string& no_label,
                                       const core::rectangle& rect,
                                       std::function<yes_no_action> action) {
      yes.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&, action] () {
        set_visible(false);
        end_modal();
        if (action) {
          action(true);
        }
      }));
      no.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&, action] () {
        set_visible(false);
        end_modal();
        if (action) {
          action(false);
        }
      }));
      register_event_handler(REGISTER_FUNCTION, set_focus_event([&] (window*) {
        yes.take_focus();
      }));

      get_layout().set_bottom(&buttons);

      super::create(parent, rect);
      set_title(title);
      buttons.create(*this);
      no.create(buttons, no_label);
      yes.create(buttons, yes_label);
    }

    void standard_dialog_base::show (win::container& parent) {
      set_children_visible();
      set_visible();
      parent.disable();
      run_modal({
                  hot_key_action{ hot_key(keys::escape, state::none), [&] () {
                                    end_modal();
                                  }}
                });
      parent.enable();
      parent.take_focus();
    }

    //-----------------------------------------------------------------------------
    yes_no_dialog::yes_no_dialog ()
      : super(content_view_type(20, 15, 15, 15))
    {
      content_view.get_layout().set_center(&message_view);
    }

    void yes_no_dialog::create (win::container& parent,
                                const std::string& title,
                                const std::string& message,
                                const std::string& yes_label,
                                const std::string& no_label,
                                const core::rectangle& rect,
                                std::function<yes_no_action> action) {
      super::create(parent, title, yes_label, no_label, rect, action);
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
      dialog.show(parent);
    }

    //-----------------------------------------------------------------------------
    file_save_dialog::file_save_dialog ()
      : super(25)
      , top_view(2, 2, 120, 2)
    {}

    void file_save_dialog::create (win::container& parent,
                                   const std::string& title,
                                   const std::string& default_name,
                                   const std::string& name_label,
                                   const std::string& ok_label,
                                   const std::string& cancel_label,
                                   std::function<file_selected> action) {

      auto& dir_tree = content_view.first;
      auto& files = content_view.second;

      content_view.init([&, action] (const sys_fs::path& path) {
        set_visible(false);
        end_modal();
        action(path);
      });

      files.list.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&] (event_source) {
        input_line.set_text(files.get_selected_path().filename().string());
      }));

      top_view.get_layout().set_center(&input_line);
      top_view.get_layout().set_left(&input_label);
      get_layout().set_top(&top_view);

      super::create(parent, title, ok_label, cancel_label,
                    core::rectangle(300, 200, 600, 400),
                    [&, action] (bool open) {
        if (open) {
          int idx = dir_tree.get_selection();
          if (idx > -1) {
            sys_fs::path path = dir_tree.get_item(idx);
            path /= input_line.get_text();
            action(path);
          }
        }
      });

      top_view.create(*this, core::rectangle(0, 0, 100, 100));
      input_label.create(top_view, name_label, core::rectangle(0, 0, 100, 100));
      input_line.create(top_view, default_name, core::rectangle(0, 0, 100, 100));
      content_view.set_split_pos(0.3);

      sys_fs::path current = sys_fs::current_path();
      dir_tree.root = current.root_path();
      for (auto next = current; next.has_root_path(); next = next.parent_path()) {
        dir_tree.add_open_node(next);
      }
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
      dialog.create(parent, title, default_name, name_label, ok_label, cancel_label, action);
      dialog.super::show(parent);
    }

    //-----------------------------------------------------------------------------

  } // win

} // gui
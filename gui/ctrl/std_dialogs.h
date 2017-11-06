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

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/container.h>
#include <gui/win/border_layout.h>
#include <gui/win/adaption_layout.h>
#include <gui/ctrl/control.h>
#include <gui/ctrl/file_tree.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/textbox.h>


namespace gui {

  namespace win {

      //-----------------------------------------------------------------------------
      typedef void (yes_no_action) (bool);

    //-----------------------------------------------------------------------------
    class standard_dialog_base : public layout_dialog_window<layout::border_layout<>, float, float, float, float> {
    public:
      typedef layout_dialog_window<layout::border_layout<>, float, float, float, float> super;
      typedef group_window<layout::horizontal_adaption<>, color::very_light_gray> button_view_type;

      standard_dialog_base (float top = 0)
        : super(top, 45, 0, 0)
      {}

      void create (win::container& parent,
                   const std::string& title,
                   const std::string& yes_label,
                   const std::string& no_label,
                   const core::rectangle& rect,
                   std::function<yes_no_action> action);

      void show (win::container& parent);

      button_view_type buttons;
      text_button yes, no;
    };

    //-----------------------------------------------------------------------------
    template<typename T>
    class standard_dialog : public standard_dialog_base {
    public:
      typedef standard_dialog_base super;
      typedef T content_view_type;

      standard_dialog (float top = 0)
        : super(top)
      {}

      standard_dialog (content_view_type&& view, float top = 0)
        : super(top)
        , content_view(std::move(view))
      {}

      void create (win::container& parent,
                   const std::string& title,
                   const std::string& yes_label,
                   const std::string& no_label,
                   const core::rectangle& rect,
                   std::function<yes_no_action> action) {
        get_layout().set_center(&content_view);
        super::create(parent, title, yes_label, no_label, rect, action);
        content_view.create(*this, rect);
      }

      content_view_type content_view;
    };

    //-----------------------------------------------------------------------------
    class yes_no_dialog : public standard_dialog<win::group_window<layout::border_layout<>,
                                                                   color::very_light_gray,
                                                                   float, float, float, float>> {
    public:
      typedef win::basic_textbox<text_origin::center, draw::frame::sunken_relief, color::black, color::very_light_gray> message_view_type;
      typedef win::group_window<layout::border_layout<>, color::very_light_gray, float, float, float, float> content_view_type;
      typedef standard_dialog<content_view_type> super;

      yes_no_dialog ();

      void create (win::container& parent,
                   const std::string& title,
                   const std::string& message,
                   const std::string& yes_label,
                   const std::string& no_label,
                   const core::rectangle& rect,
                   std::function<yes_no_action> action);

      static void ask (win::container& parent,
                       const std::string& title,
                       const std::string& message,
                       const std::string& yes_label,
                       const std::string& no_label,
                       std::function<yes_no_action> action);

      message_view_type message_view;

    };

    //-----------------------------------------------------------------------------
    typedef void (file_selected) (const sys_fs::path&);

    //-----------------------------------------------------------------------------
    template<typename T = path_tree::sorted_path_info>
    class dir_file_view : public win::vertical_split_view<win::sorted_dir_tree,
                                                          win::file_list<T>> {
    public:
      typedef win::sorted_dir_tree dir_tree_type;
      typedef win::file_list<T> file_list_type;
      typedef win::vertical_split_view<dir_tree_type, file_list_type> super;

      dir_file_view ()
      {}

      void init (std::function<file_selected> action) {
        super::first.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&](event_source) {
          int idx = super::first.get_selection();
          if (idx > -1) {
            super::second.set_path(super::first.get_item(idx));
          }
        }));
        super::second.list.register_event_handler(REGISTER_FUNCTION, win::selection_commit_event([&, action] () {
          auto path = super::second.get_selected_path();
          if (sys_fs::is_directory(path)) {
            super::first.open_node(path.parent_path());
            super::first.select_node(path);
            super::second.set_path(path);
          } else {
            action(path);
          }
        }));
      }

    };

    //-----------------------------------------------------------------------------
    template<typename T>
    class path_open_dialog_base : public standard_dialog<dir_file_view<T>> {
    public:
      typedef standard_dialog<dir_file_view<T>> super;

      void create (win::container& parent,
                   const std::string& title,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   std::function<file_selected> action) {
        auto& dir_tree = super::content_view.first;
        auto& file_list = super::content_view.second;

        super::content_view.init([&, action] (const sys_fs::path& path) {
          super::set_visible(false);
          super::end_modal();
          action(path);
        });

        super::create(parent, title, ok_label, cancel_label,
                      core::rectangle(300, 200, 600, 400),
                      [&] (bool open) {
          if (open) {
            if (super::content_view.second.list.get_selection() > -1) {
              action(super::content_view.second.get_selected_path());
            } else {
              int idx = super::content_view.first.get_selection();
              if (idx > -1) {
                action(super::content_view.first.get_item(idx));
              }
            }
          }
        });

        super::content_view.set_split_pos(0.3);

        sys_fs::path current = sys_fs::current_path();
        dir_tree.root = current.root_path();
        for (auto next = current; next.has_root_path(); next = next.parent_path()) {
          dir_tree.add_open_node(next);
        }
        dir_tree.update_node_list();
        dir_tree.select_node(current);
        file_list.set_path(current);
      }

      static void show (win::container& parent,
                        const std::string& title,
                        const std::string& ok_label,
                        const std::string& cancel_label,
                        std::function<file_selected> action) {
        path_open_dialog_base dialog;
        dialog.create(parent, title, ok_label, cancel_label, action);
        dialog.super::show(parent);
      }
    };

    //-----------------------------------------------------------------------------
    typedef path_open_dialog_base<path_tree::sorted_path_info> file_open_dialog;
    typedef path_open_dialog_base<path_tree::sorted_dir_info> dir_open_dialog;

    //-----------------------------------------------------------------------------
    class file_save_dialog : public standard_dialog<dir_file_view<>> {
    public:
      typedef standard_dialog<dir_file_view<>> super;
      typedef group_window<layout::border_layout<>, color::very_light_gray, float, float, float, float> top_view_type;

      file_save_dialog ();

      void create (win::container& parent,
                   const std::string& title,
                   const std::string& default_name,
                   const std::string& name_label,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   std::function<file_selected> action);

      static void show (win::container& parent,
                        const std::string& title,
                        const std::string& default_name,
                        const std::string& name_label,
                        const std::string& ok_label,
                        const std::string& cancel_label,
                        std::function<file_selected> action);

      top_view_type top_view;
      edit input_line;
      label_right input_label;

    };

  } // win

} // gui

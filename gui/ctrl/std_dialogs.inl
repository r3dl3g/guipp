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


namespace gui {

  namespace win {

    //-----------------------------------------------------------------------------
    inline standard_dialog_base::standard_dialog_base (float top)
      : super(top, 45, 0, 0)
    {}

    //-----------------------------------------------------------------------------
    template<typename T>
    inline standard_dialog<T>::standard_dialog (float top)
      : super(top)
    {}

    template<typename T>
    inline standard_dialog<T>::standard_dialog (content_view_type&& view, float top)
      : super(top)
      , content_view(std::move(view))
    {}

    template<typename T>
    void standard_dialog<T>::create (win::container& parent,
                                     const std::string& title,
                                     const std::string& yes_label,
                                     const std::string& no_label,
                                     const core::rectangle& rect,
                                     std::function<yes_no_action> action) {
      get_layout().set_center(&content_view);
      super::create(parent, title, yes_label, no_label, rect, action);
      content_view.create(*this, rect);
    }

    //-----------------------------------------------------------------------------
    template<typename T>
    inline dir_file_view<T>::dir_file_view ()
    {}

    template<typename T>
    void dir_file_view<T>::init (std::function<file_selected> action) {
      super::first.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&](event_source) {
        int idx = super::first.get_selection();
        if (idx > -1) {
          super::second.set_path(super::first.get_item(idx).path);
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

    //-----------------------------------------------------------------------------
    template<typename T>
    void path_open_dialog_base<T>::create (win::container& parent,
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
              action(super::content_view.first.get_item(idx).path);
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

    template<typename T>
    void path_open_dialog_base<T>::show (win::container& parent,
                                         const std::string& title,
                                         const std::string& ok_label,
                                         const std::string& cancel_label,
                                         std::function<file_selected> action) {
      path_open_dialog_base dialog;
      dialog.create(parent, title, ok_label, cancel_label, action);
      dialog.super::show(parent);
    }

  } // win

} // gui

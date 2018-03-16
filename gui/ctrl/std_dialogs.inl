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

  namespace ctrl {

    //-----------------------------------------------------------------------------
    inline standard_dialog_base::standard_dialog_base (float top)
      : super(top, 45, 0, 0)
    {}

    //-----------------------------------------------------------------------------
    template<typename T>
    inline standard_dialog<T>::standard_dialog (const content_view_type& view, float top)
      : super(top)
      , content_view(view)
    {}

    template<typename T>
    inline standard_dialog<T>::standard_dialog (content_view_type&& view, float top)
      : super(top)
      , content_view(std::move(view))
    {}

    template<typename T>
    void standard_dialog<T>::create (win::container& parent,
                                     const std::string& title,
                                     const core::rectangle& rect,
                                     std::function<dialog_action> action,
                                     const std::initializer_list<std::string>& labels) {
      get_layout().set_center(&content_view);
      super::create(parent, title, rect, action, labels);
      content_view.create(*this, rect);
    }

    //-----------------------------------------------------------------------------
    template<typename T>
    void dir_file_view<T>::init (std::function<file_selected> action) {
      super::first.on_selection_changed([&](event_source) {
        int idx = super::first.get_selection();
        if (idx > -1) {
          super::second.set_path(super::first.get_item(idx).path);
        }
      });
      super::second.list.on_selection_commit([&, action] () {
        auto path = super::second.get_selected_path();
        if (sys_fs::is_directory(path)) {
          super::first.open_node(path.parent_path());
          super::first.select_node(path);
          super::second.set_path(path);
        } else {
          action(path);
        }
      });
    }

    //-----------------------------------------------------------------------------
    template<typename T>
    path_open_dialog_base<T>::path_open_dialog_base ()
      : super(dir_file_view<T>())
    {}

    template<typename T>
    path_open_dialog_base<T>::path_open_dialog_base (const path_open_dialog_base& rhs)
      : super(rhs)
    {}

    template<typename T>
    path_open_dialog_base<T>::path_open_dialog_base (path_open_dialog_base&& rhs)
      : super(std::move(rhs))
    {}

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

      super::create(parent, title,
                    core::rectangle(300, 200, 600, 400),
                    [&] (int btn) {
        if (1 == btn) {
          if (super::content_view.second.list.get_selection() > -1) {
            action(super::content_view.second.get_selected_path());
          } else {
            int idx = super::content_view.first.get_selection();
            if (idx > -1) {
              action(super::content_view.first.get_item(idx).path);
            }
          }
        }
      }, {cancel_label, ok_label});

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

  } // ctrl

} // gui

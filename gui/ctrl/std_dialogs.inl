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
 * @brief     C++ API: standard dialogs
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    //-----------------------------------------------------------------------------
    template<int T, int L, int R>
    void standard_dialog_base<T, L, R>::create (win::overlapped_window& parent,
                                                const std::string& title,
                                                const core::rectangle& rect,
                                                std::function<dialog_action> action,
                                                const std::initializer_list<std::string>& labels) {
      super::get_layout().set_bottom(layout::lay(button_layout));
      super::create(parent, rect);
      super::set_title(title);
      buttons.resize(labels.size());

      auto i = 0;
      for (auto l : labels) {
        text_button& btn = buttons[i];
        btn.on_clicked([&, action, i] () {
          super::end_modal();
          if (action) {
            action(*this, i);
          }
        });
        btn.create(*this, l);
        button_layout.add(layout::lay(btn));
        ++i;
      }
      super::on_set_focus([&] () {
        if (!buttons.empty()) {
          buttons.back().take_focus();
        }
      });
    }

    //-----------------------------------------------------------------------------
    template<int T, int L, int R>
    void standard_dialog_base<T, L, R>::show (win::overlapped_window& parent) {
      super::run_modal(parent, {
        win::hot_key_action{
          core::hot_key(core::keys::escape, core::state::none),
          [&] () {
            super::end_modal();
          }
        }
      });
    }

    template<int T, int L, int R>
    standard_dialog_base<T, L, R>::standard_dialog_base ()
    {}

    //-----------------------------------------------------------------------------
    template<typename C, int T, int L, int R>
    inline standard_dialog<C, T, L, R>::standard_dialog () {
      super::set_background(color::very_light_gray);
      content_view.set_background(color::very_light_gray);
    }

    template<typename C, int T, int L, int R>
    void standard_dialog<C, T, L, R>::create (win::overlapped_window& parent,
                                              const std::string& title,
                                              const core::rectangle& rect,
                                              std::function<dialog_action> action,
                                              const std::initializer_list<std::string>& labels) {
      super::get_layout().set_center(layout::lay(content_view));
      super::create(parent, title, rect, action, labels);
      content_view.create(*this, rect);
    }

    //-----------------------------------------------------------------------------
    template<typename ... Arguments>
    multi_input_dialog<Arguments...>::multi_input_dialog () {
      for (int n = 0; n < N; ++n) {
        content_view.get_layout().add(layout::lay(labels[n]));
        content_view.get_layout().add(layout::lay(edits[n]));
      }
    }

    template<typename ... Arguments>
    void multi_input_dialog<Arguments...>::create (win::overlapped_window& parent,
                                                   const std::string& title,
                                                   const std::vector<std::string>& message,
                                                   const std::tuple<Arguments...>& initial,
                                                   const std::string& ok_label,
                                                   const std::string& cancel_label,
                                                   const core::rectangle& rect,
                                                   std::function<action> action) {
      super::create(parent, title, rect, [&, action] (win::overlapped_window&, int i) {
        if (i == 1) {
          std::vector<std::string> strings;
          for (int n = 0; n < N; ++n) {
            strings.emplace_back(edits[n].get_text());
          }
          action(util::tuple::convert::from_vector<Arguments...>(strings));
        }
      }, {cancel_label, ok_label});
      for (int n = 0; n < N; ++n) {
        labels[n].create(content_view, message[n]);
        edits[n].create(content_view, util::tuple::convert::as_string(n, initial));
      }
    }

    template<typename ... Arguments>
    void multi_input_dialog<Arguments...>::ask (win::overlapped_window& parent,
                                                const std::string& title,
                                                const std::vector<std::string>& message,
                                                const std::tuple<Arguments...>& initial,
                                                const std::string& ok_label,
                                                const std::string& cancel_label,
                                                std::function<action> action) {
      multi_input_dialog dialog;
      dialog.create(parent, title, message, initial, ok_label, cancel_label,
                    detail::std_multi_input_dialog_size<>(parent.geometry(), N),
                    action);
      dialog.show(parent);
    }

    //-----------------------------------------------------------------------------
    template<typename T>
    select_dialog<T>::select_dialog () {
      content_view.get_layout().set_center(layout::lay(vlist));
    }

    template<typename T>
    void select_dialog<T>::create (win::overlapped_window& parent,
                                   const std::string& title,
                                   const std::vector<T>& data,
                                   const T initial,
                                   const std::string& ok_label,
                                   const std::string& cancel_label,
                                   const core::rectangle& rect,
                                   std::function<select_action> action) {
      super::create(parent, title, rect, [&, action] (win::overlapped_window& dlg, int i) {
        if ((i == 1) && (vlist->has_selection())) {
          action(dlg, data[vlist->get_selection()]);
        }
      }, {cancel_label, ok_label});
      vlist->set_data(data);
      auto it = std::find(data.begin(), data.end(), initial);
      if (it != data.end()) {
        int idx = std::distance(data.begin(), it);
        vlist->set_selection(idx, event_source::logic);
      }
      vlist->on_selection_commit([&] () {
        super::end_modal();
        if (action) {
          action(*this, data[vlist->get_selection()]);
        }
      });
      vlist.create(super::content_view);
    }

    template<typename T>
    void select_dialog<T>::ask (win::overlapped_window& parent,
                                const std::string& title,
                                const std::vector<T>& list,
                                const T initial,
                                const std::string& ok_label,
                                const std::string& cancel_label,
                                std::function<select_action> action) {
      select_dialog dialog;
      dialog.create(parent, title, list, initial, ok_label, cancel_label,
                    gui::ctrl::detail::std_multi_input_dialog_size<>(parent.geometry(), 5),
                    action);
      dialog.show(parent);
    }

    //-----------------------------------------------------------------------------
    template<typename ... Arguments>
    select_from_columnlist_dialog<Arguments...>::select_from_columnlist_dialog () {
      content_view.get_layout().set_center(layout::lay(vlist));
    }

    template<typename ... Arguments>
    void select_from_columnlist_dialog<Arguments...>::create (win::overlapped_window& parent,
                 const std::string& title,
                 data_t& data,
                 const int initial,
                 std::initializer_list<std::string> labels,
                 const std::string& ok_label,
                 const std::string& cancel_label,
                 const core::rectangle& rect,
                 std::function<select_action> action) {
      super::create(parent, title, rect, [&, action] (win::overlapped_window& dlg, int i) {
        if ((i == 1) && (vlist.list->has_selection())) {
          action(dlg, data.at(vlist.list->get_selection()));
        }
      }, {cancel_label, ok_label});
      vlist.get_column_layout().set_column_count(labels.size());
      vlist.set_data([&] () -> data_t& { return data; });
      vlist.header.set_labels(std::move(labels));
      vlist.list->set_selection(initial, event_source::logic);
      vlist.list->on_selection_commit([&] () {
        super::end_modal();
        if (action) {
          action(*this, data.at(vlist.list->get_selection()));
        }
      });
      vlist.create(super::content_view);
    }

    template<typename ... Arguments>
    void select_from_columnlist_dialog<Arguments...>::ask (win::overlapped_window& parent,
                     const std::string& title,
                     data_t& data,
                     const int initial,
                     std::initializer_list<std::string> labels,
                     const std::string& ok_label,
                     const std::string& cancel_label,
                     std::function<select_action> action) {
      select_from_columnlist_dialog<Arguments...> dialog;
      dialog.create(parent, title, data, initial, std::move(labels),
                    ok_label, cancel_label,
                    gui::ctrl::detail::std_multi_input_dialog_size<>(parent.geometry(), 5),
                    std::move(action));
      dialog.show(parent);
    }

    //-----------------------------------------------------------------------------
    template<typename T>
    void dir_file_view<T>::init (std::function<file_selected> action,
                                 std::function<fs::filter_fn> filter) {
      super::first->on_selection_changed([&,filter](event_source) {
        int idx = super::first->get_selection();
        if (idx > -1) {
          super::second.set_path(super::first->get_item(idx).path, filter);
        }
      });
      super::second.list->on_selection_commit([&, action, filter] () {
        auto path = super::second.get_selected_path();
        if (sys_fs::is_directory(path)) {
          super::first->open_node(path.parent_path());
          super::first->select_node(path);
          super::second.set_path(path, filter);
        } else {
          action(super::get_overlapped_window(), path);
        }
      });
    }

    //-----------------------------------------------------------------------------
    template<typename T>
    void path_open_dialog_base<T>::create (win::overlapped_window& parent,
                                           const std::string& title,
                                           const std::string& ok_label,
                                           const std::string& cancel_label,
                                           const core::rectangle& rect,
                                           std::function<file_selected> action,
                                           std::function<fs::filter_fn> filter) {
      auto& dir_tree = super::content_view.first.view;
      auto& file_list = super::content_view.second;

      super::content_view.init([&, action] (win::overlapped_window& dlg, const sys_fs::path& path) {
        super::set_visible(false);
        super::end_modal();
        action(dlg, path);
      }, filter);

      super::create(parent, title, rect, [&, action] (win::overlapped_window& dlg, int btn) {
        if (1 == btn) {
          if (super::content_view.second.list->get_selection() > -1) {
            action(dlg, super::content_view.second.get_selected_path());
          } else {
            int idx = super::content_view.first->get_selection();
            if (idx > -1) {
              action(dlg, super::content_view.first->get_item(idx).path);
            }
          }
        }
      }, {cancel_label, ok_label});

      super::content_view.set_split_pos(0.5);

      sys_fs::path current = sys_fs::current_path();
      dir_tree.set_roots(fs::get_all_root_file_infos());
      for (auto next = current; next.has_root_path() && (next != next.root_path()); next = next.parent_path()) {
        dir_tree.add_open_node(next);
      }
      dir_tree.add_open_node(current.root_path());
      dir_tree.update_node_list();
      dir_tree.select_node(current);
      file_list.set_path(current, filter);
    }

    template<typename T>
    void path_open_dialog_base<T>::show (win::overlapped_window& parent,
                                         const std::string& title,
                                         const std::string& ok_label,
                                         const std::string& cancel_label,
                                         std::function<file_selected> action,
                                         std::function<fs::filter_fn> filter) {
      path_open_dialog_base dialog;
      dialog.create(parent, title, ok_label, cancel_label,
                    detail::std_path_open_dialog_size<>(parent.geometry()),
                    action, filter);
      dialog.super::show(parent);
    }

  } // ctrl

} // gui

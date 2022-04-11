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

#pragma once


namespace gui {

  namespace ctrl {

    //-----------------------------------------------------------------------------
    template<int T, int L, int R>
    void standard_dialog_base<T, L, R>::create (win::overlapped_window& parent,
                                                const std::string& title,
                                                const core::rectangle& rect,
                                                const std::initializer_list<std::string>& labels,
                                                std::function<dialog_action> action) {
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
    inline standard_dialog<C, T, L, R>::standard_dialog (content_view_type&& content_view)
      : content_view(std::move(content_view))
    {
      super::set_background(color::very_light_gray);
    }

    template<typename C, int T, int L, int R>
    inline standard_dialog<C, T, L, R>::standard_dialog (std::vector<std::reference_wrapper<win::window>> list) {
      super::set_background(color::very_light_gray);
      content_view.set_background(color::very_light_gray);
      content_view.add(list);
    }

    template<typename C, int T, int L, int R>
    void standard_dialog<C, T, L, R>::create (win::overlapped_window& parent,
                                              const std::string& title,
                                              const core::rectangle& rect,
                                              const std::initializer_list<std::string>& labels,
                                              std::function<dialog_action> action) {
      super::get_layout().set_center(layout::lay(content_view));
      super::create(parent, title, rect, labels, action);
      content_view.create(*this);
    }

    //-----------------------------------------------------------------------------
    template<typename T>
    yes_no_dialog_t<T>::yes_no_dialog_t ()
    {}

    template<typename T>
    yes_no_dialog_t<T>::yes_no_dialog_t (content_view_type&& rhs)
      :super(std::move(rhs))
    {}

    template<typename T>
    yes_no_dialog_t<T>::yes_no_dialog_t (std::vector<std::reference_wrapper<win::window>> list)
      :super(list)
    {}

    template<typename T>
    void yes_no_dialog_t<T>::create (win::overlapped_window& parent,
                                     const std::string& title,
                                     const std::string& yes_label,
                                     const std::string& no_label,
                                     const core::rectangle& rect,
                                     std::function<yes_no_action> act) {
      action = act;
      super::create(parent, title, rect, {no_label, yes_label},
                    [&] (win::overlapped_window& dlg, int i) {
        action(dlg, i == 1);
      });
    }

    template<typename T>
    void yes_no_dialog_t<T>::show (win::overlapped_window& parent) {
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
      super::create(parent, title, rect, {cancel_label, ok_label},
                    [&, action] (win::overlapped_window&, int i) {
        if (i == 1) {
          std::vector<std::string> strings;
          for (int n = 0; n < N; ++n) {
            strings.emplace_back(edits[n].get_text());
          }
          action(util::tuple::convert::from_vector<Arguments...>(strings));
        }
      });
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
      super::create(parent, title, rect, {cancel_label, ok_label},
                    [&, action] (win::overlapped_window& dlg, int i) {
        if ((i == 1) && (vlist->has_selection())) {
          const int sel = vlist->get_selection();
          action(dlg, sel, data[sel]);
        }
      });
      vlist->set_data(data);
      auto it = std::find(data.begin(), data.end(), initial);
      if (it != data.end()) {
        int idx = std::distance(data.begin(), it);
        vlist->set_selection(idx, event_source::logic);
      }
      vlist->on_selection_commit([&, action] () {
        super::end_modal();
        if (action) {
          const int sel = vlist->get_selection();
          action(*this, sel, data[sel]);
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
      super::create(parent, title, rect, {cancel_label, ok_label},
                    [&, action] (win::overlapped_window& dlg, int i) {
        if ((i == 1) && (vlist.list->has_selection())) {
          const int sel = vlist.list->get_selection();
          action(dlg, sel, data.at(sel));
        }
      });
      vlist.get_column_layout().set_column_count(labels.size());
      vlist.set_data([&] () -> data_t& { return data; });
      vlist.header.set_labels(std::move(labels));
      vlist.list->set_selection(initial, event_source::logic);
      vlist.list->on_selection_commit([&, action] () {
        super::end_modal();
        if (action) {
          const int sel = vlist.list->get_selection();
          action(*this, sel, data.at(sel));
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
    dir_file_view<T>::dir_file_view (create_subdirectory_fn fn) {
      super::first.set_create_subdirectory_fn(fn);
    }

    template<typename T>
    void dir_file_view<T>::init (std::function<file_selected> action,
                                 std::function<fs::filter_fn> filter) {
      super::first.view->on_selection_changed([&,filter](event_source) {
        if (super::first.view->has_selection()) {
          super::second.set_path(super::first.view->get_item(super::first.view->get_selection()).path, filter);
        }
      });
      super::second.list->on_selection_commit([&, action, filter] () {
        auto path = super::second.get_selected_path();
        if (sys_fs::is_directory(path)) {
          super::first.view->open_node(path.parent_path());
          super::first.view->select_node(path);
          super::second.set_path(path, filter);
        } else {
          action(super::get_overlapped_window(), path);
        }
      });
    }

    //-----------------------------------------------------------------------------
    template<typename T>
    path_open_dialog_base<T>::path_open_dialog_base (create_subdirectory_fn fn)
      : super(dir_file_view<T>(fn))
    {}

    template<typename T>
    void path_open_dialog_base<T>::create (win::overlapped_window& parent,
                                           const std::string& title,
                                           const std::string& ok_label,
                                           const std::string& cancel_label,
                                           const core::rectangle& rect,
                                           std::function<file_selected> action,
                                           std::function<fs::filter_fn> filter) {
      auto& dir_tree = super::content_view.first.view.view;
      auto& file_list = super::content_view.second;

      super::content_view.init([&, action] (win::overlapped_window& dlg, const sys_fs::path& path) {
        super::set_visible(false);
        super::end_modal();
        action(dlg, path);
      }, filter);

      super::create(parent, title, rect, {cancel_label, ok_label},
                    [&, action] (win::overlapped_window& dlg, int btn) {
        if (1 == btn) {
          if (super::content_view.second.list->get_selection() > -1) {
            action(dlg, super::content_view.second.get_selected_path());
          } else {
            int idx = super::content_view.first.view->get_selection();
            if (idx > -1) {
              action(dlg, super::content_view.first.view->get_item(idx).path);
            }
          }
        }
      });

      super::content_view.set_split_pos(0.3);

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
                                         std::function<fs::filter_fn> filter,
                                         create_subdirectory_fn fn) {
      path_open_dialog_base dialog(fn);
      dialog.create(parent, title, ok_label, cancel_label,
                    detail::std_path_open_dialog_size<>(parent.geometry()),
                    action, filter);
      dialog.super::show(parent);
    }

  } // ctrl

} // gui

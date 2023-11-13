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

    namespace input {

      template<typename T, typename L>
      T limited_number_edit<T, L>::get () const {
        return super::get();
      }

      template<typename T, typename L>
      void limited_number_edit<T, L>::set (const detail::limited_number<T, L>& l) {
        super::edit.set_limiter(l.limiter);
        super::set(l.value);
      }

      template<typename T, typename C>
      T option_list<T, C>::get () const {
        return options[get_selection()];
      }

      template<typename T, typename C>
      void option_list<T, C>::set (const detail::selectable_option<T, C>& o) {
        options = o.options;
        set_data<T>(options);
        auto i = std::find(options.begin(), options.end(), o.value);
        if (i != options.end()) {
          set_selection(std::distance(options.begin(), i), event_source::logic);
        }
      }

      inline controls<util::time::time_point>::controls ()
        :label(editor)
      {}

    } // namespace input

    //-----------------------------------------------------------------------------
    namespace detail {

      //-----------------------------------------------------------------------------
      template<int N, typename C, typename T>
      struct mutli_input_layout_traits {

        static void add (C& container, T& edits) {
          mutli_input_layout_traits<N - 1, C, T>::add(container, edits);
          auto& ctrls = std::get<N>(edits);
          container.get_layout().add(ctrls.label);
          container.get_layout().add(ctrls.editor);
        }

      };

      template<typename C, typename T>
      struct mutli_input_layout_traits<-1, C, T> {

        static void add (C&, T&)
        {}

      };

      template<typename C, typename T>
      void add2layout (C& container, T& edits) {
        mutli_input_layout_traits<(std::tuple_size<T>::value - 1), C, T>::add(container, edits);
      }

      //-----------------------------------------------------------------------------
      template<typename C, typename T, typename V>
      struct init {
        static void control_pair (C& container, T& ctrls, const V& value, const std::string& message) {
          ctrls.label.create(container, message);
          ctrls.editor.create(container);
          ctrls.editor.set(value);
        }
      };

      template<typename C, typename T>
      struct init<C, T, bool> {
        static void control_pair (C& container, T& ctrls, bool value, const std::string& message) {
          ctrls.label.create(container);
          ctrls.editor.create(container, message);
          ctrls.editor.set(value);
        }
      };

      template<typename C, typename T>
      struct init<C, T, util::time::time_point> {
        static void control_pair (C& container, T& ctrls, util::time::time_point value, const std::string& message) {
          ctrls.label.create(container);
          ctrls.editor.create(container);
          ctrls.label.set_labels(message);
          ctrls.editor.set(value);
        }
      };

      template<typename C, typename T, typename V>
      void init_control_pair (C& container, T& ctrl, const V& value, const std::string& message) {
        init<C, T, V>::control_pair(container, ctrl, value, message);
      }

      //-----------------------------------------------------------------------------
      template<int N, typename C, typename T, typename V, std::size_t S>
      struct mutli_input_create_traits {

        static void create (C& container, T& edits, const V& v,
                            const std::array<std::string, S>& message) {
          mutli_input_create_traits<N - 1, C, T, V, S>::create(container, edits, v, message);
          init_control_pair(container, std::get<N>(edits), std::get<N>(v), message[N]);
        }

      };

      template<typename C, typename T, typename V, std::size_t S>
      struct mutli_input_create_traits<0, C, T, V, S> {

        static void create (C& container, T& edits, const V& v,
                            const std::array<std::string, S>& message) {
          init_control_pair(container, std::get<0>(edits), std::get<0>(v), message[0]);
        }

      };

      template<typename C, typename T, typename V, std::size_t N>
      void create (C& container, T& edits, const V& v,
                   const std::array<std::string, N>& message) {
        mutli_input_create_traits<(std::tuple_size<T>::value - 1), C, T, V, N>::create(container, edits, v, message);
      }

      //-----------------------------------------------------------------------------
      template<int I, typename E, typename T, typename ... Arguments>
      struct mutli_input_value_traits {

        static std::tuple<T, Arguments ...> get (const E& edits) {
          return std::tuple_cat(std::make_tuple(std::get<I>(edits).editor.get()),
                                mutli_input_value_traits<I + 1, E, Arguments ...>::get(edits));
        }

      };

      template<int I, typename E, typename T>
      struct mutli_input_value_traits<I, E, T> {

        static std::tuple<T> get (const E& edits) {
          return std::make_tuple(std::get<I>(edits).editor.get());
        }

      };

      template<typename E, typename ... Arguments>
      std::tuple<Arguments...> get_values (const E& e) {
        return mutli_input_value_traits<0, E, Arguments...>::get(e);
      }

      //-----------------------------------------------------------------------------
    } // namespace detail

    //-----------------------------------------------------------------------------
    template<typename ... Arguments>
    multi_input_dialog<Arguments...>::multi_input_dialog () {
      detail::add2layout(content_view, edits);
    }

    template<typename ... Arguments>
    void multi_input_dialog<Arguments...>::create (win::overlapped_window& parent,
                                                   const std::string& title,
                                                   const std::array<std::string, N>& message,
                                                   const init_types& initial,
                                                   const std::string& ok_label,
                                                   const std::string& cancel_label,
                                                   const core::rectangle& rect,
                                                   std::function<action> action) {
      super::create(parent, title, rect, {cancel_label, ok_label},
                    [&, action] (win::overlapped_window&, int i) {
        if (i == 1) {
          action(detail::get_values<controls_types, typename input::value_type<Arguments>::type ...>(edits));
        }
      });
      detail::create(content_view, edits, initial, message);
    }

    template<typename ... Arguments>
    core::rectangle multi_input_dialog<Arguments...>::get_standard_size (win::overlapped_window& parent) {
      return detail::std_multi_input_dialog_size<>(parent.geometry(), N * 2, 22);
    }

    template<typename ... Arguments>
    void multi_input_dialog<Arguments...>::ask (win::overlapped_window& parent,
                                                const std::string& title,
                                                const std::array<std::string, N>& message,
                                                const init_types& initial,
                                                const std::string& ok_label,
                                                const std::string& cancel_label,
                                                std::function<action> action) {
      multi_input_dialog dialog;
      dialog.create(parent, title, message, initial, ok_label, cancel_label,
                    get_standard_size(parent), action);
      dialog.show(parent);
    }

  } // ctrl

} // gui

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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/date_time_header_line.h>
#include <gui/ctrl/drop_down.h>
#include <util/tuple_util.h>


namespace gui {

  namespace ctrl {

    namespace detail {
      //-----------------------------------------------------------------------------
      template<typename T, typename C>
      struct selectable_option {
        T value;
        C options;
      };

      template<typename T, typename L>
      struct limited_number {
        T value;
        L limiter;
      };
    
    } // namespace detail
    
    //-----------------------------------------------------------------------------
    template<typename T, typename C>
    detail::selectable_option<T, C> mk_selectable_option (const T& current, const C& options) {
      return {current, options};
    }

    template<typename T, typename L>
    detail::limited_number<T, L> mk_limited_number (const T& current, const L& limiter) {
      return {current, limiter};
    }

    //-----------------------------------------------------------------------------
    namespace input {

      template<typename T, typename L>
      class limited_number_edit : public number_edit<T, default_converter<T>, L> {
      public:
        typedef number_edit<T, default_converter<T>, L> super;

        T get () const;
        void set (const detail::limited_number<T, L>& l);

      };

      template<typename T, typename C>
      class option_list : public drop_down_list {
      public:
        T get () const;
        void set (const detail::selectable_option<T, C>& o);

      private:
        C options;

      };

      using label_t = basic_label<text_origin_t::bottom_left,
                                  draw::frame::no_frame>;

      template<typename T, class Enable = void>
      struct controls {
        label_t label;
        ctrl::basic_edit<T> editor;
      };

      template<typename T>
      struct controls<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
        label_t label;
        ctrl::number_edit<T> editor;
      };

      template<typename T, typename C>
      struct controls<detail::selectable_option<T, C>> {
        label_t label;
        option_list<T, C> editor;
      };

      template<typename T, typename L>
      struct controls<detail::limited_number<T, L>> {
        label_t label;
        limited_number_edit<T, L> editor;
      };

      template<>
      struct controls<bool> {
        label_t label;
        ctrl::check_box<> editor;
      };

      template<>
      struct controls<const std::string> {
        label_t label;
        label_t editor;
      };

      template<>
      struct controls<util::time::time_point> {
        ctrl::date_time_header_line<util::time::time_point> label;
        ctrl::date_time_edit<util::time::time_point> editor;

        controls ();

      };

      template<>
      struct controls<util::time::duration> {
        label_t label;
        ctrl::duration_edit<util::time::duration> editor;
      };

      template<typename T>
      struct value_type {
        typedef T type;
      };

      template<typename T, typename C>
      struct value_type<detail::selectable_option<T, C>> {
        typedef T type;
      };

      template<typename T, typename C>
      struct value_type<detail::limited_number<T, C>> {
        typedef T type;
      };

    } // namespace input

    //-----------------------------------------------------------------------------
    template<typename ... Arguments>
    class multi_input_dialog :
        public standard_dialog<win::group_window<layout::vertical_lineup<20, 15, 2>>> {
    public:
      typedef win::group_window<layout::vertical_lineup<20, 15, 2>> content_view_type;
      typedef standard_dialog<content_view_type> super;
      typedef typename super::layout_type layout_type;
      static constexpr std::size_t N = sizeof...(Arguments);
      typedef std::tuple<Arguments ...> init_types;
      typedef std::tuple<typename input::value_type<Arguments>::type ...> result_types;
      typedef std::tuple<typename input::controls<Arguments> ...> controls_types;

      typedef void (action) (const result_types&);

      multi_input_dialog ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const std::array<std::string, N>& message,
                   const init_types& initial,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<action> action);

      static void ask (win::overlapped_window& parent,
                       const std::string& title,
                       const std::array<std::string, N>& message,
                       const init_types& initial,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       std::function<action> action);

      static core::rectangle get_standard_size (win::overlapped_window& parent);

      controls_types edits;
    };

  } // ctrl

} // gui

#include <gui/ctrl/multi_input_dialog.inl>

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

    template<typename T, std::size_t N>
    struct selectable_option {
      T value;
      std::array<T, N> options;
    };

    template<typename T, std::size_t N>
    selectable_option<T, N> mk_selectable_option (const T& current, const std::array<T, N>& options) {
      return {current, options};

    }


    //-----------------------------------------------------------------------------
    namespace input {

      template<typename T, std::size_t N>
      class option_list : public drop_down_list {
      public:
        option_list ()
        {}

        T get () const {
          return options[get_selection()];
        }

        void set (const selectable_option<T, N>& o) {
          options = o.options;
          set_data<T>(options);
          auto i = std::find(options.begin(), options.end(), o.value);
          if (i != options.end()) {
            set_selection(std::distance(options.begin(), i), event_source::logic);
          }
        }

        std::array<T, N> options;
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

      template<typename T, std::size_t N>
      struct controls<selectable_option<T, N>> {
        label_t label;
        option_list<T, N> editor;
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

        inline controls ()
          :label(editor)
        {}
      };

      template<>
      struct controls<util::time::duration> {
        label_t label;
        ctrl::duration_edit<util::time::duration> editor;
      };

      template<typename T, class Enable = void>
      struct value_type {
        typedef T type;
      };

      template<typename T, std::size_t N>
      struct value_type<selectable_option<T, N>> {
        typedef T type;
      };

    }

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
                   const std::vector<std::string>& message,
                   const init_types& initial,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<action> action);

      static void ask (win::overlapped_window& parent,
                       const std::string& title,
                       const std::vector<std::string>& message,
                       const init_types& initial,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       std::function<action> action);

      controls_types edits;
    };

  } // ctrl

} // gui

#include <gui/ctrl/multi_input_dialog.inl>

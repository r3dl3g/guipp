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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/ctrl/control.h>
#include <gui/ctrl/file_tree.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/textbox.h>
#include <gui/ctrl/list.h>

#include <util/tuple_util.h>


namespace gui {

  namespace ctrl {

    //-----------------------------------------------------------------------------
    typedef void (dialog_action) (win::overlapped_window&, int); // Button no. pressed

    //-----------------------------------------------------------------------------
    typedef void (yes_no_action) (win::overlapped_window&, bool);

    //-----------------------------------------------------------------------------
    typedef void (input_action) (win::overlapped_window&, const std::string&);

    //-----------------------------------------------------------------------------
    typedef void (file_selected) (win::overlapped_window&, const sys_fs::path&);

    namespace detail {

      // --------------------------------------------------------------------------
      template<core::os::ui_t T = core::os::system_ui>
      core::rectangle std_multi_input_dialog_size (const core::rectangle&, std::size_t n);

      template<core::os::ui_t T = core::os::system_ui>
      core::rectangle std_path_open_dialog_size (const core::rectangle& area);

      template<core::os::ui_t T = core::os::system_ui>
      core::rectangle std_message_dialog_size (const core::rectangle& area);

      template<core::os::ui_t T = core::os::system_ui>
      core::rectangle std_yes_no_dialog_size (const core::rectangle& area);

      template<core::os::ui_t T = core::os::system_ui>
      core::rectangle std_input_dialog_size (const core::rectangle& area);

      template<core::os::ui_t T = core::os::system_ui>
      core::rectangle std_file_save_dialog_size (const core::rectangle& area);

    } // namespace detail

    //-----------------------------------------------------------------------------
    template<int TO = 0, int LE = 0, int RI = 0>
    class standard_dialog_base :
        public win::layout_dialog_window<layout::border::layouter<TO, 45, LE, RI>> {
    public:
      typedef win::layout_dialog_window<layout::border::layouter<TO, 45, LE, RI>> super;
      using button_layout_type = layout::horizontal_adaption<>;

      standard_dialog_base ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const core::rectangle& rect,
                   std::function<dialog_action> action,
                   const std::initializer_list<std::string>& button_labels);

      void show (win::overlapped_window& parent);

      button_layout_type button_layout;
      std::vector<text_button> buttons;
    };

    //-----------------------------------------------------------------------------
    template<typename C, int TO = 0, int LE = 0, int RI = 0>
    class standard_dialog : public standard_dialog_base<TO, LE, RI> {
    public:
      typedef standard_dialog_base<TO, LE, RI> super;
      typedef C content_view_type;

      standard_dialog ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const core::rectangle& rect,
                   std::function<dialog_action> action,
                   const std::initializer_list<std::string>& button_labels);

      content_view_type content_view;
    };

    //-----------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT message_dialog :
        public standard_dialog<win::group_window<layout::border::layouter<20, 15, 15, 15>>> {
    public:
      typedef basic_textbox<text_origin_t::center,
                            draw::frame::sunken_relief,
                            color::black,
                            color::very_light_gray> message_view_type;
      typedef win::group_window<layout::border::layouter<20, 15, 15, 15>> content_view_type;
      typedef standard_dialog<content_view_type> super;

      message_dialog ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const std::string& message,
                   const std::string& ok_label,
                   const core::rectangle& rect);

      static void show (win::overlapped_window& parent,
                       const std::string& title,
                       const std::string& message,
                       const std::string& ok_label);

      message_view_type message_view;

    };

    //-----------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT yes_no_dialog :
        public standard_dialog<win::group_window<layout::border::layouter<20, 15, 15, 15>>> {
    public:
      typedef basic_textbox<text_origin_t::center,
                            draw::frame::sunken_relief,
                            color::black,
                            color::very_light_gray> message_view_type;
      typedef win::group_window<layout::border::layouter<20, 15, 15, 15>> content_view_type;
      typedef standard_dialog<content_view_type> super;

      yes_no_dialog ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const std::string& message,
                   const std::string& yes_label,
                   const std::string& no_label,
                   const core::rectangle& rect,
                   std::function<yes_no_action> action);

      static void ask (win::overlapped_window& parent,
                       const std::string& title,
                       const std::string& message,
                       const std::string& yes_label,
                       const std::string& no_label,
                       std::function<yes_no_action> action);

      void show (win::overlapped_window& parent,
                 std::function<yes_no_action> action);

      message_view_type message_view;

    };

    //-----------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT input_dialog :
        public standard_dialog<win::group_window<layout::vertical_lineup<20, 15, 2>>> {
    public:
      typedef win::group_window<layout::vertical_lineup<20, 15, 2>> content_view_type;
      typedef standard_dialog<content_view_type> super;

      input_dialog ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const std::string& message,
                   const std::string& initial,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<input_action> action);

      static void ask (win::overlapped_window& parent,
                       const std::string& title,
                       const std::string& message,
                       const std::string& initial,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       std::function<input_action> action);

      label_left label;
      edit_left edit;
    };

    //-----------------------------------------------------------------------------
    template<typename ... Arguments>
    class multi_input_dialog :
        public standard_dialog<win::group_window<layout::vertical_lineup<20, 15, 2>>> {
    public:
      typedef win::group_window<layout::vertical_lineup<20, 15, 2>> content_view_type;
      typedef standard_dialog<content_view_type> super;
      static constexpr std::size_t N = sizeof...(Arguments);
      using label_t = basic_label<text_origin_t::bottom_left,
                                  draw::frame::no_frame>;

      typedef void (action) (const std::tuple<Arguments...>&);

      multi_input_dialog ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const std::vector<std::string>& message,
                   const std::tuple<Arguments...>& initial,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<action> action);

      static void ask (win::overlapped_window& parent,
                       const std::string& title,
                       const std::vector<std::string>& message,
                       const std::tuple<Arguments...>& initial,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       std::function<action> action);

      label_t labels[N];
      edit_left edits[N];
    };

    //-----------------------------------------------------------------------------
    template<typename T>
    class select_dialog :
        public standard_dialog<win::group_window<layout::border::center_layout<vertical_list, 15, 15, 15, 15>>> {
    public:
      typedef win::group_window<layout::border::center_layout<vertical_list, 15, 15, 15, 15>> content_view_type;
      typedef standard_dialog<content_view_type> super;
      typedef void (select_action) (win::overlapped_window&, int, T);

      select_dialog ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const std::vector<T>& data,
                   const T initial,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<select_action> action);

      static void ask (win::overlapped_window& parent,
                       const std::string& title,
                       const std::vector<T>& data,
                       const T initial,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       std::function<select_action> action);

      vertical_scrollable_list vlist;
    };

    //-----------------------------------------------------------------------------
    template<typename ... Arguments>
    class select_from_columnlist_dialog :
        public standard_dialog<win::group_window<layout::border::center_layout<vertical_list, 15, 15, 15, 15>>> {
    public:
      typedef win::group_window<layout::border::center_layout<vertical_list, 15, 15, 15, 15>> content_view_type;
      typedef standard_dialog<content_view_type> super;
      typedef layout::weight_column_list_layout layout_t;
      typedef ctrl::column_list_t<layout_t, Arguments ...> list_t;
      typedef column_list_data_t<Arguments...> data_t;
      typedef typename data_t::row_type row_type;
      typedef void (select_action) (win::overlapped_window&, int, row_type);

      select_from_columnlist_dialog ();

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   data_t& data,
                   const int initial,
                   std::initializer_list<std::string> labels,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<select_action> action);

      static void ask (win::overlapped_window& parent,
                       const std::string& title,
                       data_t& data,
                       const int initial,
                       std::initializer_list<std::string> labels,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       std::function<select_action> action);

      list_t vlist;
    };

    //-----------------------------------------------------------------------------
    template<typename T = path_tree::sorted_path_info>
    class dir_file_view : public vertical_split_view<virtual_view<sorted_dir_tree>,
                                                     file_column_list<T>> {
    public:
      typedef sorted_dir_tree dir_tree_type;
      typedef file_column_list<T> file_list_type;
      typedef vertical_split_view<virtual_view<dir_tree_type>, file_list_type> super;

      void init (std::function<file_selected> action,
                 std::function<fs::filter_fn> filter = nullptr);

    };

    //-----------------------------------------------------------------------------
    template<typename T>
    class path_open_dialog_base : public standard_dialog<dir_file_view<T>> {
    public:
      typedef standard_dialog<dir_file_view<T>> super;

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<file_selected> action,
                   std::function<fs::filter_fn> filter = nullptr);

      static void show (win::overlapped_window& parent,
                        const std::string& title,
                        const std::string& ok_label,
                        const std::string& cancel_label,
                        std::function<file_selected> action,
                        std::function<fs::filter_fn> filter = nullptr);
    };

    //-----------------------------------------------------------------------------
    typedef path_open_dialog_base<path_tree::sorted_path_info> file_open_dialog;
    typedef path_open_dialog_base<path_tree::sorted_dir_info> dir_open_dialog;

    //-----------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT file_save_dialog :
        public standard_dialog<dir_file_view<>, 30> {
    public:
      typedef standard_dialog<dir_file_view<>, 30> super;
      typedef win::group_window<layout::border::layouter<4, 4, 120, 4>> top_view_type;

      void create (win::overlapped_window& parent,
                   const std::string& title,
                   const std::string& default_name,
                   const std::string& name_label,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<file_selected> action);

      static void show (win::overlapped_window& parent,
                        const std::string& title,
                        const std::string& default_name,
                        const std::string& name_label,
                        const std::string& ok_label,
                        const std::string& cancel_label,
                        std::function<file_selected> action);

      top_view_type top_view;
      basic_edit<text_origin_t::vcenter_left,
                 draw::frame::sunken_deep_relief> input_line;
      basic_label<text_origin_t::vcenter_right,
                  draw::frame::no_frame> input_label;

    };

  } // ctrl

} // gui

#include <gui/ctrl/std_dialogs.inl>

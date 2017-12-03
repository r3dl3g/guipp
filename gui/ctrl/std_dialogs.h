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
    class GUIPP_EXPORT standard_dialog_base : public layout_dialog_window<layout::border_layout<>, float, float, float, float> {
    public:
      typedef layout_dialog_window<layout::border_layout<>, float, float, float, float> super;
      typedef group_window<layout::horizontal_adaption<>, color::very_light_gray> button_view_type;

      standard_dialog_base (float top = 0);

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

      standard_dialog (float top = 0);
      standard_dialog (content_view_type&& view, float top = 0);

      void create (win::container& parent,
                   const std::string& title,
                   const std::string& yes_label,
                   const std::string& no_label,
                   const core::rectangle& rect,
                   std::function<yes_no_action> action);

      content_view_type content_view;
    };

    //-----------------------------------------------------------------------------
    class GUIPP_EXPORT yes_no_dialog : public standard_dialog<win::group_window<layout::border_layout<>,
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
                                                          win::file_column_list<T>> {
    public:
      typedef win::sorted_dir_tree dir_tree_type;
      typedef win::file_column_list<T> file_list_type;
      typedef win::vertical_split_view<dir_tree_type, file_list_type> super;

      dir_file_view ();

      void init (std::function<file_selected> action);

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
                   std::function<file_selected> action);

      static void show (win::container& parent,
                        const std::string& title,
                        const std::string& ok_label,
                        const std::string& cancel_label,
                        std::function<file_selected> action);
    };

    //-----------------------------------------------------------------------------
    typedef path_open_dialog_base<path_tree::sorted_path_info> file_open_dialog;
    typedef path_open_dialog_base<path_tree::sorted_dir_info> dir_open_dialog;

    //-----------------------------------------------------------------------------
    class GUIPP_EXPORT file_save_dialog : public standard_dialog<dir_file_view<>> {
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

#include <gui/ctrl/std_dialogs.inl>

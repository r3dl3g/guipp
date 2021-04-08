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
 * @brief     C++ API: file tree/list view
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/sys_fs.h>
#include <util/string_util.h>
#include <gui/ctrl/tree.h>
#include <gui/ctrl/column_list.h>


namespace gui {

  namespace fs {

    typedef bool (filter_fn)(const sys_fs::directory_entry&);

    struct GUIPP_CTRL_EXPORT filtered_iterator {

      filtered_iterator () = default;
      explicit filtered_iterator (const sys_fs::directory_iterator& i, std::function<filter_fn> f = nullptr);
      explicit filtered_iterator (sys_fs::directory_iterator&& i, std::function<filter_fn> f = nullptr);
      filtered_iterator (filtered_iterator&& rhs) noexcept ;
      filtered_iterator (const filtered_iterator& rhs);

      filtered_iterator& operator= (const filtered_iterator&) = default;
      filtered_iterator& operator= (filtered_iterator&&) = default;

      void init ();

      const sys_fs::directory_entry& operator* () const;
      const sys_fs::directory_entry* operator-> () const;

      bool is_excluded (const sys_fs::directory_entry& i) const;

      filtered_iterator& operator++ ();
      filtered_iterator operator++ (int);

      sys_fs::directory_iterator iterator;
      sys_fs::directory_iterator end;
      std::function<filter_fn> filter;
    };

    bool operator== (const filtered_iterator& lhs,
                     const filtered_iterator& rhs);

    bool operator!= (const filtered_iterator& lhs,
                     const filtered_iterator& rhs);

    struct GUIPP_CTRL_EXPORT file_info {
      file_info (const sys_fs::path& path); // NOLINT(google-explicit-constructor)
      file_info ();

      sys_fs::path path;
      sys_fs::file_status status;
      sys_fs::file_time_type last_write_time;
      uintmax_t size;

      bool is_directory () const;
      bool is_regular_file () const;
      std::string filename () const;
      std::string filepath () const;

      bool operator== (const file_info& rhs) const;
      bool operator!= (const file_info& rhs) const;
      bool operator< (const file_info& rhs) const;
      bool operator<= (const file_info& rhs) const;
      bool operator> (const file_info& rhs) const;
      bool operator>= (const file_info& rhs) const;
    };

    std::vector<sys_fs::path> GUIPP_CTRL_EXPORT get_all_root_paths ();
    std::vector<file_info> GUIPP_CTRL_EXPORT get_all_root_file_infos ();

  } // namespace fs

  namespace ctrl {

    namespace path_tree {

      sys_fs::directory_iterator path_iterator (sys_fs::path const& n);

      struct GUIPP_CTRL_EXPORT path_info {
        typedef fs::file_info type;
        typedef fs::filtered_iterator iterator;
        typedef fs::file_info reference;
        typedef std::vector<type> list_type;

        static bool has_sub_nodes (type const& n);
        static reference make_reference (type const& n);
        static type const& dereference (reference const& r);
        static std::string label (type const& n);
        static const gui::tree::icon_drawer* icon (type const&, bool has_children, bool is_open, bool selected);

      };

      struct GUIPP_CTRL_EXPORT unsorted_path_info : public path_info {
        typedef core::range<iterator> range;

        static range sub_nodes (type const& n, const std::function<fs::filter_fn>& f = nullptr);
      };

      struct GUIPP_CTRL_EXPORT unsorted_dir_info : public path_info {
        typedef core::range<iterator> range;

        static range sub_nodes (type const& n, const std::function<fs::filter_fn>& f = nullptr);
      };

      struct GUIPP_CTRL_EXPORT unsorted_file_info : public path_info {
        typedef core::range<iterator> range;

        static range sub_nodes (type const& n, const std::function<fs::filter_fn>& f = nullptr);
      };

      struct GUIPP_CTRL_EXPORT sorted_path_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n, const std::function<fs::filter_fn>& f = nullptr);
      };

      struct GUIPP_CTRL_EXPORT sorted_dir_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n, const std::function<fs::filter_fn>& f = nullptr);
      };

      struct GUIPP_CTRL_EXPORT sorted_file_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n, const std::function<fs::filter_fn>& f = nullptr);
      };


    } // path_tree

    // --------------------------------------------------------------------------
    typedef tree::basic_tree<path_tree::unsorted_path_info> file_tree;
    typedef tree::basic_tree<path_tree::unsorted_dir_info> dir_tree;
    typedef tree::basic_tree<path_tree::sorted_path_info> sorted_file_tree;
    typedef tree::basic_tree<path_tree::sorted_dir_info> sorted_dir_tree;

    // --------------------------------------------------------------------------
    namespace detail {

      struct GUIPP_CTRL_EXPORT file_list_row_data :
          public column_list_data_t<const gui::tree::icon_drawer*,
                                    const fs::file_info&,
                                    const fs::file_info&,
                                    const sys_fs::file_time_type&> {

        typedef column_list_data_t<const gui::tree::icon_drawer*,
                                   const fs::file_info&,
                                   const fs::file_info&,
                                   const sys_fs::file_time_type&> super;
        typedef typename super::row_type row_type;
        typedef typename super::drawer_type drawer_type;
        typedef vertical_scrollable_list list_type;

        file_list_row_data (const std::vector<fs::file_info>&, const list_type&);

        std::size_t size () const override;

        row_type at (std::size_t idx) const override;

      private:
        const std::vector<fs::file_info>& data;
        const list_type& list;

      };

      GUIPP_CTRL_EXPORT void init_file_list_layout (layout::weight_column_list_layout&, core::size::type);

      GUIPP_CTRL_EXPORT std::string format_file_size (uintmax_t s);

    } // detail

    // --------------------------------------------------------------------------
    typedef void (file_item_drawer) (const fs::file_info&,
                                     draw::graphics&,
                                     const core::rectangle&,
                                     const draw::brush&,
                                     item_state);
    // --------------------------------------------------------------------------
    GUIPP_CTRL_EXPORT void default_file_item_drawer (const fs::file_info&,
                                                     draw::graphics&,
                                                     const core::rectangle&,
                                                     const draw::brush&,
                                                     item_state);
    // --------------------------------------------------------------------------
    GUIPP_CTRL_EXPORT void filepath_item_drawer (const fs::file_info&,
                                                 draw::graphics&,
                                                 const core::rectangle&,
                                                 const draw::brush&,
                                                 item_state);
    // --------------------------------------------------------------------------
    template<typename T = path_tree::sorted_file_info,
             file_item_drawer D = default_file_item_drawer>
    class file_list : public vertical_list {
    public:
      typedef vertical_list super;

      explicit file_list (core::size::type item_size = list_defaults<>::item_size,
                 os::color background = color::white,
                 bool grab_focus = true);

      file_list (const file_list&);
      file_list (file_list&&) noexcept ;

      void set_path (const sys_fs::path& dir, std::function<fs::filter_fn> f = nullptr);
      sys_fs::path get_selected_path () const;

    private:
      std::vector<fs::file_info> current_dir;

      void init ();
    };

    enum class sort_order : unsigned short {
      none = 0,
      name_up,
      name_down,
      size_up,
      size_down,
      date_up,
      date_down
    };

    GUIPP_CTRL_EXPORT void sort_list_by (std::vector<fs::file_info>& list, sort_order);

    // --------------------------------------------------------------------------
    template<typename T = path_tree::sorted_file_info>
    class file_column_list : public column_list_t<layout::weight_column_list_layout,
                                                  const gui::tree::icon_drawer*, const fs::file_info&, const fs::file_info&, const sys_fs::file_time_type&> {
    public:
      typedef column_list_t<layout::weight_column_list_layout,
                            const gui::tree::icon_drawer*, const fs::file_info&, const fs::file_info&, const sys_fs::file_time_type&> super;

      explicit file_column_list (core::size::type item_size = list_defaults<>::item_size,
                        os::color background = color::white,
                        bool grab_focus = true);
      file_column_list (const file_column_list& rhs);
      file_column_list (file_column_list&& rhs) noexcept ;

      void set_path (const sys_fs::path& dir, std::function<fs::filter_fn> f = nullptr);
      sys_fs::path get_selected_path () const;

      void sort_by (sort_order order);
      sort_order get_sort_order () const;

    protected:
      void init_file_list_header (column_list_header<layout::weight_column_list_layout>&);

    private:
      void handle_header_mouse_down (os::key_state, const core::native_point&);
      void handle_header_mouse_up (os::key_state, const core::native_point&);

      void init ();

      core::point mouse_down_point;
      std::vector<fs::file_info> current_dir;
      sort_order order;

    };

    GUIPP_CTRL_EXPORT void draw_arrow_up (draw::graphics& g, const core::rectangle& r, os::color col);
    GUIPP_CTRL_EXPORT void draw_arrow_down (draw::graphics& g, const core::rectangle& r, os::color col);

  } // ctrl

} // gui

#include <gui/ctrl/file_tree.inl>

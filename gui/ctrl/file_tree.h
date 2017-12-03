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

#if (__cplusplus >= 201700L)

# include <filesystem>
namespace sys_fs = std::filesystem;

#elif defined(USE_BOOST)

# include <boost/filesystem.hpp>
namespace sys_fs = boost::filesystem;

#elif (_MSC_VER >= 1900) || (__GNUC__ > 5) || ((__GNUC__ == 5) && (__GNUC_MINOR__ > 3)) || defined(__clang__)

# include <experimental/filesystem>

namespace sys_fs = std::experimental::filesystem;

#else

# pragma error "No C++ filesystem available"

#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/string_util.h>
#include <gui/ctrl/tree.h>
#include <gui/ctrl/column_list.h>


namespace gui {

  namespace fs {

    struct GUIPP_EXPORT filtered_iterator {
      typedef bool (filter_fn)(const sys_fs::directory_entry&);

      filtered_iterator ();
      filtered_iterator (const sys_fs::directory_iterator& i, std::function<filter_fn> f = nullptr);
      filtered_iterator (sys_fs::directory_iterator&& i, std::function<filter_fn> f = nullptr);
      filtered_iterator (filtered_iterator&& rhs);
      filtered_iterator (const filtered_iterator& rhs);

      filtered_iterator& operator= (const filtered_iterator&) = default;
      filtered_iterator& operator= (filtered_iterator&&) = default;

      void init ();

      const sys_fs::directory_entry& operator* () const;
      const sys_fs::directory_entry* operator-> () const;

      bool is_excluded (const sys_fs::directory_entry& i);

      filtered_iterator& operator++ ();
      filtered_iterator& operator++ (int);

      sys_fs::directory_iterator iterator;
      sys_fs::directory_iterator end;
      std::function<filter_fn> filter;
    };

    bool operator== (const filtered_iterator& lhs,
                     const filtered_iterator& rhs);

    bool operator!= (const filtered_iterator& lhs,
                     const filtered_iterator& rhs);

    struct GUIPP_EXPORT file_info {
      file_info (const sys_fs::path& path);
      file_info ();

      sys_fs::path path;
      sys_fs::file_status status;
      sys_fs::file_time_type last_write_time;
      uintmax_t size;

      bool is_directory () const;
      std::string filename () const;

      bool operator== (const file_info& rhs) const;
      bool operator!= (const file_info& rhs) const;
      bool operator< (const file_info& rhs) const;
      bool operator<= (const file_info& rhs) const;
      bool operator> (const file_info& rhs) const;
      bool operator>= (const file_info& rhs) const;
    };

  } // namespace fs

  namespace win {

    namespace path_tree {

      sys_fs::directory_iterator path_iterator (sys_fs::path const& n);

      struct GUIPP_EXPORT path_info {
        typedef fs::file_info type;
        typedef fs::filtered_iterator iterator;
        typedef fs::file_info reference;

        static bool has_sub_nodes (type const& n);
        static reference make_reference (type const& n);
        static type const& dereference (reference const& r);
        static std::string label (type const& n);
        static const draw::masked_bitmap& icon (type const&, bool has_children, bool is_open, bool selected);

      };

      struct GUIPP_EXPORT unsorted_path_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n);
      };

      struct GUIPP_EXPORT unsorted_dir_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n);
      };

      struct GUIPP_EXPORT unsorted_file_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n);
      };

      struct GUIPP_EXPORT sorted_path_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n);
      };

      struct GUIPP_EXPORT sorted_dir_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n);
      };

      struct GUIPP_EXPORT sorted_file_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n);
      };


    } // path_tree

    // --------------------------------------------------------------------------
    typedef tree::basic_tree<path_tree::unsorted_path_info> file_tree;
    typedef tree::basic_tree<path_tree::unsorted_dir_info> dir_tree;
    typedef tree::basic_tree<path_tree::sorted_path_info> sorted_file_tree;
    typedef tree::basic_tree<path_tree::sorted_dir_info> sorted_dir_tree;

    // --------------------------------------------------------------------------
    namespace detail {

      GUIPP_EXPORT void init_file_list_layout (layout::weight_column_list_layout&);

      typedef column_list_row_drawer_t<layout::weight_column_list_layout,
                                       const draw::masked_bitmap*,
                                       fs::file_info,
                                       fs::file_info,
                                       sys_fs::file_time_type> file_list_row_drawer;

      GUIPP_EXPORT file_list_row_drawer create_file_list_row_drawer ();

      typedef column_list_row_t<const draw::masked_bitmap*,
                                fs::file_info,
                                fs::file_info,
                                sys_fs::file_time_type> file_list_row;

      GUIPP_EXPORT file_list_row build_file_list_row (const fs::file_info&, bool selected);

      GUIPP_EXPORT std::string format_file_size (uintmax_t s);

    } // detail

    // --------------------------------------------------------------------------
    template<typename T = path_tree::sorted_file_info>
    class file_list : public win::vertical_list {
    public:
      typedef win::vertical_list super;

      file_list (core::size::type item_size = 20,
                 os::color background = color::white,
                 bool grab_focus = true);

      void set_path (const sys_fs::path& dir);
      sys_fs::path get_selected_path () const;

    private:
      std::vector<fs::file_info> current_dir;

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

    GUIPP_EXPORT void sort_list_by (std::vector<fs::file_info>& list, sort_order);

    // --------------------------------------------------------------------------
    template<typename T = path_tree::sorted_file_info>
    class file_column_list : public win::column_list_t<layout::weight_column_list_layout,
                                                       const draw::masked_bitmap*, fs::file_info, fs::file_info, sys_fs::file_time_type> {
    public:
      typedef win::column_list_t<layout::weight_column_list_layout,
                                 const draw::masked_bitmap*, fs::file_info, fs::file_info, sys_fs::file_time_type> super;

      file_column_list (core::size::type item_size = 20,
                        os::color background = color::white,
                        bool grab_focus = true);

      void set_path (const sys_fs::path& dir);
      sys_fs::path get_selected_path () const;

      void sort_by (sort_order order);
      sort_order get_sort_order () const;

    protected:
      void init_file_list_header (column_list_header<layout::weight_column_list_layout>&);

    private:
      void handle_header_mouse_down (os::key_state, const core::point&);
      void handle_header_mouse_up (os::key_state, const core::point&);

      core::point mouse_down_point;
      std::vector<fs::file_info> current_dir;
      sort_order order;

    };

    GUIPP_EXPORT void draw_arrow_up (const draw::graphics& g, const core::rectangle& r, os::color col);
    GUIPP_EXPORT void draw_arrow_down (const draw::graphics& g, const core::rectangle& r, os::color col);

  } // win

} // gui

#include <gui/ctrl/file_tree.inl>

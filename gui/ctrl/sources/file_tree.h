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

// --------------------------------------------------------------------------
//
// Library includes
//
#if (__cplusplus >= 201700L)

#include <filesystem>
namespace sys_fs = std::filesystem;

#elif defined(USE_BOOST)

#include <boost/filesystem.hpp>
namespace sys_fs = boost::filesystem;

#elif (_MSC_VER >= 1900) || (__GNUC__ > 5) || ((__GNUC__ == 5) && (__GNUC_MINOR__ >3))

#include <experimental/filesystem>
namespace sys_fs = std::experimental::filesystem;

#else

#pragma error "No C++ filesystem available"

#endif

#include "tree.h"
#include "column_list.h"
#include "string_util.h"


namespace gui {

  namespace fs {

    struct filtered_iterator {
      typedef bool(filter_fn)(const sys_fs::directory_entry&);

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

  } // namespace fs

  namespace win {

    namespace path_tree {

      sys_fs::directory_iterator path_iterator (sys_fs::path const& n);

      struct path_info {
        typedef sys_fs::path type;
        typedef fs::filtered_iterator iterator;
        typedef sys_fs::path reference;

        static bool has_sub_nodes (type const& n);
        static reference make_reference (type const& n);
        static type const& dereference (reference const& r);
        static std::string label (type const& n);
        static const draw::masked_bitmap& icon (type const&, bool has_children, bool is_open, bool selected);

      };

      struct unsorted_path_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n);
      };

      struct unsorted_dir_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n);
      };

      struct unsorted_file_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n);
      };

      struct sorted_path_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n);
      };

      struct sorted_dir_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n);
      };

      struct sorted_file_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n);
      };


    } // path_tree

    // --------------------------------------------------------------------------
    typedef tree::basic_tree<path_tree::unsorted_path_info> file_tree;
    typedef tree::basic_tree<path_tree::sorted_path_info> sorted_file_tree;

    // --------------------------------------------------------------------------
    namespace detail {

      void init_file_list_layout (layout::weight_column_list_layout&);
      void init_file_list_header (column_list_header<layout::weight_column_list_layout>&);

      typedef column_list_row_drawer_t<layout::weight_column_list_layout,
                                       const draw::masked_bitmap*, std::string, uintmax_t,
                                       sys_fs::file_time_type> file_list_row_drawer;

      file_list_row_drawer create_file_list_row_drawer ();

      typedef column_list_row_t<const draw::masked_bitmap*, std::string, uintmax_t,
                                sys_fs::file_time_type> file_list_row;

      file_list_row build_file_list_row (const sys_fs::path&, bool selected);

    } // detail

    // --------------------------------------------------------------------------
    class file_list : public win::column_list_t<layout::weight_column_list_layout,
                                                const draw::masked_bitmap*, std::string, uintmax_t, sys_fs::file_time_type> {
    public:
      typedef win::column_list_t<layout::weight_column_list_layout,
                                 const draw::masked_bitmap*, std::string, uintmax_t, sys_fs::file_time_type> super;

      file_list (core::size_type item_size = 20,
                 os::color background = color::white,
                 bool grab_focus = true);

      void set_path (const sys_fs::path& dir);

    private:
      std::vector<sys_fs::path> current_dir;

    };

  } // win

  // --------------------------------------------------------------------------
  // inlines

  namespace fs {

    inline const sys_fs::directory_entry& filtered_iterator::operator* () const {
      return *iterator;
    }

    inline const sys_fs::directory_entry* filtered_iterator::operator-> () const {
      return &(*iterator);
    }

    inline bool operator== (const filtered_iterator& lhs,
                            const filtered_iterator& rhs) {
      return lhs.iterator == rhs.iterator;
    }

    inline bool operator!= (const filtered_iterator& lhs,
                            const filtered_iterator& rhs) {
      return lhs.iterator != rhs.iterator;
    }

  } // namespace fs

  namespace win {

    namespace path_tree {

      inline sys_fs::directory_iterator path_iterator (sys_fs::path const& n) {
#ifdef WIN32
        return sys_fs::directory_iterator(n);
#endif // WIN32
#ifdef X11
        return sys_fs::directory_iterator(n, sys_fs::directory_options::skip_permission_denied);
#endif // X11
      }

      inline bool path_info::has_sub_nodes (type const& n) {
        return sys_fs::is_directory(n);
      }

      inline auto path_info::make_reference (type const& n) -> reference {
        return n;
      }

      inline auto path_info::dereference (reference const& r) -> type const& {
        return r;
      }

      inline std::string path_info::label (type const& n) {
        return n.filename().string();
      }

      inline const draw::masked_bitmap& path_info::icon (type const&, bool has_children, bool is_open, bool selected) {
        return tree::standard_icon(has_children, is_open, selected);
      }

    } // path_tree

  } // win

  // --------------------------------------------------------------------------

} // gui

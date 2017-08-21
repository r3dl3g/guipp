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
#if (_MSC_VER >= 1900) || (__GNUC__ > 5) || ((__GNUC__ == 5) && (__GNUC_MINOR__ >3))
#include <experimental/filesystem>
namespace sys_fs = std::experimental::filesystem;
#else
#define USE_BOOST
#include <boost/filesystem.hpp>
namespace sys_fs = boost::filesystem;
#endif

#include "tree.h"
#include "column_list.h"
#include "string_util.h"


namespace gui {

  namespace fs {

    struct filtered_iterator {
      typedef bool(filter_fn)(const sys_fs::directory_entry&);

      filtered_iterator ()
      {}

      filtered_iterator (const sys_fs::directory_iterator& i, std::function<filter_fn> f = nullptr)
        : iterator(i)
        , end()
        , filter(f)
      {
        init();
      }

      filtered_iterator (sys_fs::directory_iterator&& i, std::function<filter_fn> f = nullptr)
        : iterator(std::move(i))
        , end()
        , filter(f)
      {
        init();
      }

      filtered_iterator (filtered_iterator&& rhs)
        : iterator(std::move(rhs.iterator))
        , end(std::move(rhs.end))
        , filter(std::move(rhs.filter))
      {
        init();
      }

      filtered_iterator (const filtered_iterator& rhs)
        : iterator(rhs.iterator)
        , end(rhs.end)
        , filter(rhs.filter)
      {
        init();
      }
      filtered_iterator& operator= (const filtered_iterator&) = default;
      filtered_iterator& operator= (filtered_iterator&&) = default;

      void init () {
        while ((iterator != end) && is_excluded(*iterator)) {
          ++iterator;
        }
      }

      const sys_fs::directory_entry& operator* () const { return *iterator; }
      const sys_fs::directory_entry* operator->() const { return &(*iterator); }

      bool is_excluded (const sys_fs::directory_entry& i) {
        return (filter && filter(*iterator));
      }

      filtered_iterator& operator++() {
        do {
          ++iterator;
        } while ((iterator != end) && is_excluded(*iterator));
        return *this;
      }

      filtered_iterator& operator++(int) {
        do {
          ++iterator;
        } while ((iterator != end) && is_excluded(*iterator));
        return *this;
      }

      sys_fs::directory_iterator iterator;
      sys_fs::directory_iterator end;
      std::function<filter_fn> filter;
    };

    inline bool operator ==(const filtered_iterator& lhs,
                            const filtered_iterator& rhs) {
      return lhs.iterator == rhs.iterator;
    }

    inline bool operator !=(const filtered_iterator& lhs,
                            const filtered_iterator& rhs) {
      return lhs.iterator != rhs.iterator;
    }

  }

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

      struct path_info {
        typedef sys_fs::path type;
        typedef fs::filtered_iterator iterator;
        typedef sys_fs::path reference;

        static bool has_sub_nodes (type const& n) {
          return sys_fs::is_directory(n);
        }

        static reference make_reference (type const& n) {
          return n;
        }

        static type const& dereference (reference const& r) {
          return r;
        }

        static std::string label (type const& n) {
          return n.filename().string();
        }

        static const draw::masked_bitmap& icon (type const&, bool has_children, bool is_open, bool selected) {
          return tree::standard_icon(has_children, is_open, selected);
        }
      };

      struct unsorted_path_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n) {
          return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)), [](const sys_fs::directory_entry& i) {
                          return ibr::string::starts_with(i.path().filename().string(), ".");
                       }),
                       fs::filtered_iterator(sys_fs::end(path_iterator(n))));
        }

      };

      struct unsorted_dir_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n) {
          return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)), [](const sys_fs::directory_entry& i) {
                         return !sys_fs::is_directory(i.path()) || ibr::string::starts_with(i.path().filename().string(), ".");
                       }),
                       fs::filtered_iterator(sys_fs::end(path_iterator(n))));
        }

      };

      struct unsorted_file_info : public path_info {
        typedef tree::range<iterator> range;

        static range sub_nodes (type const& n) {
          return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)), [](const sys_fs::directory_entry& i) {
                         return sys_fs::is_directory(i.path()) || ibr::string::starts_with(i.path().filename().string(), ".");
                       }),
                       fs::filtered_iterator(sys_fs::end(path_iterator(n))));
        }
      };

      struct sorted_path_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n) {
          range v;
          for (auto i = sys_fs::begin(path_iterator(n)),
                    e = sys_fs::end(path_iterator(n)); i != e; ++i) {
            const bool is_hidden = ibr::string::starts_with(i->path().filename().string(), ".");
            if (!is_hidden) {
              v.emplace_back(*i);
            }
          }
          std::sort(v.begin(), v.end(), [](type const& lhs, type const& rhs) -> bool {
            bool l_is_dir = sys_fs::is_directory(lhs);
            bool r_is_dir = sys_fs::is_directory(rhs);
            return ((l_is_dir == r_is_dir) && (lhs.filename() < rhs.filename())) || (l_is_dir > r_is_dir);
          });
          return v;
        }
      };

      struct sorted_dir_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n) {
          range v;
          for (auto i = sys_fs::begin(path_iterator(n)),
                    e = sys_fs::end(path_iterator(n)); i != e; ++i) {
            if (sys_fs::is_directory(i->path()) && !ibr::string::starts_with(i->path().filename().string(), ".")) {
              v.emplace_back(*i);
            }
          }
          std::sort(v.begin(), v.end(), [](type const& lhs, type const& rhs) -> bool {
            return (lhs.filename() < rhs.filename());
          });
          return v;
        }
      };

      struct sorted_file_info : public path_info {
        typedef std::vector<type> range;

        static range sub_nodes (type const& n) {
          range v;
          for (auto i = sys_fs::begin(path_iterator(n)),
                    e = sys_fs::end(path_iterator(n)); i != e; ++i) {
            if (!(sys_fs::is_directory(i->path()) || ibr::string::starts_with(i->path().filename().string(), "."))) {
              v.emplace_back(*i);
            }
          }
          std::sort(v.begin(), v.end(), [](type const& lhs, type const& rhs) -> bool {
            return (lhs.filename() < rhs.filename());
          });
          return v;
        }
      };


    } // tree

    // --------------------------------------------------------------------------
    typedef tree::tree<path_tree::unsorted_path_info> file_tree;

    typedef tree::tree<path_tree::sorted_path_info> sorted_file_tree;

    // --------------------------------------------------------------------------
    namespace detail {
      void init_file_list_layout (layout::weight_column_list_layout&);
      void init_file_list_header (column_list_header<layout::weight_column_list_layout>&);

      typedef column_list_row_drawer_t<layout::weight_column_list_layout,
                                       const draw::masked_bitmap*, std::string, uintmax_t,
                                       sys_fs::file_time_type> file_list_row_drawer;

      file_list_row_drawer create_file_list_row_drawer();

      typedef column_list_row_t<const draw::masked_bitmap*, std::string, uintmax_t,
                                sys_fs::file_time_type> file_list_row;

      file_list_row build_file_list_row (const sys_fs::path&, bool selected);

    } // detail

    class file_list : public win::column_list_t<layout::weight_column_list_layout,
                                                const draw::masked_bitmap*, std::string, uintmax_t, sys_fs::file_time_type> {
    public:
      typedef win::column_list_t<layout::weight_column_list_layout,
                                 const draw::masked_bitmap*, std::string, uintmax_t, sys_fs::file_time_type> super;

      file_list (core::size_type item_size = 20,
                 os::color background = color::white,
                 bool grab_focus = true)
        : super(item_size, background, grab_focus)
      {
        detail::init_file_list_layout(super::get_column_layout());
        detail::init_file_list_header(super::header);
        super::set_drawer(detail::create_file_list_row_drawer());
        super::set_data([&](std::size_t i) {
          return detail::build_file_list_row(current_dir[i], (i == super::list.get_selection()));
        }, 0);
      }

      void set_path (const sys_fs::path& dir) {
        current_dir = path_tree::sorted_file_info::sub_nodes(dir);
        super::list.set_count(current_dir.size());
        super::list.clear_selection(event_source::logic);
        super::list.set_scroll_pos(0);
        super::redraw_later();
      }

    private:
      std::vector<sys_fs::path> current_dir;

    };

  } // win

} // gui

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
* @brief     C++ API: file tree view
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

    namespace tree {

      template<>
      struct node_info<sys_fs::path> {
        typedef sys_fs::path type;
        typedef fs::filtered_iterator iterator;
        typedef sys_fs::path reference;
      };

      template<>
      inline bool has_sub_nodes<sys_fs::path> (sys_fs::path const& n) {
        return is_directory(n);
      }

      inline sys_fs::directory_iterator path_iterator (sys_fs::path const& n) {
#ifdef WIN32
        return sys_fs::directory_iterator(n);
#endif // WIN32
#ifdef X11
        return sys_fs::directory_iterator(n, sys_fs::directory_options::skip_permission_denied);
#endif // X11
      }

      template<>
      inline range<sys_fs::path> sub_nodes (sys_fs::path const& n) {
        return range<sys_fs::path>(fs::filtered_iterator(sys_fs::begin(path_iterator(n)), [](const sys_fs::directory_entry& i) {
                              const bool is_hidden = ibr::string::starts_with(i.path().filename().string(), ".");
                              return is_hidden;
                           }),
                           fs::filtered_iterator(sys_fs::end(path_iterator(n))));
      }

      template<>
      inline node_info<sys_fs::path>::reference make_reference<sys_fs::path> (sys_fs::path const& n) {
        return n;
      }

      template<>
      inline const sys_fs::path& dereference<sys_fs::path> (node_info<sys_fs::path>::reference const& r) {
        return r;
      }

      template<>
      inline std::string label<sys_fs::path> (sys_fs::path const& n) {
        return n.filename().string();
      }

    } // tree

    // --------------------------------------------------------------------------
    template<int S = 20, os::color B = color::white>
    using file_tree = tree::tree<sys_fs::path, S, B>;

  } // win

} // gui

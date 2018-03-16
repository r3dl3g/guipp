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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/time_util.h>
#include <gui/draw/bitmap.h>
#include <gui/ctrl/file_tree.h>


namespace gui {

  namespace fs {

    filtered_iterator::filtered_iterator ()
    {}

    filtered_iterator::filtered_iterator (const sys_fs::directory_iterator& i, std::function<filter_fn> f)
      : iterator(i)
      , end()
      , filter(f)
    {
      init();
    }

    filtered_iterator::filtered_iterator (sys_fs::directory_iterator&& i, std::function<filter_fn> f)
      : iterator(std::move(i))
      , end()
      , filter(f)
    {
      init();
    }

    filtered_iterator::filtered_iterator (filtered_iterator&& rhs)
      : iterator(std::move(rhs.iterator))
      , end(std::move(rhs.end))
      , filter(std::move(rhs.filter))
    {
      init();
    }

    filtered_iterator::filtered_iterator (const filtered_iterator& rhs)
      : iterator(rhs.iterator)
      , end(rhs.end)
      , filter(rhs.filter)
    {
      init();
    }

    void filtered_iterator::init () {
      while ((iterator != end) && is_excluded(*iterator)) {
        ++iterator;
      }
    }

    bool filtered_iterator::is_excluded (const sys_fs::directory_entry& i) {
      return (filter && filter(i));
    }

    filtered_iterator& filtered_iterator::operator++ () {
      do {
        ++iterator;
      } while ((iterator != end) && is_excluded(*iterator));
      return *this;
    }

    filtered_iterator& filtered_iterator::operator++ (int) {
      do {
        ++iterator;
      } while ((iterator != end) && is_excluded(*iterator));
      return *this;
    }

    file_info::file_info (const sys_fs::path& path)
      : path(path)
      , status(sys_fs::status(path))
      , size(0)
    {
      if (!is_directory()) {
        size = sys_fs::file_size(path);
      }
      try {
        last_write_time = sys_fs::last_write_time(path);
      } catch (...) {}
    }

    file_info::file_info ()
      : size(0)
    {}

  } // namespace fs

  namespace ctrl {

    namespace path_tree {

      template<typename Comp>
      struct dirs_first {
        bool operator() (const fs::file_info& lhs, const fs::file_info& rhs) {
          bool l_is_dir = lhs.is_directory();
          bool r_is_dir = rhs.is_directory();
          return ((l_is_dir == r_is_dir) && cmp(lhs, rhs)) || (l_is_dir > r_is_dir);
        }

      private:
        Comp cmp;
      };

      template<bool up>
      struct comp_by_name {
        bool operator() (const fs::file_info& lhs, const fs::file_info& rhs) {
          return up == (lhs.filename() < rhs.filename());
        }
      };

      template<bool up>
      struct comp_by_date {
        bool operator() (const fs::file_info& lhs, const fs::file_info& rhs) {
          return up == (lhs.last_write_time < rhs.last_write_time);
        }
      };

      template<bool up>
      struct comp_by_size {
        bool operator() (const fs::file_info& lhs, const fs::file_info& rhs) {
          return up == (lhs.size < rhs.size);
        }
      };

      template<bool up>
      using comp_by_name_dirs_first = dirs_first<comp_by_name<up>>;

      template<bool up>
      using comp_by_date_dirs_first = dirs_first<comp_by_date<up>>;

      template<bool up>
      using comp_by_size_dirs_first = dirs_first<comp_by_size<up>>;

      auto unsorted_path_info::sub_nodes(type const & n)->range {
        return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)),
                                           [] (const sys_fs::directory_entry & i) {
           return basepp::string::starts_with(i.path().filename().string(), ".");
         }),
         fs::filtered_iterator(sys_fs::end(path_iterator(n))));
      }

      auto unsorted_dir_info::sub_nodes(type const & n)->range {
        return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)),
                                           [] (const sys_fs::directory_entry & i) {
          return !sys_fs::is_directory(i.path()) || basepp::string::starts_with(i.path().filename().string(), ".");
        }),
        fs::filtered_iterator(sys_fs::end(path_iterator(n))));
      }

      auto unsorted_file_info::sub_nodes(type const & n)->range {
        return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)),
                                           [] (const sys_fs::directory_entry & i) {
          return sys_fs::is_directory(i.path()) || basepp::string::starts_with(i.path().filename().string(), ".");
        }),
        fs::filtered_iterator(sys_fs::end(path_iterator(n))));
      }

      auto sorted_path_info::sub_nodes(type const & n)->range {
        range v;
        for (auto i = sys_fs::begin(path_iterator(n)), e = sys_fs::end(path_iterator(n)); i != e; ++i) {
          const bool is_hidden = basepp::string::starts_with(i->path().filename().string(), ".");
          if (!is_hidden) {
            v.emplace_back(*i);
          }
        }
        std::sort(v.begin(), v.end(), comp_by_name_dirs_first<true>());
        return v;
      }

      auto sorted_dir_info::sub_nodes(type const & n)->range {
        range v;
        for (auto i = sys_fs::begin(path_iterator(n)),
             e = sys_fs::end(path_iterator(n)); i != e; ++i) {
          if (sys_fs::is_directory(i->path()) && !basepp::string::starts_with(i->path().filename().string(), ".")) {
            v.emplace_back(*i);
          }
        }
        std::sort(v.begin(), v.end(), comp_by_name<true>());
        return v;
      }

      auto sorted_file_info::sub_nodes(type const & n)->range {
        range v;
        for (auto i = sys_fs::begin(path_iterator(n)),
             e = sys_fs::end(path_iterator(n)); i != e; ++i) {
          if (!(sys_fs::is_directory(i->path()) || basepp::string::starts_with(i->path().filename().string(), "."))) {
            v.emplace_back(*i);
          }
        }
        std::sort(v.begin(), v.end(), comp_by_name<true>());
        return v;
      }


    } // path_tree

    namespace detail {

      void init_file_list_layout (layout::weight_column_list_layout& lay) {
        lay.set_columns({
          layout::weight_column_info {24, text_origin::center, 24, 0.0F},
          layout::weight_column_info {120, text_origin::vcenter_left, 20, 1.0F},
          layout::weight_column_info {80, text_origin::vcenter_right, 20, 0.0F},
          layout::weight_column_info {120, text_origin::vcenter_right, 20, 0.001F}
        }, false);
      }

      std::string format_file_size (uintmax_t s) {
        if (s < 1000) {
          return ostreamfmt(s);
        }
        if (s < 1000000) {
          return ostreamfmt(s / 1000 << " KB");
        }
        if (s < 1000000000) {
          return ostreamfmt(s / 1000000 << " MB");
        }
        if (s < 1000000000000) {
          return ostreamfmt(s / 1000000000 << " GB");
        }
        return ostreamfmt(s / 1000000000000 << " TB");
      }

      file_list_row_drawer create_file_list_row_drawer () {
        return file_list_row_drawer {
          [] (const draw::masked_bitmap* const& img, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin) {
            if (img) {
              g.fill(draw::image<draw::masked_bitmap>(*img, r), s ? color::highLightColor() : b);
            } else {
              g.fill(draw::rectangle(r), s ? color::highLightColor() : b);
            }
            draw::frame::lines(g, r);
          },
          [] (const fs::file_info& path, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin align) {
            paint::text_item(g, r, b, path.filename(), s, align);
            draw::frame::lines(g, r);
          },
          [] (const fs::file_info& path, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin align) {
            if (path.is_directory()) {
              paint::text_item(g, r, b, std::string(), s, align);
            } else {
              paint::text_item(g, r, b, format_file_size(path.size), s, align);
            }
            draw::frame::lines(g, r);
          },
          [] (const sys_fs::file_time_type& tp, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin align) {
            paint::text_item(g, r, b, basepp::time::format_time(tp), s, align);
            draw::frame::lines(g, r);
          }
        };
      }

      file_list_row build_file_list_row (const fs::file_info& f, bool selected) {
        const draw::masked_bitmap* img = nullptr;
        if (f.is_directory()) {
          img = &(tree::closed_folder_icon(selected));
        } else {
          img = &(tree::file_icon(selected));
        }

        return std::make_tuple(img, f, f, f.last_write_time);
      }

    } // detail

    template<sort_order O>
    void sort_by (std::vector<fs::file_info>& list);

    template<>
    void sort_by<sort_order::none> (std::vector<fs::file_info>&) {
    }

    template<>
    void sort_by<sort_order::name_down> (std::vector<fs::file_info>& v) {
      std::stable_sort(std::begin(v), std::end(v), path_tree::comp_by_name_dirs_first<false>());
    }

    template<>
    void sort_by<sort_order::name_up> (std::vector<fs::file_info>& v) {
      std::stable_sort(std::begin(v), std::end(v), path_tree::comp_by_name_dirs_first<true>());
    }

    template<>
    void sort_by<sort_order::size_down> (std::vector<fs::file_info>& v) {
      std::stable_sort(std::begin(v), std::end(v), path_tree::comp_by_size_dirs_first<false>());
    }

    template<>
    void sort_by<sort_order::size_up> (std::vector<fs::file_info>& v) {
      std::stable_sort(std::begin(v), std::end(v), path_tree::comp_by_size_dirs_first<true>());
    }

    template<>
    void sort_by<sort_order::date_down> (std::vector<fs::file_info>& v) {
      std::stable_sort(std::begin(v), std::end(v), path_tree::comp_by_date_dirs_first<false>());
    }

    template<>
    void sort_by<sort_order::date_up> (std::vector<fs::file_info>& v) {
      std::stable_sort(std::begin(v), std::end(v), path_tree::comp_by_date_dirs_first<true>());
    }

    void sort_list_by (std::vector<fs::file_info>& list, sort_order order) {
      switch (order) {
        case sort_order::name_down: sort_by<sort_order::name_down>(list); break;
        case sort_order::name_up: sort_by<sort_order::name_up>(list); break;
        case sort_order::size_down: sort_by<sort_order::size_down>(list); break;
        case sort_order::size_up: sort_by<sort_order::size_up>(list); break;
        case sort_order::date_down: sort_by<sort_order::date_down>(list); break;
        case sort_order::date_up: sort_by<sort_order::date_up>(list); break;
      }
    }

    void draw_arrow_up (const draw::graphics& g, const core::rectangle& r, os::color col) {
      g.fill(draw::polygon({{r.center_x(), r.y()}, r.bottom_right(), r.bottom_left()}), col);
    }

    void draw_arrow_down (const draw::graphics& g, const core::rectangle& r, os::color col) {
      g.fill(draw::polygon({r.top_left(), r.top_right(), {r.center_x(), r.y2()}}), col);
    }


  } // ctrl

} // gui

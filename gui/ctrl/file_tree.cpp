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
#include <gui/core/time_util.h>
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
      return (filter && filter(*iterator));
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

  } // namespace fs

  namespace win {

    namespace path_tree {

      auto unsorted_path_info::sub_nodes(type const & n)->range {
        return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)),
                                           [] (const sys_fs::directory_entry & i) {
           return string::starts_with(i.path().filename().string(), ".");
         }),
         fs::filtered_iterator(sys_fs::end(path_iterator(n))));
      }

      auto unsorted_dir_info::sub_nodes(type const & n)->range {
        return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)),
                                           [] (const sys_fs::directory_entry & i) {
          return !sys_fs::is_directory(i.path()) || string::starts_with(i.path().filename().string(), ".");
        }),
        fs::filtered_iterator(sys_fs::end(path_iterator(n))));
      }

      auto unsorted_file_info::sub_nodes(type const & n)->range {
        return range(fs::filtered_iterator(sys_fs::begin(path_iterator(n)),
                                           [] (const sys_fs::directory_entry & i) {
          return sys_fs::is_directory(i.path()) || string::starts_with(i.path().filename().string(), ".");
        }),
        fs::filtered_iterator(sys_fs::end(path_iterator(n))));
      }

      auto sorted_path_info::sub_nodes(type const & n)->range {
        range v;
        for (auto i = sys_fs::begin(path_iterator(n)), e = sys_fs::end(path_iterator(n)); i != e; ++i) {
          const bool is_hidden = string::starts_with(i->path().filename().string(), ".");
          if (!is_hidden) {
            v.emplace_back(*i);
          }
        }
        std::sort(v.begin(), v.end(), [] (type const & lhs, type const & rhs)->bool {
                    bool l_is_dir = sys_fs::is_directory(lhs);
                    bool r_is_dir = sys_fs::is_directory(rhs);
                    return ((l_is_dir == r_is_dir) && (lhs.filename() < rhs.filename())) || (l_is_dir > r_is_dir);
                  });
        return v;
      }

      auto sorted_dir_info::sub_nodes(type const & n)->range {
        range v;
        for (auto i = sys_fs::begin(path_iterator(n)),
             e = sys_fs::end(path_iterator(n)); i != e; ++i) {
          if (sys_fs::is_directory(i->path()) && !string::starts_with(i->path().filename().string(), ".")) {
            v.emplace_back(*i);
          }
        }
        std::sort(v.begin(), v.end(), [] (type const & lhs, type const & rhs)->bool {
                    return (lhs.filename() < rhs.filename());
                  });
        return v;
      }

      auto sorted_file_info::sub_nodes(type const & n)->range {
        range v;
        for (auto i = sys_fs::begin(path_iterator(n)),
             e = sys_fs::end(path_iterator(n)); i != e; ++i) {
          if (!(sys_fs::is_directory(i->path()) || string::starts_with(i->path().filename().string(), "."))) {
            v.emplace_back(*i);
          }
        }
        std::sort(v.begin(), v.end(), [] (type const & lhs, type const & rhs)->bool {
                    return (lhs.filename() < rhs.filename());
                  });
        return v;
      }

    } // path_tree

    namespace detail {

      void init_file_list_layout (layout::weight_column_list_layout& lay) {
        lay.set_columns({
                          layout::weight_column_info {24, text_origin::center, 24, 0.0F},
                          layout::weight_column_info {120, text_origin::vcenter_left, 20, 1.0F},
                          layout::weight_column_info {60, text_origin::vcenter_right, 20, 0.0F},
                          layout::weight_column_info {100, text_origin::vcenter_right, 20, 0.1F}
                        }, false);
      }

      void init_file_list_header (column_list_header<layout::weight_column_list_layout>& header) {
        header.set_cell_drawer([] (std::size_t i, const draw::graphics & g, const core::rectangle & r, const draw::brush & background) {
                                 static std::string title[] = {"", "Name", "Size", "Changed"};
                                 g.fill(draw::rectangle(r), background);
                                 draw::frame::raised_relief(g, r);
                                 g.text(draw::text_box(title[i], r, text_origin::center), draw::font::system(), color::windowTextColor());
                               });
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
          [] (const sys_fs::path& path, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin align) {
            win::paint::text_item(g, r, b, path.filename().string(), s, align);
            draw::frame::lines(g, r);
          },
          [] (const sys_fs::path& path, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin align) {
            if (sys_fs::is_directory(path)) {
              win::paint::text_item(g, r, b, std::string(), s, align);
            } else {
              win::paint::text_item(g, r, b, format_file_size(sys_fs::file_size(path)), s, align);
            }
            draw::frame::lines(g, r);
          },
          [] (const sys_fs::file_time_type& tp, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin align) {
            win::paint::text_item(g, r, b, time::format_time(tp), s, align);
            draw::frame::lines(g, r);
          }
        };
      }

      file_list_row build_file_list_row (const sys_fs::path& f, bool selected) {
        sys_fs::file_time_type lwt = sys_fs::file_time_type();
        try {
          lwt = sys_fs::last_write_time(f);
        } catch (...) {}

        const draw::masked_bitmap* img = nullptr;
        if (sys_fs::is_directory(f)) {
          img = &(tree::closed_folder_icon(selected));
        } else {
          img = &(tree::file_icon(selected));
        }

        return std::make_tuple(img, f, f, lwt);
      }

    } // detail

  } // win

} // gui

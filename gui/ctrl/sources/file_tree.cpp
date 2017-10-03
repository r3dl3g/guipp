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

#include "file_tree.h"
#include "time_util.h"
#include "bitmap.h"


namespace gui {

  namespace win {

    namespace detail {

      void init_file_list_layout (layout::weight_column_list_layout& lay) {
        lay.set_columns({
          layout::weight_column_info{ 24, text_origin::center, 24, 0.0F },
          layout::weight_column_info{ 120, text_origin::vcenter_left, 20, 1.0F },
          layout::weight_column_info{ 60, text_origin::vcenter_right, 20, 0.0F },
          layout::weight_column_info{ 100, text_origin::vcenter_right, 20, 0.1F }
        }, false);
      }

      void init_file_list_header (column_list_header<layout::weight_column_list_layout>& header) {
        header.set_cell_drawer([](std::size_t i, const draw::graphics& g, const core::rectangle& r, const draw::brush& background) {
          static std::string title[] = { "", "Name", "Size", "Changed" };
          g.fill(draw::rectangle(r), background);
          draw::frame::raised_relief(g, r);
          g.text(draw::text_box(title[i], r, text_origin::center), draw::font::system(), color::windowTextColor());
        });
      }

      file_list_row_drawer create_file_list_row_drawer () {
        return file_list_row_drawer {
          [](const draw::masked_bitmap* const& img, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin) {
            if (img) {
              g.fill(draw::image<draw::masked_bitmap>(*img, r), s ? color::highLightColor() : b);
            } else {
              g.fill(draw::rectangle(r), s ? color::highLightColor() : b);
            }
            draw::frame::lines(g, r);
        },
          win::cell_drawer<std::string, draw::frame::lines>,
          win::cell_drawer<uintmax_t, draw::frame::lines>,
          [](const sys_fs::file_time_type& tp, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, bool, text_origin align) {
            win::paint::text_item(g, r, b, ibr::time::format_time(tp), s, align);
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
        uintmax_t fs = uintmax_t();
        if (sys_fs::is_directory(f)) {
          img = &(tree::closed_folder_icon(selected));
        } else {
          img = &(tree::file_icon(selected));
          try {
            fs = sys_fs::file_size(f);
          } catch (...) {}
        }

        return std::make_tuple(img, f.filename().string(), fs, lwt);
      }

    } // detail

  } // win

} // gui

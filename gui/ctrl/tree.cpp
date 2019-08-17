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
 * @brief     C++ API: tree control
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/bitmap.h>
#include <gui/io/pnm.h>
#include <gui/ctrl/tree.h>


namespace gui {

  namespace image_data {

#include <gui/ctrl/res/file_icon.h>
#include <gui/ctrl/res/close_folder_icon.h>
#include <gui/ctrl/res/open_folder_icon.h>

  } // image_data

  namespace ctrl {

    namespace paint {

      void tree_button (const draw::graphics& graph,
                        const core::rectangle& area,
                        bool is_open) {

        core::rectangle r(area.center() - core::size(4, 4), core::size(8, 8));
        if (!r.empty()) {
          std::vector<core::point> p;
          if (is_open) {
            core::point::type y = r.center_y() - 2;
            core::point::type y2 = y + 4;
            p = {core::point(r.x(), y), core::point(r.x2(), y), {r.center_x(), y2}};
          } else {
            core::point::type x = r.center_x() - 2;
            core::point::type x2 = x + 4;
            p = {core::point(x, r.y()), core::point(x, r.y2()), {x2, r.center_y()}};
          }
          graph.fill(draw::polygon(p), color::black);
        }
      }

      void tree_node (const draw::graphics& graph,
                      const core::rectangle& area,
                      const draw::brush& background,
                      std::size_t depth,
                      const std::string& label,
                      const draw::masked_bitmap& icon,
                      bool has_children,
                      bool is_open,
                      bool selected,
                      bool hilited) {
        if (selected) {
          graph.fill(draw::rectangle(area), color::highLightColor());
        } else if (hilited) {
          graph.fill(draw::rectangle(area), color::buttonHighLightColor());
        } else {
          graph.fill(draw::rectangle(area), background);
        }

        core::rectangle r = area + core::point(core::point::type(depth * 16), 0);

        if (has_children) {
          r.width(16);
          tree_button(graph, r, is_open);
        }

        r += core::point(16, 0);

        if (icon) {
          core::size sz = icon.image.size();
          core::point::type y = r.y() + (r.height() - core::global::unscale(sz.height())) / 2;
          graph.copy_from(icon, core::point(r.x(), y));
          r += core::point(core::global::unscale(sz.width()) + 5, 0);
        }

        r.x2(area.x2());
        os::color col = selected ? color::highLightTextColor()
                        : color::black;
        graph.text(draw::text_box(label, r, text_origin::vcenter_left), draw::font::system(), col);
      }

    } // paint

    namespace detail {

      enum class tree_icon {
        file,
        closed_folder,
        open_folder
      };

      template<typename T, size_t N>
      inline std::string make_string (T(&t)[N]) {
        return std::string((const char*)t, N);
      }

      std::string get_icon_chars (tree_icon type) {
        switch (type) {
        case tree_icon::file:           return make_string(image_data::file_icon_bits);
        case tree_icon::closed_folder:  return make_string(image_data::close_folder_icon_bits);
        case tree_icon::open_folder:    return make_string(image_data::open_folder_icon_bits);
        }
        return std::string();
      }

      draw::masked_bitmap build_tree_icon (tree_icon type, bool selected) {
        draw::bwmap mask;
        std::istringstream in(get_icon_chars(type));
        io::load_pnm(in, mask);

        if (core::global::get_scale_factor() != 1.0) {
          draw::bwmap rhs = mask;
          mask.create(rhs.size());  // size() will be scaled by reading
          mask.stretch_from(rhs);
        }
#ifdef WIN32
        mask.invert();
#endif // WIN32
        draw::pixmap icon(mask);
        if (!selected) {
          icon.invert();
        }
        return draw::masked_bitmap(icon, mask);
      }

    } // detail


    namespace tree {

      const draw::masked_bitmap& open_folder_icon (bool selected) {
        static draw::masked_bitmap icon(detail::build_tree_icon(detail::tree_icon::open_folder, false));
        static draw::masked_bitmap icon_selected(detail::build_tree_icon(detail::tree_icon::open_folder, true));
        return selected ? icon_selected : icon;
      }

      const draw::masked_bitmap& closed_folder_icon (bool selected) {
        static draw::masked_bitmap icon(detail::build_tree_icon(detail::tree_icon::closed_folder, false));
        static draw::masked_bitmap icon_selected(detail::build_tree_icon(detail::tree_icon::closed_folder, true));
        return selected ? icon_selected : icon;
      }

      const draw::masked_bitmap& file_icon (bool selected) {
        static draw::masked_bitmap icon(detail::build_tree_icon(detail::tree_icon::file, false));
        static draw::masked_bitmap icon_selected(detail::build_tree_icon(detail::tree_icon::file, true));
        return selected ? icon_selected : icon;
      }

      const draw::masked_bitmap& standard_icon (bool has_children, bool is_open, bool selected) {
        if (has_children) {
          return is_open ? open_folder_icon(selected) : closed_folder_icon(selected);
        }
        return file_icon(selected);
      }

    } // tree

  } // ctrl

} // gui

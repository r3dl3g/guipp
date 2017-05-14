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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "tree.h"
#include "bitmap.h"
#include "pnm.h"


namespace gui {

  namespace win {

    namespace paint {

      void tree_button (const draw::graphics& graph,
                        const core::rectangle& area,
                        bool is_open) {

        core::rectangle r(area.center() - core::size(4, 4), core::size(8, 8));
        if (!r.empty()) {
          std::vector<core::point> p;
          if (is_open) {
            core::point_type y = r.center_y() - 2;
            core::point_type y2 = y + 4;
            p = { core::point(r.x(), y), core::point(r.x2(), y), {r.center_x(), y2} };
          } else {
            core::point_type x = r.center_x() - 2;
            core::point_type x2 = x + 4;
            p = { core::point(x, r.y()), core::point(x, r.y2()), {x2, r.center_y()} };
          }
          graph.fill(draw::polygon(p), color::black);
        }
      }

      void tree_node (const draw::graphics& graph,
                      const core::rectangle& area,
                      const draw::brush& background,
                      int depth,
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

        core::rectangle r = area + core::point(core::point_type(depth * 16), 0);

        if (has_children) {
          r.width(16);
          tree_button(graph, r, is_open);
        }

        r += core::point(16, 0);

        if (icon) {
          core::size sz = icon.image.size();
          core::point_type y = r.y() + (r.height() - sz.height()) / 2;
          graph.copy_from(icon, core::point(r.x(), y));
          r += core::point(sz.width() + 5, 0);
        }

        r.x2(area.x2());
        os::color col = selected ? color::highLightTextColor()
                                 : color::black;
        graph.text(draw::text_box(label, r, text_origin::vcenter_left), draw::font::system(), col);
      }

    } // paint


    namespace detail {

      namespace image_data {

      static const byte file_icon_bits[] = {
       'P', '4', '\n', '2', '0',' ', '2', '0', '\t',
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00011111, 0b10000000, 0b00000000,
       0b00010000, 0b11000000, 0b00000000,
       0b00010000, 0b10100000, 0b00000000,
       0b00010000, 0b11110000, 0b00000000,
       0b00010000, 0b00010000, 0b00000000,
       0b00010000, 0b00010000, 0b00000000,
       0b00010000, 0b00010000, 0b00000000,
       0b00010000, 0b00010000, 0b00000000,
       0b00010000, 0b00010000, 0b00000000,
       0b00010000, 0b00010000, 0b00000000,
       0b00010000, 0b00010000, 0b00000000,
       0b00010000, 0b00010000, 0b00000000,
       0b00011111, 0b11110000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000};

      static const std::string file_icon_data((const char*)file_icon_bits, sizeof(file_icon_bits));

      static const byte close_folder_bits[] = {
       'P', '4', '\n', '2', '0',' ', '2', '0', '\t',
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00011111, 0b00000000, 0b00000000,
       0b00100000, 0b10000000, 0b00000000,
       0b11111111, 0b11111111, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b11111111, 0b11111111, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000};

      static const std::string close_folder_data((const char*)close_folder_bits, sizeof(close_folder_bits));

      static const byte open_folder_bits[] = {
       'P', '4', '\n', '2', '0',' ', '2', '0', '\t',
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00011111, 0b00000000, 0b00000000,
       0b00100000, 0b10000000, 0b00000000,
       0b11100000, 0b11111111, 0b00000000,
       0b10000000, 0b00000001, 0b00000000,
       0b10001111, 0b11111111, 0b11110000,
       0b10001000, 0b00000000, 0b00010000,
       0b10010000, 0b00000000, 0b00100000,
       0b10010000, 0b00000000, 0b00100000,
       0b10100000, 0b00000000, 0b01000000,
       0b10100000, 0b00000000, 0b01000000,
       0b11000000, 0b00000000, 0b10000000,
       0b11000000, 0b00000000, 0b10000000,
       0b11111111, 0b11111111, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000,
       0b00000000, 0b00000000, 0b00000000};

      static const std::string open_folder_data((const char*)open_folder_bits, sizeof(open_folder_bits));

      } // image_data

      enum class tree_icon {
        file,
        closed_folder,
        open_folder
      };

      const std::string& get_icon_chars(tree_icon type) {
        switch (type) {
          case tree_icon::file:           return image_data::file_icon_data;
          case tree_icon::closed_folder:  return image_data::close_folder_data;
          case tree_icon::open_folder:    return image_data::open_folder_data;
        }
        static std::string dummy;
        return dummy;
      }

      draw::masked_bitmap build_tree_icon (tree_icon type, bool selected) {
        draw::maskmap mask;
        std::istringstream in(get_icon_chars(type));
        io::load_pnm(in, mask);

        mask.invert();
        draw::memmap icon(mask.size());
        icon = mask;
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

    } // tree

  } // win

} // gui

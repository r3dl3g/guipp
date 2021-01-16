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
 * @brief     C++ API: tree control
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/io/pnm.h>
#include <gui/ctrl/look/control.h>
#include <gui/ctrl/look/tree.h>


namespace gui {

  namespace image_data {

#include <gui/ctrl/look/res/file_icon.h>
#include <gui/ctrl/look/res/close_folder_icon.h>
#include <gui/ctrl/look/res/open_folder_icon.h>

  } // image_data

  namespace detail {

    enum class icon_type {
      file,
      closed_folder,
      open_folder
    };

    template<typename T, size_t N>
    inline std::string make_string (T(&t)[N]) {
      return std::string((const char*)t, N);
    }

    std::string get_icon_chars (icon_type type) {
      switch (type) {
      case icon_type::file:           return make_string(image_data::file_icon_bits);
      case icon_type::closed_folder:  return make_string(image_data::close_folder_icon_bits);
      case icon_type::open_folder:    return make_string(image_data::open_folder_icon_bits);
      }
      return std::string();
    }

    tree::tree_icon build_tree_icon (icon_type type, bool /*selected*/) {
      using namespace gui::draw;
      bwmap mask;
      std::istringstream in(get_icon_chars(type));
      io::load_pnm(in, mask);
#ifdef GUIPP_WIN
      mask.invert();
#endif // GUIPP_WIN


      if (core::global::get_scale_factor() != 1.0) {
        bwmap rhs = mask;
        // Upscale the image. Douplicate the pixels by scale factor.
        mask.create(rhs.native_size() * core::global::get_scale_factor());
        mask.stretch_from(rhs);
      }

      return tree::tree_icon(bitmap(mask));
    }

  } // detail

  namespace tree {

    const tree_icon& open_folder_icon (bool selected) {
      static tree_icon icon(detail::build_tree_icon(detail::icon_type::open_folder, false));
      static tree_icon icon_selected(detail::build_tree_icon(detail::icon_type::open_folder, true));
      return selected ? icon_selected : icon;
    }

    const tree_icon& closed_folder_icon (bool selected) {
      static tree_icon icon(detail::build_tree_icon(detail::icon_type::closed_folder, false));
      static tree_icon icon_selected(detail::build_tree_icon(detail::icon_type::closed_folder, true));
      return selected ? icon_selected : icon;
    }

    const tree_icon& file_icon (bool selected) {
      static tree_icon icon(detail::build_tree_icon(detail::icon_type::file, false));
      static tree_icon icon_selected(detail::build_tree_icon(detail::icon_type::file, true));
      return selected ? icon_selected : icon;
    }

    const tree_icon& standard_icon (bool has_children, bool is_open, bool selected) {
      if (has_children) {
        return is_open ? open_folder_icon(selected) : closed_folder_icon(selected);
      }
      return file_icon(selected);
    }

  } // tree

  namespace look {

    void tree_button (const draw::graphics& graph,
                      const core::rectangle& area,
                      bool is_open,
                      bool is_disabled) {

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
        graph.fill(draw::polygon(p), is_disabled ? color::light_gray : color::black);
      }
    }

    void tree_node (const draw::graphics& graph,
                    const core::rectangle& area,
                    const draw::brush& background,
                    std::size_t depth,
                    const std::string& label,
                    const tree::tree_icon& icon,
                    bool has_children,
                    bool is_open,
                    ctrl::item_state state) {
      graph.fill(draw::rectangle(area), get_background_color(state, background.color()));

      core::rectangle r = area + core::point(core::point::type(depth * 16), 0);

      if (has_children) {
        r.width(16);
        tree_button(graph, r, is_open, state.is_disabled());
      }

      r += core::point(16, 0);

      if (icon) {
        auto sz = icon.native_size();
        core::native_point::type y = r.os_y() + (r.os_height() - sz.height()) / 2;
        graph.copy_from(icon, core::native_point(r.os_x(), y));
        r += core::point(core::global::scale_from_native<core::size::type>(sz.width()) + 5, 0);
      }

      r.x2(area.x2());
      graph.text(draw::text_box(label, r, text_origin_t::vcenter_left),
                 draw::font::system(), get_text_color(state));
    }

  } // look

} // gui

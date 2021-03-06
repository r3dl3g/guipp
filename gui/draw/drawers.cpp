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
 * @brief     C++ API: basic window types
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>

#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/arc_coords.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/draw/bitmap.h>
#include <gui/draw/use.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    template<>
    void arc_or_pie<arc_type::arc>::operator() (graphics& g,
                                                const pen& p) const {
      Use<brush> br(g, brush::invisible);
      draw_arc<arc_type::arc>(g, arc_coords(g.context(), rect, start_angle, end_angle), p);
    }

    template<>
    void arc_or_pie<arc_type::arc>::operator() (graphics& g,
                                                const brush& b) const {
      arc_or_pie<arc_type::arc>::operator()(g, pen(b.color()));
    }

    template<>
    void arc_or_pie<arc_type::arc>::operator() (graphics& g,
                                                const brush&,
                                                const pen& p) const {
      arc_or_pie<arc_type::arc>::operator()(g, p);
    }

    // --------------------------------------------------------------------------
    template<>
    void arc_or_pie<arc_type::pie>::operator() (graphics& g,
                                                const pen& p) const {
      Use<brush> br(g, brush::invisible);
      draw_arc<arc_type::pie>(g, arc_coords(g.context(), rect, start_angle, end_angle), p);
    }

    template<>
    void arc_or_pie<arc_type::pie>::operator() (graphics& g,
                                                const brush& b) const {
      arc_coords c(g.context(), rect, start_angle, end_angle);
      pen pn(b.color());
      Use<pen> upn(g, pn);
      fill_arc<arc_type::pie>(g, c, b);
      Use<brush> br(g, brush::invisible);
      draw_arc<arc_type::pie>(g, c, pn);
    }

    template<>
    void arc_or_pie<arc_type::pie>::operator() (graphics& g,
                                                const brush& b,
                                                const pen& p) const {
      arc_coords c(g.context(), rect, start_angle, end_angle);
      Use<pen> pn(g, pen::invisible);
      fill_arc<arc_type::pie>(g, c, b);
      Use<brush> br(g, brush::invisible);
      draw_arc<arc_type::pie>(g, c, p);
    }

    // --------------------------------------------------------------------------
    std::vector<os::point> polyline::convert (graphics& g) const {
      std::vector<os::point> pts;
      const auto sz = points.size();
      if (sz) {
        pts.reserve(points.size());
        for (const core::point& pt : points) {
          pts.push_back(pt.os(g.context()));
        }
      }
      return pts;
    }

    polyline::polyline (const std::vector<core::point>& pts)
      : points(pts)
    {}

    polyline::polyline (std::initializer_list<core::point> pts)
      : points(pts) {
    }

    // --------------------------------------------------------------------------
    std::vector<os::point> polygon::convert (graphics& g) const {
      std::vector<os::point> pts;
      const auto sz = points.size();
      if (sz) {
        pts.reserve(sz + 1);
        for (const core::point& pt : points) {
          pts.push_back(pt.os(g.context()));
        }
        pts.push_back(points[0].os(g.context()));
      }
      return pts;
    }

    polygon::polygon (const std::vector<core::point>& pts)
      : points(pts)
    {}

    polygon::polygon (std::initializer_list<core::point> pts)
      : points(pts)
    {}

    // --------------------------------------------------------------------------
    template<pixel_format_t px_fmt>
    void copy_frame_image (draw::const_image_data<px_fmt> src_img,
                           draw::image_data<px_fmt> dest_img,
                           const bitmap_info& src_bmi, const bitmap_info& dest_bmi,
                           const core::native_rect& frame) {

      const int32_t width = dest_bmi.width;
      const int32_t height = dest_bmi.height;

      const int32_t left = std::min(frame.x(), width);
      const int32_t right = std::min<int32_t>(frame.width(), width - left);
      const int32_t top = std::min(frame.y(), height);
      const int32_t bottom = std::min<int32_t>(frame.height(), height - top);

      const int32_t target_right = dest_bmi.width - right;
      const int32_t target_bottom = dest_bmi.height - bottom;
      const int32_t source_right = src_bmi.width - right;
      const int32_t source_bottom = src_bmi.height - bottom;

      using namespace convert;

      // top left
      if (top && left) {
        copy::sub<px_fmt>(src_img, dest_img, {0, 0}, {0, 0, static_cast<uint32_t>(left), static_cast<uint32_t>(top)});
      }

      // top right
      if (top && right) {
        copy::sub<px_fmt>(src_img, dest_img, {source_right, 0}, {target_right, 0, static_cast<uint32_t>(right), static_cast<uint32_t>(top)});
      }

      // bottom left
      if (bottom && left) {
        copy::sub<px_fmt>(src_img, dest_img, {0, source_bottom}, {0, target_bottom, static_cast<uint32_t>(left), static_cast<uint32_t>(bottom)});
      }

      if (bottom && right) {
        // bottom right
        copy::sub<px_fmt>(src_img, dest_img,
                       {source_right, source_bottom}, {target_right, target_bottom, static_cast<uint32_t>(right), static_cast<uint32_t>(bottom)});
      }

      if ((target_right >= left) && (target_bottom >= top) && (source_right >= left) && (source_bottom >= top)) {
        const uint32_t target_inner_width = static_cast<uint32_t>(std::max(0, target_right - left));
        const uint32_t target_inner_height = static_cast<uint32_t>(std::max(0, target_bottom - top));
        const uint32_t source_inner_width = static_cast<uint32_t>(std::max(0, source_right - left));
        const uint32_t source_inner_height = static_cast<uint32_t>(std::max(0, source_bottom - top));

        // top center
        if (top && target_inner_width) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {left, 0, source_inner_width, static_cast<uint32_t>(top)},
                               {left, 0, target_inner_width, static_cast<uint32_t>(top)});
        }

        // bottom center
        if (bottom && target_inner_width) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {left, source_bottom, source_inner_width, static_cast<uint32_t>(bottom)},
                               {left, target_bottom, target_inner_width, static_cast<uint32_t>(bottom)});
        }

        // left center
        if (left && target_inner_height) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {0, top, static_cast<uint32_t>(left), source_inner_height},
                               {0, top, static_cast<uint32_t>(left), target_inner_height});
        }

        // right center
        if (right && target_inner_height) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {source_right, top, static_cast<uint32_t>(right), source_inner_height},
                               {target_right, top, static_cast<uint32_t>(right), target_inner_height});
        }

        // center
        if (target_inner_width && target_inner_height) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {left, top, source_inner_width, source_inner_height},
                               {left, top, target_inner_width, target_inner_height});
        }
      }
    }

    template<pixel_format_t T>
    void draw_image_frame (graphics& g,
                           const core::point& pt,
                           const core::rectangle rect,
                           const datamap<T>& img,
                           const core::native_rect& frame) {
      if (rect.size() <= core::size::two) {
        return;
      }

      const auto width = core::roundup<uint32_t>(rect.os_width());
      const auto height = core::roundup<uint32_t>(rect.os_height());

      datamap<T> dest(width, height);
      copy_frame_image<T>(img.get_data(), dest.get_data(),
                          img.get_info(), dest.get_info(), frame);

      pixmap buffer;
      buffer = dest;
      g.copy_from(buffer, pt);
    }

    template<>
    void image_frame<pixel_format_t::BW>::operator() (graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<pixel_format_t::GRAY>::operator() (graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<pixel_format_t::RGB>::operator() (graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<pixel_format_t::RGBA>::operator() (graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

  }

}

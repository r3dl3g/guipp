/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: color definition
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <vector>
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>
#include <gui/core/angle.h>
#include <gui/draw/graphics.h>
#include <gui/draw/text_origin.h>
#include <gui/draw/gui++-draw-export.h>

namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT line {
      line (const core::point& from,
            const core::point& to);

      void operator() (graphics&, const pen&) const;

      const core::point& p0 () const;
      const core::point& p1 () const;

    private:
      const core::point from;
      const core::point to;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT rectangle {
      explicit rectangle (const core::rectangle& rect);
      rectangle (const core::point& pos,
                 const core::size& sz);
      rectangle (const core::point& pos1,
                 const core::point& pos2);

      void operator() (graphics&, const brush&, const pen&) const;
      void operator() (graphics&, const pen&) const;
      void operator() (graphics&, const brush&) const;

    private:
      const core::rectangle rect;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT ellipse {
      explicit ellipse (const core::rectangle& rect);
      ellipse (const core::point& pos,
               const core::size& sz);
      ellipse (const core::point& pos1,
               const core::point& pos2);

      void operator() (graphics&, const brush&, const pen&) const;
      void operator() (graphics&, const pen&) const;
      void operator() (graphics&, const brush&) const;

    private:
      const core::rectangle rect;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT round_rectangle {
      round_rectangle (const core::rectangle& rect, const core::size& radius);
      round_rectangle (const core::rectangle& rect, core::size::type radius);

      void operator() (graphics&, const brush&, const pen&) const;
      void operator() (graphics&, const pen&) const;
      void operator() (graphics&, const brush&) const;

    private:
      const core::rectangle rect;
      const core::size radius;
    };

    // --------------------------------------------------------------------------
    enum class arc_type {
      pie,
      arc
    };

    // --------------------------------------------------------------------------
    template<arc_type T>
    struct arc_or_pie {
      arc_or_pie (const core::point& center,
                  core::size::type radius,
                  const core::angle& start_angle,
                  const core::angle& end_angle);
      arc_or_pie (const core::point& center,
                  const core::size& radius,
                  const core::angle& start_angle,
                  const core::angle& end_angle);
      arc_or_pie (const core::rectangle& rect,
                  const core::angle& start_angle,
                  const core::angle& end_angle);

      void operator() (graphics&, const brush&, const pen&) const;
      void operator() (graphics&, const pen&) const;
      void operator() (graphics&, const brush&) const;

    private:
      const core::rectangle rect;
      const core::angle start_angle;
      const core::angle end_angle;

    };

    using pie = arc_or_pie<arc_type::pie>;
    using arc = arc_or_pie<arc_type::arc>;

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT polyline {
      explicit polyline (const std::vector<core::point>& pts);
      polyline (std::initializer_list<core::point> pts);

      void operator() (graphics&, const brush&, const pen&) const;
      void operator() (graphics&, const pen&) const;
      void operator() (graphics&, const brush&) const;

    protected:
      std::vector<os::point> convert (graphics&) const;

      std::vector<core::point> points;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT polygon {
      explicit polygon (const std::vector<core::point>& pts);
      polygon (std::initializer_list<core::point> pts);

      void operator() (graphics&, const brush&, const pen&) const;
      void operator() (graphics&, const pen&) const;
      void operator() (graphics&, const brush&) const;

    protected:
      std::vector<os::point> convert (graphics&) const;

      std::vector<core::point> points;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT text_box {
      text_box (const std::string& str,
                const core::rectangle& rect,
                text_origin_t origin = text_origin_t::top_left);

      void operator() (graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      const core::rectangle rect;
      const text_origin_t origin;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT bounding_box {
      bounding_box (const std::string& str,
                    core::rectangle& rect,
                    text_origin_t origin = text_origin_t::top_left);

      void operator() (graphics&, const font& font, os::color color) const;

      // Not implemented!
      void operator= (const bounding_box&) = delete;

    private:
      const std::string str;
      core::rectangle& rect;
      const text_origin_t origin;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT text {
      text (const std::string& str,
            const core::point& pos,
            text_origin_t origin = text_origin_t::top_left);

      void operator() (graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      const core::point pos;
      const text_origin_t origin;
    };

    // --------------------------------------------------------------------------
    template<typename I>
    struct image {
      image (const I& img,
             const core::rectangle& rect,
             text_origin_t origin = text_origin_t::center);

      void operator() (graphics& g, const brush& b) const;

    private:
      const I& img;
      const core::rectangle rect;
      const text_origin_t origin;
    };

    // --------------------------------------------------------------------------
    template<pixel_format_t T> class datamap;

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    struct image_frame {
      image_frame (const core::rectangle& r, const datamap<T>& img, const core::native_rect& frame);

      void operator() (graphics&, const core::point&) const;

      void operator= (image_frame&) = delete;

    private:
      const core::rectangle rect;
      const datamap<T>& img;
      const core::native_rect frame;
    };

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, const core::native_rect& frame);

    template<pixel_format_t T>
    image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, uint32_t edge);

    template<pixel_format_t T>
    image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, uint32_t horizontal, uint32_t vertical);

    template<pixel_format_t T>
    image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, int32_t left, int32_t top, uint32_t right, uint32_t bottom);

  } // draw

} // gui

#include <gui/draw/drawers.inl>

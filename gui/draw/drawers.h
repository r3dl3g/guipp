/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <gui/draw/text_origin.h>


namespace gui {

  namespace draw {

    class graphics;

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT line {
      line (const core::point& from,
            const core::point& to);

      void operator() (const graphics&, const pen&) const;

    private:
      core::point from;
      core::point to;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT rectangle {
      rectangle (const core::rectangle& rect);
      rectangle (const core::point& pos,
                 const core::size& sz);
      rectangle (const core::point& pos1,
                 const core::point& pos2);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::rectangle rect;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT ellipse {
      ellipse (const core::rectangle& rect);
      ellipse (const core::point& pos,
               const core::size& sz);
      ellipse (const core::point& pos1,
               const core::point& pos2);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::rectangle rect;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT round_rectangle {
      round_rectangle (const core::rectangle& rect, const core::size& size);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::rectangle rect;
      const core::size size;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT arc {
      arc (const core::point& pos, unsigned int radius, float startrad, float endrad);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::point pos;
      unsigned int radius;
      float start_radius;
      float end_radius;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT polygon {
      polygon (const std::vector<core::point>& pts);
      polygon (std::initializer_list<core::point> pts);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      std::vector<os::point> points;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT text_box {
      text_box (const std::string& str,
                const core::rectangle& rect,
                text_origin origin = text_origin::top_left);

      void operator() (const graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      const core::rectangle rect;
      const text_origin origin;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT bounding_box {
      bounding_box (const std::string& str,
                    core::rectangle& rect,
                    text_origin origin = text_origin::top_left);

      void operator() (const graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      core::rectangle& rect;
      const text_origin origin;

      // Not implemented!
      void operator= (const bounding_box&) = delete;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_EXPORT text {
      text (const std::string& str,
            const core::point& pos,
            text_origin origin = text_origin::top_left);

      void operator() (const graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      const core::point pos;
      const text_origin origin;
    };

    // --------------------------------------------------------------------------
    template<typename I>
    struct image {
      image (const I& img,
             const core::rectangle& rect,
             text_origin origin = text_origin::center);

      void operator() (const graphics& g, const brush& b) const;

    private:
      const I& img;
      const core::rectangle rect;
      const text_origin origin;
    };

    // --------------------------------------------------------------------------
    template<BPP T> class datamap;

    // --------------------------------------------------------------------------
    template<BPP T>
    struct image_frame {
      image_frame (const core::rectangle& r, const datamap<T>& img, const core::uint32_rect& frame);

      void operator() (const graphics&, const core::point&) const;

    private:
      const core::rectangle rect;
      const datamap<T>& img;
      const core::uint32_rect frame;

      void operator= (image_frame&) = delete;
    };

    // --------------------------------------------------------------------------
    template<BPP T>
    image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, const core::uint32_rect& frame);

    template<BPP T>
    image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, uint32_t edge);

    template<BPP T>
    image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, uint32_t horizontal, uint32_t vertical);

    template<BPP T>
    image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);

  } // draw

} // gui

#include <gui/draw/drawers.inl>

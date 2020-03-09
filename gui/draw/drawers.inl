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


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    inline line::line (const core::point& from,
                       const core::point& to)
      : from(from)
      , to(to)
    {}

    // --------------------------------------------------------------------------
    inline rectangle::rectangle (const core::rectangle& rect)
      : rect(rect)
    {}

    inline rectangle::rectangle (const core::point& pos,
                                 const core::size& sz)
      : rect(pos, sz)
    {}

    inline rectangle::rectangle (const core::point& pos1,
                                 const core::point& pos2)
      : rect(pos1, pos2)
    {}

    // --------------------------------------------------------------------------
    inline ellipse::ellipse (const core::rectangle& rect)
      : rect(rect)
    {}

    inline ellipse::ellipse (const core::point& pos,
                             const core::size& sz)
      : rect(pos, sz)
    {}

    inline ellipse::ellipse (const core::point& pos1,
                             const core::point& pos2)
      : rect(pos1, pos2)
    {}

    // --------------------------------------------------------------------------
    inline round_rectangle::round_rectangle (const core::rectangle& rect,
                                             const core::size& size)
      : rect(rect)
      , size(size)
    {}

    // --------------------------------------------------------------------------
    inline text_box::text_box (const std::string& str,
                               const core::rectangle& rect,
                               text_origin_t origin)
      : str(str)
      , rect(rect)
      , origin(origin)
    {}

    // --------------------------------------------------------------------------
    inline bounding_box::bounding_box (const std::string& str,
                                       core::rectangle& rect,
                                       text_origin_t origin)
      : str(str)
      , rect(rect)
      , origin(origin)
    {}

    // --------------------------------------------------------------------------
    inline text::text (const std::string& str,
                       const core::point& pos,
                       text_origin_t origin)
      : str(str)
      , pos(pos)
      , origin(origin)
    {}

    // --------------------------------------------------------------------------
    template<typename I>
    inline image<I>::image (const I& img,
                            const core::rectangle& rect,
                            text_origin_t origin)
      : img(img)
      , rect(rect)
      , origin(origin)
    {}

    template<typename I>
    inline void image<I>::operator() (const graphics& g, const brush& b) const {
      g.fill(draw::rectangle(rect), b);

      auto sz = img.scaled_size();
      core::point::type px = rect.x();
      core::point::type py = rect.y();

      if (origin_is_h_center(origin)) {
        px += (rect.width() - sz.width()) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.width() - sz.width();
      }
      if (origin_is_v_center(origin)) {
        py += (rect.height() - sz.height()) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.height() - sz.height();
      }
      g.copy_from(img, core::point(px, py));
    }

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    image_frame<T>::image_frame (const core::rectangle& rect, const datamap<T>& img, const core::native_rect& frame)
      : rect(rect)
      , img(img)
      , frame(frame)
    {}

    template<>
    GUIPP_DRAW_EXPORT void image_frame<pixel_format_t::BW>::operator() (const graphics& g, const core::point& pt) const;

    template<>
    GUIPP_DRAW_EXPORT void image_frame<pixel_format_t::GRAY>::operator() (const graphics& g, const core::point& pt) const;

    template<>
    GUIPP_DRAW_EXPORT void image_frame<pixel_format_t::RGB>::operator() (const graphics& g, const core::point& pt) const;

    template<>
    GUIPP_DRAW_EXPORT void image_frame<pixel_format_t::RGBA>::operator() (const graphics& g, const core::point& pt) const;

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, const core::native_rect& frame) {
      return image_frame<T>(r, img, frame);
    }

    template<pixel_format_t T>
    inline image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, uint32_t edge) {
      return image_frame<T>(r, img, {static_cast<int32_t>(edge), static_cast<int32_t>(edge), edge, edge});
    }

    template<pixel_format_t T>
    inline image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, uint32_t horizontal, uint32_t vertical) {
      return image_frame<T>(r, img, {static_cast<int32_t>(horizontal), static_cast<int32_t>(vertical), horizontal, vertical});
    }

    template<pixel_format_t T>
    inline image_frame<T> frame_image (const core::rectangle& r, const datamap<T>& img, int32_t left, int32_t top, uint32_t right, uint32_t bottom) {
      return image_frame<T>(r, img, {left, top, right, bottom});
    }

  } // draw

} // gui

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
#include <functional>
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>
#include <gui/core/color.h>
#include <gui/core/bind_method.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>


namespace gui {

  // --------------------------------------------------------------------------
  enum class text_origin : unsigned short;

  // --------------------------------------------------------------------------
  enum class placement : unsigned short {
    left            = IF_WIN32_ELSE(DT_LEFT, 0x0000),
    hcenter         = IF_WIN32_ELSE(DT_CENTER, 0x0001),
    right           = IF_WIN32_ELSE(DT_RIGHT, 0x0002),

    top             = IF_WIN32_ELSE(DT_TOP, 0x0000),
    vcenter         = IF_WIN32_ELSE(DT_VCENTER, 0x0004),
    bottom          = IF_WIN32_ELSE(DT_BOTTOM, 0x0008)
  };

  // --------------------------------------------------------------------------
  inline constexpr unsigned short operator| (placement lhs, placement rhs) {
    return (static_cast<unsigned short>(lhs) | static_cast<unsigned short>(rhs));
  }

  inline constexpr unsigned short operator| (unsigned short lhs, placement rhs) {
    return (lhs | static_cast<unsigned short>(rhs));
  }

  inline constexpr bool operator== (text_origin lhs, placement rhs) {
    return (static_cast<unsigned short>(lhs) == static_cast<unsigned short>(rhs));
  }

  // --------------------------------------------------------------------------
  enum class line_handling : unsigned short {
    wordbreak       = IF_WIN32_ELSE(DT_WORDBREAK, 0x0010),
    singleline      = IF_WIN32_ELSE(DT_SINGLELINE, 0x0020)
  };

  inline constexpr unsigned short operator| (unsigned short lhs, line_handling rhs) {
    return (lhs | static_cast<unsigned int>(rhs));
  }

  // --------------------------------------------------------------------------
  enum class text_origin : unsigned short {
    top_left        = placement::top     | placement::left    | line_handling::wordbreak,
    top_hcenter     = placement::top     | placement::hcenter | line_handling::wordbreak,
    top_right       = placement::top     | placement::right   | line_handling::wordbreak,

    bottom_left     = placement::bottom  | placement::left    | line_handling::singleline,
    bottom_hcenter  = placement::bottom  | placement::hcenter | line_handling::singleline,
    bottom_right    = placement::bottom  | placement::right   | line_handling::singleline,

    vcenter_left    = placement::vcenter | placement::left    | line_handling::singleline,
    vcenter_right   = placement::vcenter | placement::right   | line_handling::singleline,
    center          = placement::vcenter | placement::hcenter | line_handling::singleline,

    h_align_mask    = placement::left    | placement::hcenter | placement::right,
    v_align_mask    = placement::top     | placement::vcenter | placement::bottom,

    undefined       = 0xFFFF,
  };

  // --------------------------------------------------------------------------
  inline constexpr text_origin operator& (text_origin lhs, text_origin rhs) {
    return text_origin(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
  }

  inline constexpr text_origin operator| (text_origin lhs, text_origin rhs) {
    return text_origin(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
  }

  // --------------------------------------------------------------------------
  inline bool origin_is_h_center (text_origin origin) {
    return (origin & text_origin::h_align_mask) == placement::hcenter;
  }

  inline bool origin_is_left (text_origin origin) {
    return (origin & text_origin::h_align_mask) == placement::left;
  }

  inline bool origin_is_right (text_origin origin) {
    return (origin & text_origin::h_align_mask) == placement::right;
  }

  inline bool origin_is_v_center (text_origin origin) {
    return (origin & text_origin::v_align_mask) == placement::vcenter;
  }

  inline bool origin_is_top (text_origin origin) {
    return (origin & text_origin::v_align_mask) == placement::top;
  }

  inline bool origin_is_bottom (text_origin origin) {
    return (origin & text_origin::v_align_mask) == placement::bottom;
  }

  // --------------------------------------------------------------------------
  namespace draw {

    class graphics;
    class memmap;
    class bitmap;
    class masked_bitmap;

    typedef void (drawable) (const graphics&, const brush&, const pen&);
    typedef void (frameable) (const graphics&, const pen&);
    typedef void (fillable) (const graphics&, const brush&);
    typedef void (textable) (const graphics&, const font& font, os::color color);
    typedef void (copyable) (const graphics&, const core::point&);

    // --------------------------------------------------------------------------
    class graphics {
    public:
      graphics (os::drawable target, os::graphics gc);
      graphics (draw::memmap& target);
      graphics (const graphics&);
      ~graphics ();

      void operator= (const graphics&);

      const graphics& clear (os::color color) const;
      const graphics& draw_pixel (const core::point& pt,
                                  os::color color) const;

      os::color get_pixel (const core::point&) const;

      const graphics& draw_lines (std::initializer_list<core::point> points,
                                  const pen& pen) const;

      const graphics& frame (const std::function<frameable>&, const pen& pen) const;
      const graphics& fill (const std::function<fillable>&, const brush& brush) const;
      const graphics& draw (const std::function<drawable>&, const brush& brush, const pen& pen) const;
      const graphics& text (const std::function<textable>&, const font& font, os::color color) const;
      const graphics& copy (const std::function<copyable>&, const core::point&) const;

      const graphics& copy_from (const graphics&, const core::point& dest = core::point::zero) const;
      const graphics& copy_from (const graphics&, const core::rectangle& src, const core::point& dest = core::point::zero) const;

      const graphics& copy_from (const draw::bitmap&, const core::point& dest = core::point::zero) const;
      const graphics& copy_from (const draw::bitmap&, const core::rectangle& src, const core::point& dest) const;
      const graphics& copy_from (const draw::masked_bitmap&, const core::point& dest = core::point::zero) const;

      const graphics& copy_from (os::drawable, const core::rectangle& src,
                                 const core::point& dest = core::point::zero) const;
      const graphics& stretch_from (os::drawable, const core::rectangle& src,
                                    const core::rectangle& dest) const;

      void invert (const core::rectangle&) const;
      void flush () const;

      int depth () const;
      core::rectangle area () const;

      os::graphics os () const;
      operator os::graphics () const;
      operator os::drawable() const;

#ifdef X11
      operator XftDraw* () const;
#endif // X11

    protected:
#ifdef X11
      XftDraw* get_xft () const;
#endif // X11

      friend struct clip;
      void push_clipping (const core::rectangle&) const;
      void pop_clipping () const;
      void restore_clipping () const;

    private:
      void destroy ();

      os::graphics gc;
      os::drawable target;
      bool own_gc;
      bool ref_gc;
      mutable std::vector<IF_WIN32_ELSE(HRGN, os::rectangle)> clipping_stack;
    };

    // --------------------------------------------------------------------------
    class buffered_paint {
    public:
      typedef std::function<void (const draw::graphics&)> painter;

      buffered_paint (painter f);

      template<typename T, typename F>
      buffered_paint (T* t, F f);

      void operator() (const draw::graphics& g);

    private:
      painter p;
    };

    // --------------------------------------------------------------------------
    struct clip {
      clip (const graphics& g, const core::rectangle& r);
      ~clip ();

    private:
      const graphics& g;

      void operator= (clip&) = delete;
    };

    // --------------------------------------------------------------------------
    struct line {
      line (const core::point& from,
            const core::point& to);

      void operator() (const graphics&, const pen&) const;

    private:
      core::point from;
      core::point to;
    };

    // --------------------------------------------------------------------------
    struct rectangle {
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
    struct ellipse {
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
    struct round_rectangle {
      round_rectangle (const core::rectangle& rect, const core::size& size);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::rectangle rect;
      const core::size size;
    };

    // --------------------------------------------------------------------------
    struct arc {
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
    struct polygon {
      polygon (const std::vector<core::point>& pts);
      polygon (std::initializer_list<core::point> pts);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      std::vector<os::point> points;
    };

    // --------------------------------------------------------------------------
    struct text_box {
      text_box (const std::string& str,
                const core::rectangle& rect,
                text_origin origin = text_origin::top_left,
                bool clear_background = false);

      void operator() (const graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      const core::rectangle rect;
      const text_origin origin;
      bool clear_background;
    };

    // --------------------------------------------------------------------------
    struct bounding_box {
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
    struct text {
      text (const std::string& str,
            const core::point& pos,
            text_origin origin = text_origin::top_left,
            bool clear_background = false);

      void operator() (const graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      const core::point pos;
      const text_origin origin;
      bool clear_background;
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
    namespace frame {

      typedef void (drawer)(const draw::graphics&, const core::rectangle&);

      void no_frame (const draw::graphics&, const core::rectangle&);

      void black (const draw::graphics&, const core::rectangle&);
      void white (const draw::graphics&, const core::rectangle&);

      void dots (const draw::graphics&, const core::rectangle&);

      void lines (const draw::graphics&, const core::rectangle&);
      void vline (const draw::graphics&, const core::rectangle&);
      void hline (const draw::graphics&, const core::rectangle&);

      void vraise (const draw::graphics&, const core::rectangle&);
      void hraise (const draw::graphics&, const core::rectangle&);

      void vgroove (const draw::graphics&, const core::rectangle&);
      void hgroove (const draw::graphics&, const core::rectangle&);

      void raised_relief (const draw::graphics&, const core::rectangle&);
      void sunken_relief (const draw::graphics&, const core::rectangle&);

      void relief (const draw::graphics& g, const core::rectangle& r, bool sunken);

      void raised_deep_relief (const draw::graphics&, const core::rectangle&);
      void sunken_deep_relief (const draw::graphics&, const core::rectangle&);

      void deep_relief (const draw::graphics& g, const core::rectangle& r, bool sunken);

      // --------------------------------------------------------------------------
    } // frame

  } // draw

} // gui

#include <gui/draw/graphics.inl>

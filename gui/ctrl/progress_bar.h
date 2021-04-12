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
 * @brief     C++ API: label
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/look/progress_bar.h>
#include <gui/ctrl/label.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {

      class GUIPP_CTRL_EXPORT progress_bar_base : public label_base {
      public:
        typedef label_base super;
        typedef core::size::type type;

        explicit progress_bar_base (const std::string& = std::string());
        explicit progress_bar_base (const text_source&);

        progress_bar_base (const progress_bar_base& rhs) = default;
        progress_bar_base (progress_bar_base&& rhs) = default;

        void set_min (type mi);
        void set_max (type ma);
        void set_value (type v);
        void set_min_max (type mi, type ma);
        void set_min_max_value (type mi, type ma, type v);

        type get_min () const;
        type get_max () const;
        type get_value () const;

        void operator++ ();
        void operator++ (int);

      protected:
        struct data {
          data ();

          type min;
          type max;
          type value;
        } data;

      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin_t origin, draw::frame::drawer frame = draw::frame::no_frame>
    class basic_progress_bar : public detail::progress_bar_base {
    public:
      typedef detail::progress_bar_base super;

      basic_progress_bar (os::color bc = color::very_light_blue,
                          os::color fg = color::black,
                          os::color bg = color::very_light_gray);
      basic_progress_bar (const basic_progress_bar& rhs);
      basic_progress_bar (basic_progress_bar&& rhs) noexcept ;

      void paint (draw::graphics& graph);

    private:
      void init ();

      os::color bar_color;
      os::color foreground;

    };

    // --------------------------------------------------------------------------
    using progress_bar = basic_progress_bar<text_origin_t::center,
                                            draw::frame::sunken_relief>;

  } // ctrl

} // gui

#include <gui/ctrl/progress_bar.inl>

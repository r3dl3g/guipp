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
 * @brief     C++ API: label
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {
      class label_base : public gui::win::window {
      public:
        typedef window super;
        typedef no_erase_window_class<label_base> clazz;

        label_base (const std::string& = std::string());
        label_base (const text_source&);

        label_base (const label_base& rhs);
        label_base (label_base&& rhs);

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def);

        void create (const container& parent,
                     const text_source& txt,
                     const core::rectangle& place = core::rectangle::def);

        void create (const container& parent,
                     const std::string& txt,
                     const core::rectangle& place = core::rectangle::def);

        void set_text (const text_source&);
        void set_text (const std::string&);

        std::string get_text () const;

        template<typename T>
        void operator<< (const T& t);

        template<typename T>
        void operator>> (T& t) const;

        template<typename T>
        void operator>> (T&& t) const;

      protected:
        text_source text;

      };

    } // namespace detail

    // --------------------------------------------------------------------------
    namespace paint {

      void label (const draw::graphics& graph,
                  const core::rectangle& area,
                  const std::string& text,
                  os::color foreground,
                  os::color background,
                  text_origin origin);

    }

    template<text_origin alignment,
             draw::frame::drawer frame = draw::frame::no_frame,
             os::color foreground = color::black,
             os::color background = color::very_light_gray>
    class basic_label : public detail::label_base {
    public:
      typedef detail::label_base super;

      basic_label ();
      basic_label (const basic_label& rhs);
      basic_label (basic_label&& rhs);

      void paint (const draw::graphics& graph);

    private:
      void init ();

    };

    // --------------------------------------------------------------------------
    using label_left = basic_label<text_origin::vcenter_left,
                                   draw::frame::no_frame,
                                   color::black,
                                   color::very_light_gray>;

    using label = label_left;

    using label_right = basic_label<text_origin::vcenter_right,
                                    draw::frame::no_frame,
                                    color::black,
                                    color::very_light_gray>;

    using label_center = basic_label<text_origin::center,
                                     draw::frame::no_frame,
                                     color::black,
                                     color::very_light_gray>;

  } // win

} // gui

#include <gui/ctrl/label.inl>

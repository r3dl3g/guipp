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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/label.h>


//#define SHOW_TEXT_AREA

namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      label_base::label_base (const std::string& t)
        : text(const_text(t))
      {}

      label_base::label_base (const text_source& t)
        : text(t)
      {}

      label_base::label_base (const label_base& rhs)
        : super(rhs)
        , text(rhs.text)
      {}

      label_base::label_base (label_base&& rhs)
        : super(rhs)
      {
        std::swap(text, rhs.text);
      }

      void label_base::set_text (const std::string& t) {
        set_text(const_text(t));
      }

      void label_base::set_text (const text_source& t) {
        text = t;
        redraw();
      }

      std::string label_base::get_text () const {
        return text();
      }

    } // detail

    namespace paint {

      void label (const draw::graphics& graph,
                  const core::rectangle& area,
                  const std::string& text,
                  os::color foreground,
                  os::color background,
                  text_origin origin) {
        using namespace gui::draw;
        graph.fill(draw::rectangle(area), background);
        graph.text(draw::text_box(text, area, origin),
                   font::system(),
                   foreground);
#ifdef SHOW_TEXT_AREA
        graph.text(draw::bounding_box(text, area, origin), font::system(), color::black);
        graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
#endif // SHOW_TEXT_AREA
      }

    } //paint

  } // ctrl

} // gui

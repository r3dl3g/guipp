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
* @brief     C++ API: textbox controls
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "control.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace paint {

      void text_box (const draw::graphics& graph,
                     const core::rectangle& area,
                     const std::vector<std::string>& lines,
                     const draw::font& fnt,
                     os::color foreground,
                     os::color background,
                     const text_origin origin,
                     const core::range<core::position<int>>& selection,
                     const core::position<int>& cursor_pos,
                     const core::point& offset,
                     const bool has_focus);

    } // paint

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      class textbox_base : public window {
      public:
        typedef window super;
        typedef std::vector<std::string> strings;
        typedef strings::size_type size_type;

        typedef core::position<int> position;
        typedef core::range<position> range;

        void set_text (const std::string&);
        std::string get_text () const;

        inline size_type row_count () const {
          return data.lines.size();
        }

        void set_scroll_pos (const core::point& pos);
        void set_selection (const range&);
        void set_cursor_pos (const position&, bool shift = false);

        inline const core::point& get_scroll_pos () const {
          return data.offset;
        }

        inline const range& get_selection () const {
          return data.selection;
        }

        inline const position& get_cursor_pos () const {
          return data.cursor_pos;
        }

        void replace_selection (const std::string&);
        std::string get_text_in_range (const range&) const;
        std::string get_selected_text () const;

        core::rectangle get_virtual_place () const;

        void make_cursor_visible ();

      protected:
        position get_position_at_point (const core::point& pt) const;

        void erase_lines (int first, int last);
        void erase_line (int first);

        void notify_content_changed () const;
        void notify_selection_changed () const;

        struct data {
          data ()
            : font(draw::font::monospace())
            , last_mouse_point(core::point::undefined)
          {}

          strings lines;
          draw::font font;
          core::point offset;
          core::point last_mouse_point;
          position cursor_pos;
          range selection;
          mutable core::size virtual_size;
        } data;
      };

      // --------------------------------------------------------------------------
      class textbox : public textbox_base {
      public:
        typedef textbox_base super;

        void create (const container& parent,
                     const core::rectangle& r = core::rectangle::def) {
          window::create(clazz, parent, r);
        }

      private:
        static const no_erase_window_class clazz;
      };
      // --------------------------------------------------------------------------

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin alignment = text_origin::vcenter_left,
             void(F)(const draw::graphics&, const core::rectangle&) = draw::frame::no_frame,
             os::color foreground = color::black,
             os::color background = color::white>
    class textbox_t : public detail::textbox {
    public:
      typedef detail::textbox super;

      textbox_t () {
        register_event_handler(REGISTER_FUNCTION, paint_event(this, &textbox_t::handle_paint));
      }

      void handle_paint (const draw::graphics& graph) {
        const auto area = client_area();
        paint::text_box(graph, area, data.lines, data.font,
                        foreground, background, alignment,
                        data.selection, data.cursor_pos, data.offset, has_focus());
        F(graph, area);
      }

    };

    // --------------------------------------------------------------------------
    typedef textbox_t<> textbox;

  } // win

} // gui

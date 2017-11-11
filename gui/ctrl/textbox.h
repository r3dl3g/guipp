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
// Library includes
//
#include <gui/ctrl/control.h>


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
                     const core::range<core::basic_point<int> >& selection,
                     const core::basic_point<int>& cursor_pos,
                     const core::point& offset,
                     const bool has_focus);

    } // paint

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      class textbox_base : public window {
      public:
        typedef window super;
        typedef no_erase_window_class<textbox_base, cursor_type::ibeam> clazz;
        typedef std::vector<std::string> strings;
        typedef strings::size_type size_type;

        typedef core::basic_point<int> position;
        typedef core::range<position> range;

        void create (const container& parent,
                     const core::rectangle& r = core::rectangle::def);

        void create (const container& parent,
                     const std::string& txt,
                     const core::rectangle& place = core::rectangle::def);

        void set_text (const std::string&);
        std::string get_text () const;

        size_type row_count () const;

        void set_scroll_pos (const core::point& pos);
        void set_selection (const range&);
        void set_cursor_pos (const position&, bool shift = false);

        const core::point& get_scroll_pos () const;
        const range& get_selection () const;
        const position& get_cursor_pos () const;

        void replace_selection (const std::string&);
        std::string get_text_in_range (const range&) const;
        std::string get_selected_text () const;

        core::rectangle get_virtual_place () const;

        void make_cursor_visible ();
        void enable_select_by_mouse ();

        position find_prev_word (const position& pos);
        position find_next_word (const position& pos);

      protected:
        position get_position_at_point (const core::point& pt) const;

        void erase_lines (int first, int last);
        void erase_line (int first);

        void notify_content_changed () const;
        void notify_selection_changed () const;

        struct data {
          data ();

          strings lines;
          draw::font font;
          core::point offset;
          core::point last_mouse_point;
          position cursor_pos;
          range selection;
          mutable core::size virtual_size;
        } data;

      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin align = text_origin::vcenter_left,
             draw::frame::drawer frame = draw::frame::no_frame,
             os::color foreground = color::black,
             os::color background = color::white>
    class basic_textbox : public detail::textbox_base {
    public:
      typedef detail::textbox_base super;

      basic_textbox ();

    private:
      void handle_paint (const draw::graphics& graph);

    };

    // --------------------------------------------------------------------------
    using textbox = basic_textbox<>;

    // --------------------------------------------------------------------------
    // inlines
    namespace detail {

      inline void textbox_base::create (const container& parent,
                                        const core::rectangle& r) {
        window::create(clazz::get(), parent, r);
      }

      inline void textbox_base::create (const container& parent,
                                        const std::string& txt,
                                        const core::rectangle& place) {
        create(parent, place);
        set_text(txt);
      }

      inline auto textbox_base::row_count() const-> size_type {
        return data.lines.size();
      }

      inline const core::point& textbox_base::get_scroll_pos () const {
        return data.offset;
      }

      inline auto textbox_base::get_selection() const->const range& {
        return data.selection;
      }

      inline auto textbox_base::get_cursor_pos() const->const position& {
        return data.cursor_pos;
      }

      inline textbox_base::data::data ()
        : font(draw::font::system())
        , last_mouse_point(core::point::undefined)
      {}

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin align, draw::frame::drawer frame, os::color fg, os::color bg>
    inline basic_textbox<align, frame, fg, bg>::basic_textbox () {
      register_event_handler(REGISTER_FUNCTION, paint_event(draw::buffered_paint(this, &basic_textbox::handle_paint)));
    }

    template<text_origin align, draw::frame::drawer frame, os::color fg, os::color bg>
    inline void basic_textbox<align, frame, fg, bg>::handle_paint (const draw::graphics& graph) {
      const auto area = client_area();
      paint::text_box(graph, area, data.lines, data.font,
                      fg, bg, align,
                      data.selection, data.cursor_pos, data.offset, has_focus());
      frame(graph, area);
    }

    // --------------------------------------------------------------------------

  } // win

} // gui

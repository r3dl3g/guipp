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

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace paint {

      GUIPP_CTRL_EXPORT void text_box (const draw::graphics& graph,
                     const core::rectangle& area,
                     const std::vector<std::string>& lines,
                     const draw::font& fnt,
                     os::color foreground,
                     os::color background,
                     const text_origin_t origin_t,
                     const core::range<core::basic_point<int> >& selection,
                     const core::basic_point<int>& cursor_pos,
                     const core::point& offset,
                     const bool has_focus);

    } // paint

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT textbox_base : public control {
      public:
        typedef control super;
        typedef no_erase_window_class<textbox_base, win::cursor_type::ibeam> clazz;
        typedef std::vector<std::string> strings;
        typedef strings::size_type size_type;

        typedef core::basic_point<int> position;
        typedef core::range<position> range;

        using control::create;
        void create (win::container& parent,
                     const core::rectangle& r = core::rectangle::def);

        void create (win::container& parent,
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

        void make_selection_visible ();

        position find_prev_word (const position& pos);
        position find_next_word (const position& pos);

      protected:
        position get_position_at_point (const core::point& pt, const text_origin_t origin_t) const;

        void erase_lines (int first, int last);
        void erase_line (int first);

        void notify_content_changed () const;
        void notify_selection_changed () const;

        void enable_select_by_mouse (const text_origin_t origin_t);

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
    template<text_origin_t align = text_origin_t::vcenter_left,
             draw::frame::drawer frame = draw::frame::no_frame,
             os::color foreground = color::black,
             os::color background = color::white>
    class basic_textbox : public detail::textbox_base {
    public:
      typedef detail::textbox_base super;

      basic_textbox ();

      void enable_select_by_mouse ();

    private:
      void handle_paint (const draw::graphics& graph);

    };

    // --------------------------------------------------------------------------
    using textbox = basic_textbox<>;

  } // ctrl

} // gui

#include <gui/ctrl/textbox.inl>

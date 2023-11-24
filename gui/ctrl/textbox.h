/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     multiline textbox control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/draw/font.h>
#include <gui/draw/text_origin.h>
#include <gui/draw/frames.h>
#include <gui/ctrl/control.h>
#include <gui/ctrl/look/textbox.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT textbox_base : public colored_control {
      public:
        typedef colored_control super;
        typedef win::no_erase_window_class<textbox_base, win::cursor_type::ibeam> clazz;
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
        std::string cut_selected_text ();

        core::rectangle get_virtual_geometry (const core::rectangle&) const;
        core::size get_scroll_steps () const;

        void make_selection_visible ();

        position find_prev_word (const position& pos);
        position find_next_word (const position& pos);

        void set_font (const draw::font& f);
        const draw::font& get_font () const;

      protected:
        position get_position_at_point (const core::point& pt, const text_origin_t origin_t) const;

        void erase_lines (int first, int last);
        void erase_line (int first);

        void notify_content_changed ();

        void enable_select_by_mouse (const text_origin_t origin_t);

        struct data {
          data ();

          strings lines;
          draw::font font;
          core::point offset;
          core::native_point last_mouse_point;
          position cursor_pos;
          range selection;
          mutable core::size virtual_size;
        } data;

      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<text_origin_t align = text_origin_t::vcenter_left,
             draw::frame::drawer frame = draw::frame::no_frame>
    class basic_textbox : public detail::textbox_base {
    public:
      typedef detail::textbox_base super;

      basic_textbox ();

      void enable_select_by_mouse ();

    private:
      void handle_paint (draw::graphics& graph);

    };

    // --------------------------------------------------------------------------
    using textbox = basic_textbox<>;

  } // ctrl

} // gui

#include <gui/ctrl/textbox.inl>

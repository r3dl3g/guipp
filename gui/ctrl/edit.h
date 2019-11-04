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
 * @brief     C++ API: edit controls
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

      GUIPP_CTRL_EXPORT void edit_line (const draw::graphics& graph,
                      const core::rectangle& area,
                      const std::string& text,
                      const draw::font& fnt,
                      os::color foreground,
                      os::color background,
                      const text_origin origin,
                      const core::range<size_t>& selection,
                      const size_t cursor_pos,
                      const size_t scroll_pos,
                      const bool has_focus);

    } // paint

    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT edit_base : public control {
      public:
        typedef control super;
        typedef size_t pos_t;

        template<typename T = edit_base, os::color C = color::white>
        using clazz = win::window_class<T, C, win::cursor_type::ibeam>;

        typedef core::range<pos_t> range;

        edit_base ();
        edit_base (const edit_base&);
        edit_base (edit_base&&);

        ~edit_base ();

        void create (const win::container& parent,
                     const std::string& txt = std::string(),
                     const core::rectangle& place = core::rectangle::def);

        void create (const win::container& parent,
                     const core::rectangle& place = core::rectangle::def);

        void set_text (const std::string&);
        const std::string& get_text () const;

        void set_text_limit (pos_t max_chars);
        pos_t get_text_limit () const;
        pos_t get_text_length () const;

        void handle_key (os::key_state, os::key_symbol, const std::string&);

        void set_selection(const range &sel, event_source);
        range get_selection () const;
        void make_selection_visible (bool update = true);

        pos_t get_cursor_pos () const;
        void set_cursor_pos (pos_t pos, bool shift = false);

        void replace_selection (const std::string& new_text);
        std::string get_text_in_range (const range&) const;
        std::string get_selected_text () const;

      protected:
        void register_handler ();

        void prepare_input ();
        pos_t get_position_at_point (const core::point& pt);

        void notify_content_changed () const;

        struct data {
          data ();

          std::string text;

          range selection;
          pos_t cursor_pos;
          pos_t text_limit;
          pos_t scroll_pos;
          core::point last_mouse_point;
        } data;

      private:
        void init ();

      };
    } // detail

    // --------------------------------------------------------------------------
    template<text_origin alignment,
             draw::frame::drawer frame = draw::frame::sunken_relief,
             os::color foreground = color::black,
             os::color background = color::white>
    class basic_edit : public detail::edit_base {
    public:
      typedef detail::edit_base super;

      basic_edit ();
      basic_edit (const basic_edit& rhs);
      basic_edit (basic_edit&& rhs);

      void paint (const draw::graphics& graph);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    using edit_left = basic_edit<text_origin::vcenter_left>;
    typedef edit_left edit;
    using edit_right = basic_edit<text_origin::vcenter_right>;
    using edit_center = basic_edit<text_origin::center>;

    // --------------------------------------------------------------------------
    template<text_origin alignment,
             char C = '#',
             draw::frame::drawer frame = draw::frame::sunken_relief,
             os::color foreground = color::black,
             os::color background = color::white>
    class basic_pass : public detail::edit_base {
    public:
      typedef detail::edit_base super;

      basic_pass ();
      basic_pass (const basic_pass& rhs);
      basic_pass (basic_pass&& rhs);

      void paint (const draw::graphics& graph);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    using pass_left = basic_pass<text_origin::vcenter_left>;
    typedef pass_left pass;
    using pass_right = basic_pass<text_origin::vcenter_right>;
    using pass_center = basic_pass<text_origin::center>;

  } // ctrl

} // gui

#include <gui/ctrl/edit.inl>

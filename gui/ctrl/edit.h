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
 * @brief     C++ API: edit controls
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/font.h>
#include <gui/ctrl/control.h>
#include <gui/ctrl/look/edit.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace filter {

      typedef std::string (text_filter_f) (std::string);

      std::string GUIPP_CTRL_EXPORT alpha_filter (std::string);
      std::string GUIPP_CTRL_EXPORT alpha_numeric_filter (std::string);
      std::string GUIPP_CTRL_EXPORT digits_filter (std::string);
      std::string GUIPP_CTRL_EXPORT xdigits_filter (std::string);
      std::string GUIPP_CTRL_EXPORT lower_filter (std::string);
      std::string GUIPP_CTRL_EXPORT upper_filter (std::string);
      std::string GUIPP_CTRL_EXPORT float_filter (std::string);
      std::string GUIPP_CTRL_EXPORT integer_filter (std::string);
      std::string GUIPP_CTRL_EXPORT unsigned_filter (std::string);

    }

    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT edit_base : public control {
      public:
        typedef control super;
        typedef size_t pos_t;

        typedef win::window_class<edit_base, color::white, win::cursor_type::ibeam> clazz;

        typedef std::function<filter::text_filter_f> text_filter;
        typedef core::range<pos_t> range;

        ~edit_base ();

        void create (win::container& parent,
                     const std::string& txt,
                     const core::rectangle& place = core::rectangle::def);

        void create (win::container& parent,
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
        virtual std::string get_selected_text () const;

        void set_text_filter (text_filter);
        std::string filter_text (const std::string&) const;

      protected:
        void register_handler ();

        void prepare_input ();
        pos_t get_position_at_point (const core::point& pt) const;

        struct GUIPP_CTRL_EXPORT data {
          data ();

          std::string text;

          range selection;
          pos_t cursor_pos;
          pos_t text_limit;
          pos_t scroll_pos;
          core::native_point last_mouse_point;
          text_filter filter;
        } data;

      };
    } // detail

    // --------------------------------------------------------------------------
    template<text_origin_t origin,
             draw::frame::drawer frame = draw::frame::sunken_relief,
             os::color foreground = color::black,
             os::color background = color::white>
    class basic_edit : public detail::edit_base {
    public:
      typedef detail::edit_base super;

      basic_edit ();
      basic_edit (const basic_edit& rhs);
      basic_edit (basic_edit&& rhs) noexcept ;

      void paint (draw::graphics& graph);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    using edit_left = basic_edit<text_origin_t::vcenter_left>;
    typedef edit_left edit;
    using edit_right = basic_edit<text_origin_t::vcenter_right>;
    using edit_center = basic_edit<text_origin_t::center>;

    // --------------------------------------------------------------------------
    template<text_origin_t origin,
             char C = '#',
             draw::frame::drawer frame = draw::frame::sunken_relief,
             os::color foreground = color::black,
             os::color background = color::white>
    class basic_pass : public detail::edit_base {
    public:
      typedef detail::edit_base super;

      basic_pass ();
      basic_pass (const basic_pass& rhs);
      basic_pass (basic_pass&& rhs) noexcept ;

      void paint (draw::graphics& graph);

      virtual std::string get_selected_text () const;

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    using pass_left = basic_pass<text_origin_t::vcenter_left>;
    typedef pass_left pass;
    using pass_right = basic_pass<text_origin_t::vcenter_right>;
    using pass_center = basic_pass<text_origin_t::center>;

  } // ctrl

} // gui

#include <gui/ctrl/edit.inl>

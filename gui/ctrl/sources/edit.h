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

      void edit_line (const draw::graphics& graph,
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
      class edit_base : public window {
      public:
        typedef window super;
        typedef size_t pos_t;

        typedef core::range<pos_t> range;

        edit_base ();
        edit_base (const edit_base&);
        edit_base (edit_base&&);

        ~edit_base ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def,
                     const std::string& txt = std::string()) {
          super::create(clazz, parent, place);
          prepare_input();
          set_text(txt);
        }

        void set_text (const std::string&);
        const std::string& get_text () const;

        void set_text_limit (pos_t max_chars);
        pos_t get_text_limit () const;
        pos_t get_text_length () const;

        void handle_key (os::key_state, os::key_symbol, const std::string&);

        void set_selection (const range& sel, event_source);
        range get_selection () const;
        void make_cursor_visible ();

        pos_t get_cursor_pos () const;
        void set_cursor_pos (pos_t pos, bool shift = false);

        void replace_selection (const std::string& new_text);
        std::string get_text_in_range (const range&) const;
        std::string get_selected_text () const;

      protected:
        void register_handler (text_origin);

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

        static window_class clazz;
      };
    } // detail

    // --------------------------------------------------------------------------
    template<text_origin A>
    class edit_t : public detail::edit_base {
    public:
      typedef detail::edit_base super;

      edit_t () {
        register_handler(A);
      }

      edit_t (const edit_t& rhs)
        : super(rhs)
      {
        register_handler(A);
      }

      edit_t (edit_t&& rhs)
        : super(std::move(rhs))
      {
        register_handler(A);
      }
    };

    // --------------------------------------------------------------------------
    using edit_left = edit_t<text_origin::vcenter_left>;
    typedef edit_left edit;
    using edit_right = edit_t<text_origin::vcenter_right>;
    using edit_center = edit_t<text_origin::center>;

  } // win

} // gui

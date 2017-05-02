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
#include "alignment_h.h"


namespace gui {

  namespace win {

    namespace detail {
      class edit_base : public gui::win::window_with_text {
      public:
        typedef gui::win::window_with_text super;
        typedef size_t pos_t;

        struct range {
          pos_t first;
          pos_t last;

          range (pos_t v = 0)
            : first(v)
            , last(v)
          {}

          range (pos_t f, pos_t l)
            : first(f)
            , last(l)
          {}

          bool is_inside (pos_t i) const {
            return (i > first) && (i < last);
          }

          void sort () {
            if (first > last) {
              std::swap(first, last);
            }
          }

          bool empty () const {
            return first >= last;
          }
        };

        edit_base ();
        ~edit_base ();

        void create(const container& parent,
          const core::rectangle& place = core::rectangle::def,
          const std::string& txt = std::string()) {
          super::create(clazz, parent, place);
          prepare_input();
          set_text(txt);
        }

        void set_selection (const range& sel);
        range get_selection () const;

        pos_t get_cursor_pos () const;
        void set_cursor_pos (pos_t pos, bool shift = false);

        void set_text_limit (pos_t max_chars);
        pos_t get_text_limit () const;
        pos_t get_text_length () const;

        void replace_selection (const std::string &new_text);

      protected:
        void register_handler (alignment_h);

        void prepare_input ();
        pos_t get_char_at_point (const core::point& pt);

        range selection;
        pos_t cursor_pos;
        pos_t text_limit;
        pos_t scroll_pos;
        core::point last_mouse_point;
#ifdef X11
        XIM im;
        XIC ic;
#endif // X11

      private:
        static window_class clazz;
      };
    }

    namespace paint {

      void edit_box(const draw::graphics& graph,
                    const win::window& btn,
                    const std::string& text,
                    draw::text_origin origin,
                    const detail::edit_base::range& selection,
                    detail::edit_base::pos_t cursor_pos,
                    detail::edit_base::pos_t scroll_pos);

    }

    template<alignment_h A>
    class editT : public detail::edit_base {
    public:
      typedef detail::edit_base super;

      editT () {
        register_handler(A);
      }
    };

    // --------------------------------------------------------------------------
    typedef editT<alignment_left> edit_left;
    typedef edit_left edit;
    typedef editT<alignment_right> edit_right;
    typedef editT<alignment_center> edit_center;

  } // win

} // gui

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
#include "window.h"
#include "window_event_handler.h"
#include "alignment_h.h"


namespace gui {

  namespace win {

    namespace detail {
      class edit_base : public gui::win::window_with_text {
      public:
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

        void set_selection (const range& sel);
        range get_selection () const;

        pos_t get_cursor_pos () const;
        void set_cursor_pos (pos_t pos, bool shift = false);

        void set_text_limit (pos_t max_chars);
        pos_t get_text_limit () const;
        pos_t get_text_length () const;

        void replace_selection (const std::string &new_text);

      protected:
        static window_class create_edit_class(alignment_h);
        void register_handler();

#ifdef X11
        void prepare_input ();
        pos_t get_char_at_point (const core::point& pt);

        range selection;
        pos_t cursor_pos;
        pos_t text_limit;
        pos_t scroll_pos;
        XIM im;
        XIC ic;

        core::point last_mouse_point;
#endif // X11
      };
    }

    template<alignment_h A>
    class editT : public detail::edit_base {
    public:
      typedef detail::edit_base super;

      editT () {
        register_handler();
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
#ifdef X11
        prepare_input();
#endif // X11
        set_text(txt);
      }

    private:
      static window_class clazz;
    };

    template<alignment_h A> window_class editT<A>::clazz(editT<A>::create_edit_class(A));

    // --------------------------------------------------------------------------
    typedef editT<alignment_left> edit_left;
    typedef edit_left edit;
    typedef editT<alignment_right> edit_right;
    typedef editT<alignment_center> edit_center;

  } // win

} // gui

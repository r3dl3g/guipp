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
 * @brief     multiline editbox control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/textbox.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT editbox_base : public textbox_base {
      public:
        typedef textbox_base super;
        typedef win::no_erase_window_class<editbox_base, win::cursor_type::ibeam> clazz;

        editbox_base ();
        ~editbox_base ();

        using control::create;
        void create (win::container& parent,
                     const core::rectangle& r = core::rectangle::def);

        void handle_key (os::key_state, os::key_symbol, const std::string&);

      private:
        void init ();

        void handle_create ();

        std::string& current_line ();

      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<draw::frame::drawer frame = draw::frame::no_frame>
    class basic_editbox : public detail::editbox_base {
    public:
      typedef detail::editbox_base super;

      basic_editbox (os::color fg = color::black, os::color bg = color::white);

      void enable_select_by_mouse ();

      void handle_paint (draw::graphics& graph);

    private:
      os::color foreground;
    };

    // --------------------------------------------------------------------------
    typedef basic_editbox<> editbox;

  } // ctrl

} // gui

#include <gui/ctrl/editbox.inl>

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
 * @brief     C++ API: editbox control
 *
 * @file
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
        typedef no_erase_window_class<editbox_base, win::cursor_type::ibeam> clazz;

        editbox_base ();
        ~editbox_base ();

        void create (const win::container& parent,
                     const core::rectangle& r = core::rectangle::def);

        void handle_key (os::key_state, os::key_symbol, const std::string&);

      private:
        void init ();

        void handle_create (window*, const core::rectangle&);

        std::string& current_line ();

      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<draw::frame::drawer frame = draw::frame::no_frame,
             os::color foreground = color::black,
             os::color background = color::white>
    class basic_editbox : public detail::editbox_base {
    public:
      typedef detail::editbox_base super;

      basic_editbox ();

      void enable_select_by_mouse ();

      void handle_paint (const draw::graphics& graph);

    };

    // --------------------------------------------------------------------------
    typedef basic_editbox<> editbox;

  } // ctrl

} // gui

#include <gui/ctrl/editbox.inl>

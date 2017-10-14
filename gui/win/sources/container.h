/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window
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
#include "layout.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    class container : public window {
    public:
      typedef window super;

      container ();

      bool is_parent_of (const window& parent) const;
      bool is_sub_window (const window* child) const;

      void set_children_visible (bool = true);

      std::vector<window*> get_children () const;

      void shift_focus (window&, bool backward = false);
      void forward_focus (bool backward = false);

    protected:
      container (const container&);
      container (container&&);

    private:
      void init ();
    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<typename B, typename L = layout::standard_layout, typename ... Args>
      class layout_base : public B {
      public:
        typedef B super;
        typedef L layout_type;

        layout_base (const Args& ... args)
          : layouter(this, args ...)
        {}

        layout_base (const layout_base& rhs)
          : super(rhs)
          , layouter(this, rhs.layouter)
        {}

        layout_base (layout_base&& rhs)
          : super(std::move(rhs))
          , layouter(this, std::move(rhs.layouter))
        {}

        void layout () {
          layouter.layout(super::size());
        }

        inline layout_type& get_layout () {
          return layouter;
        }

        inline const layout_type& get_layout () const {
          return layouter;
        }

      private:
        layout_type layouter;
      };

    } // detail

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_container = detail::layout_base<container, L, Args ...>;

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, os::color background = color::white, typename ... Args>
    class group_window : public layout_container<L, Args ...> {
    public:
      typedef layout_container<L, Args ...> super;
      typedef window_class<group_window, background> clazz;

      group_window (const Args& ... args)
        : super(args ...)
      {}

      void create (const container& parent,
                   const core::rectangle& r = core::rectangle::def) {
        super::create(clazz::get(), parent, r);
      }

    };

    // --------------------------------------------------------------------------
    class overlapped_window : public container {
    public:
      typedef container super;

      void set_title (const std::string&);
      std::string get_title () const;

      bool is_top_most () const;
      bool is_minimized () const;
      bool is_maximized () const;

      void minimize ();
      void maximize ();
      void restore ();

      void set_top_most (bool toplevel);

      void create (const class_info&,
                   const window&,
                   const core::rectangle& = core::rectangle::def);

      void create (const class_info&,
                   const core::rectangle& = core::rectangle::def);
    };

    // --------------------------------------------------------------------------
    class modal_window : public overlapped_window {
    public:
      typedef overlapped_window super;

      modal_window ();
      modal_window (const modal_window&);
      modal_window (modal_window&&);

      void end_modal ();
      void run_modal ();

    private:
      void init ();

      volatile bool is_modal;
    };

    // --------------------------------------------------------------------------
    class main_window : public overlapped_window {
    public:
      typedef overlapped_window super;
#ifdef WIN32
      typedef window_class<main_window,
                           (os::color)(COLOR_APPWORKSPACE + 1),
                           cursor_type::arrow,
                           WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                           WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_COMPOSITED> clazz;
#endif // WIN32
#ifdef X11
      typedef window_class<main_window, color::medium_gray> clazz;
#endif // X11

      void create (const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_main_window = detail::layout_base<main_window, L, Args ...>;

    // --------------------------------------------------------------------------
    class popup_window : public modal_window {
    public:
      typedef modal_window super;
#ifdef WIN32
      typedef window_class<popup_window,
                           color::light_gray,
                           window_class_defaults<>::cursor,
                           WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                           WS_EX_PALETTEWINDOW | WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED,
                           CS_DBLCLKS | CS_DROPSHADOW> clazz;
#endif // WIN32
#ifdef X11
      typedef window_class<popup_window, color::light_gray> clazz;
#endif // X11

      void create (const window& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_popup_window = detail::layout_base<popup_window, L, Args ...>;

    // --------------------------------------------------------------------------
    class dialog_window : public modal_window {
    public:
      typedef modal_window super;
#ifdef WIN32
      typedef window_class<dialog_window,
                           color::light_gray,
                           window_class_defaults<>::cursor,
                           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SYSMENU |
                           WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                           WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED> clazz;
#endif // WIN32
#ifdef X11
      typedef window_class<dialog_window, color::light_gray> clazz;
#endif // X11

      void create (const window& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_dialog_window = detail::layout_base<dialog_window, L, Args ...>;

    // --------------------------------------------------------------------------
  } // win

} // gui

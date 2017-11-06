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
// Library includes
//
#include <gui/win/window.h>
#include <gui/win/layout.h>


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
      class layout_container_base : public B {
      public:
        typedef B super;
        typedef L layout_type;

        layout_container_base (const Args& ... args);
        layout_container_base (const layout_container_base& rhs);
        layout_container_base (layout_container_base&& rhs);

        void layout ();

        layout_type& get_layout ();
        const layout_type& get_layout () const;

      private:
        void init ();
        layout_type layouter;
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_container = detail::layout_container_base<container, L, Args ...>;

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, os::color background = color::white, typename ... Args>
    class group_window : public layout_container<L, Args ...> {
    public:
      typedef layout_container<L, Args ...> super;
      typedef window_class<group_window, background> clazz;

      group_window (const Args& ... args);

      void create (const container& parent,
                   const core::rectangle& r = core::rectangle::def);

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

    struct hot_key_action {
      hot_key hk;
      hot_key::call fn;
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
      void run_modal (const std::vector<hot_key_action>& hot_keys);

    private:
      void init ();

      volatile bool is_modal;
    };

    // --------------------------------------------------------------------------
    class main_window : public overlapped_window {
    public:
      typedef overlapped_window super;
#ifdef WIN32
      template<typename T = main_window, os::color C = (os::color)(COLOR_APPWORKSPACE + 1)>
      using clazz = window_class<T,
                                 C,
                                 cursor_type::arrow,
                                 WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                                 WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_COMPOSITED>;
#endif // WIN32
#ifdef X11
      template<typename T = main_window, os::color C = color::medium_gray>
      using clazz = window_class<T, C>;
#endif // X11

      void create (const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_main_window = detail::layout_container_base<main_window, L, Args ...>;

    // --------------------------------------------------------------------------
    class popup_window : public modal_window {
    public:
      typedef modal_window super;
#ifdef WIN32
      template<typename T = popup_window, os::color C = color::light_gray>
      using clazz = window_class<T,
                                 C,
                                 window_class_defaults<>::cursor,
                                 WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                 WS_EX_PALETTEWINDOW | WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED,
                                 CS_DBLCLKS | CS_DROPSHADOW>;
#endif // WIN32
#ifdef X11
      template<typename T = popup_window, os::color C = color::light_gray>
      using clazz = window_class<T, C>;
#endif // X11

      void create (const window& parent, const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, const window& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_popup_window = detail::layout_container_base<popup_window, L, Args ...>;

    // --------------------------------------------------------------------------
    class dialog_window : public modal_window {
    public:
      typedef modal_window super;
#ifdef WIN32
      template<typename T = dialog_window, os::color C = color::light_gray>
      using clazz = window_class<T,
                                 C,
                                 window_class_defaults<>::cursor,
                                 WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SYSMENU |
                                 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                                 WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED>;
#endif // WIN32
#ifdef X11
      template<typename T = dialog_window, os::color C = color::light_gray>
      using clazz = window_class<T, C>;
#endif // X11

      void create (const window& parent, const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, const window& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_dialog_window = detail::layout_container_base<dialog_window, L, Args ...>;

    // --------------------------------------------------------------------------
    //
    // inlines
    //

    namespace detail {

      // --------------------------------------------------------------------------
      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (const A& ... args)
        : layouter(this, args ...)
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (const layout_container_base& rhs)
        : super(rhs)
        , layouter(this, rhs.layouter)
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline layout_container_base<B, L, A...>::layout_container_base (layout_container_base&& rhs)
        : super(std::move(rhs))
        , layouter(this, std::move(rhs.layouter))
      {
        init();
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::layout () {
        layouter.layout(super::size());
      }

      template<typename B, typename L, typename ... A>
      inline auto layout_container_base<B, L, A...>::get_layout () -> layout_type& {
        return layouter;
      }

      template<typename B, typename L, typename ... A>
      inline auto layout_container_base<B, L, A...>::get_layout () const -> const layout_type& {
        return layouter;
      }

      template<typename B, typename L, typename ... A>
      inline void layout_container_base<B, L, A...>::init () {
        super::register_event_handler(REGISTER_FUNCTION, win::show_event([&] () {
          layout();
        }));
      }


    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename L, os::color B, typename ... A>
    inline group_window<L, B, A...>::group_window (const A& ... args)
      : super(args ...)
    {}

    template<typename L, os::color B, typename ... A>
    inline void group_window<L, B, A...>::create (const container& parent,
                                                  const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

    // --------------------------------------------------------------------------
    inline void main_window::create (const core::rectangle& r) {
      create(clazz<>::get(), r);
    }

    // --------------------------------------------------------------------------
    inline void popup_window::create (const window& parent, const core::rectangle& r) {
      create(clazz<>::get(), parent, r);
    }

    // --------------------------------------------------------------------------
    inline void dialog_window::create (const window& parent, const core::rectangle& r) {
      create(clazz<>::get(), parent, r);
    }

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

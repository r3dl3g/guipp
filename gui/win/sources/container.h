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
      template<typename B, typename L = layout::standard_layout, typename... Args>
      class layout_base : public B {
      public:
        typedef B super;
        typedef L layout_type;

        layout_base (const Args&... args)
          : layouter(this, args...)
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

        inline layout_type& get_layout() {
          return layouter;
        }

        inline const layout_type& get_layout() const {
          return layouter;
        }

      private:
        layout_type layouter;
      };

    } // detail

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename... Args>
    using layout_container = detail::layout_base<container, L, Args...>;

    window_class create_group_window_clazz (os::color);

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, os::color background = color::white, typename... Args>
    class group_window : public layout_container<L, Args...> {
    public:
      typedef layout_container<L, Args...> super;

      group_window (const Args&... args)
        : super(args...)
      {}

      void create (const container& parent,
                   const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, parent, r);
      }

    private:
      static window_class clazz;
    };

    template<typename L, os::color background, typename... Args>
    window_class group_window<L, background, Args...>::clazz(create_group_window_clazz(background));

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

      void create (const window_class&,
                   const window&,
                   const core::rectangle& = core::rectangle::def);

      void create (const window_class&,
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

    namespace detail {

      // --------------------------------------------------------------------------
      class main_window_class : public window_class {
      public:
        main_window_class ();

        void prepare (window*, os::window) const override;
      };

      // --------------------------------------------------------------------------
      class popup_window_class : public window_class {
      public:
        popup_window_class ();

        void prepare (window*, os::window) const override;
      };

      // --------------------------------------------------------------------------
      class dialog_window_class : public window_class {
      public:
        dialog_window_class ();

        void prepare (window*, os::window) const override;
      };

    }

    // --------------------------------------------------------------------------
    class main_window : public overlapped_window {
      public:
        typedef overlapped_window super;

      void create (const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, r);
      }

    protected:
      static detail::main_window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename... Args>
    using layout_main_window = detail::layout_base<main_window, L, Args...>;

    // --------------------------------------------------------------------------
    class popup_window : public modal_window {
    public:
      typedef modal_window super;

      void create (const window& parent, const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, parent, r);
      }

    protected:
      static detail::popup_window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename... Args>
    using layout_popup_window = detail::layout_base<popup_window, L, Args...>;

    // --------------------------------------------------------------------------
    class dialog_window : public modal_window {
    public:
      typedef modal_window super;

      void create (const window& parent, const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, parent, r);
      }

    protected:
      static detail::dialog_window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename... Args>
    using layout_dialog_window = detail::layout_base<dialog_window, L, Args...>;

    // --------------------------------------------------------------------------
  } // win

} // gui

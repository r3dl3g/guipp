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
#ifdef WIN32
#include <gui/win/win32_container_class.h>
#endif // WIN32
#ifdef X11
#include <gui/win/x11_container_class.h>
#endif // X11
#ifdef QT_WIDGETS_LIB
#include <gui/win/x11_container_class.h>
#endif // QT_WIDGETS_LIB


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT container : public window {
    public:
      typedef window super;
      typedef std::vector<window*> window_list_t;

      container ();

      bool is_parent_of (const window& child) const;
      bool is_sub_window (const window* child) const;

      void set_children_visible (bool = true);

      window_list_t get_children () const;

      void shift_focus (const window&, bool backward = false) const;
      void forward_focus (bool backward = false) const;

    protected:
      using window::create;

      container (const container&);
      container (container&&);

    private:
      os::window get_id () const;

      void init ();
    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT overlapped_window : public container {
    public:
      typedef container super;

      overlapped_window ();

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
                   const container&,
                   const core::rectangle& = core::rectangle::def);

      void create (const class_info&,
                   const core::rectangle& = core::rectangle::def);

    private:
      os::window get_id () const;
    };

    // --------------------------------------------------------------------------
    struct hot_key_action {
      hot_key hk;
      hot_key::call fn;
    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT modal_window : public overlapped_window {
    public:
      typedef overlapped_window super;

      modal_window ();
      modal_window (const modal_window&);
      modal_window (modal_window&&);

      void end_modal ();
      void run_modal (window&);
      void run_modal (window&, const std::vector<hot_key_action>& hot_keys);

    private:
      void init ();

      volatile bool is_modal;
    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT main_window : public overlapped_window {
    public:
      typedef overlapped_window super;
      using clazz = cls::main_window_class<main_window>;

      void create (const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT popup_window : public modal_window {
    public:
      typedef modal_window super;
      using clazz = cls::popup_window_class<popup_window>;

      void create (container& parent, const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, const container& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT dialog_window : public modal_window {
    public:
      typedef modal_window super;
      using clazz = cls::dialog_window_class<dialog_window>;

      void create (container& parent, const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, const container& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

#include <gui/win/container.inl>

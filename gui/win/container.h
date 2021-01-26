/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
#ifdef GUIPP_WIN
# include <gui/win/container_class_win32.h>
#elif GUIPP_X11
# include <gui/win/container_class_x11.h>
#elif GUIPP_QT
# include <gui/win/container_class_qt.h>
# include <QtCore/QEventLoop>
#endif // GUIPP_QT


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

      void shift_focus (window*, bool backward = false);

      void shift_focus (bool backward = false) override;
      bool can_accept_focus () const override;
      void take_focus (bool backward = false) override;

    protected:
      using window::create;

      container (const container&);
      container (container&&) noexcept ;

    private:
      os::window get_os_window () const;

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
                   container&,
                   const core::rectangle& = core::rectangle::def);

      void create (const class_info&,
                   const core::rectangle& = core::rectangle::def);

    private:
      os::window get_os_window () const;
    };

    // --------------------------------------------------------------------------
    struct hot_key_action {
      core::hot_key hk;
      core::hot_key::call fn;
    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT modal_window : public overlapped_window {
    public:
      typedef overlapped_window super;

      modal_window ();
      modal_window (const modal_window&);
      modal_window (modal_window&&) noexcept ;

      void end_modal ();
      void run_modal (window&);
      void run_modal (window&, const std::vector<hot_key_action>& hot_keys);

    private:
      void init ();

      volatile bool is_modal;
#ifdef GUIPP_QT
      QEventLoop event_loop;
#endif // GUIPP_QT
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
      void create (const class_info& cls, container& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT dialog_window : public modal_window {
    public:
      typedef modal_window super;
      using clazz = cls::dialog_window_class<dialog_window>;

      void create (container& parent, const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, container& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

#include <gui/win/container.inl>

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
// Common includes
//


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT container : public window {
    public:
      typedef window super;
      typedef std::vector<window*> window_list_t;

      container ();
      ~container ();

      bool is_parent_of (const window& child) const;
      bool is_sub_window (const window* child) const;

      void set_children_visible (bool = true);

      window_list_t get_children () const;

      typedef bool (window_filter)(const window*);
      void collect_children (window_list_t&, const std::function<window_filter>& filter) const;

      void add_child (window*);
      virtual void remove_child (window*);

      window* window_at_point (const core::native_point&);

      void to_front (window*);
      void to_back (window*);

      bool handle_event (const core::event&, gui::os::event_result&) override;
      os::event_id collect_event_mask () const override;

    protected:

      using window::create;

      container (const container&);
      container (container&&) noexcept ;

    private:
      void init ();

      window_list_t children;
    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

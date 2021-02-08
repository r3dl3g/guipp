/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: list
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/window_state.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT list_state : public window_state {
      typedef window_state super;

      struct GUIPP_CORE_EXPORT is : public super::is {

        explicit is (const state_type& state);

        bool moved () const;
        bool scroll_bar_enabled () const;
        bool grab_focus () const;

      };

      struct GUIPP_CORE_EXPORT set : public super::set  {

        explicit set (state_type& state);

        bool moved (bool b);
        bool scroll_bar_enabled (bool b);
        bool grab_focus (bool b);

      };

    };

    std::ostream& operator<< (std::ostream& out, const list_state::is&);

  } // namespace core

} // namespace gui

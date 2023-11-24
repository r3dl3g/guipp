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
 * @brief     tooltip view
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <atomic>
#include <condition_variable>
#include <mutex>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/time_util.h>
#include <gui/core/text_source.h>
#include <gui/win/overlapped_window.h>
#include <gui/ctrl/gui++-ctrl-export.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT tooltip : public win::tooltip_window {
    public:
      typedef win::tooltip_window super;

      static tooltip& get ();

      void set (core::text_source, win::window*, const core::rectangle&);
      void clear ();

      void set_delay (util::time::duration delay);

    private:
      tooltip ();
      ~tooltip ();

      void create (win::window& p);

      void start ();
      void show ();

      std::mutex guard;
      std::condition_variable timer;
      util::time::time_point next;
      util::time::duration delay;
      core::text_source text;
      core::rectangle area;
      win::window* win;
      std::atomic_bool active;

      std::thread tooltip_task;

    };

  } // ctrl

} // gui

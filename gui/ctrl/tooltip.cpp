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
 * @brief     image view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/text_origin.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <gui/ctrl/tooltip.h>


using namespace gui::draw;
using namespace gui::win;
using namespace gui::core;

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    tooltip::tooltip ()
      : win(nullptr)
      , delay(std::chrono::seconds(1)) {
      on_paint(draw::paint([&](draw::graphics& graph) {
        graph.clear(color::dark_gray);
        graph.text(draw::text_box(text(), client_geometry(), text_origin_t::center),
                   draw::font::system(), color::white);
      }));
    }

    tooltip::~tooltip () {
      {
        std::unique_lock<std::mutex> lock(guard);
        active = false;
        timer.notify_all(); // it is safe, and *sometimes* optimal, to do this outside the lock
      }
      if (tooltip_task.joinable()) {
        tooltip_task.join();
      }
    }

    void tooltip::create (win::window& p) {
      if (!is_valid()) {
        auto& root = p.get_overlapped_window();
        super::create(root);
        set_visible(false);
      }
    }

    void tooltip::start () {
      if (!tooltip_task.joinable()) {
        tooltip_task = std::thread([&] () {
          const auto T100ms = util::time::mkduration(0, 0, 0, 100000);
          active = true;
          while (active) {
            std::unique_lock<std::mutex> lock(guard);
            timer.wait_until(lock, next);
            if (active && /* (util::time::now() > next) && */ win && text && !is_visible()) {
              show();
            }
            const auto d = std::max(delay, T100ms);
            next = util::time::now() + d;
          }
        });
      }
    }

    void tooltip::show () {
      win::run_on_main(*win, [&] () {
        std::unique_lock<std::mutex> lock(guard);
        if (win && text) {
          create(*win);
          const overlapped_window& overlapped = win->get_overlapped_window();
          const auto length = text().size();
          core::point pt = overlapped.get_current_pointer_pos().dxy(15, 15);
          geometry({pt, core::size{length * 8.0F, 25.0F}}, true, true);
          set_visible(true);
        }
      });
    }

    void tooltip::set (core::text_source t, win::window* w, const core::rectangle& r) {
      std::unique_lock<std::mutex> lock(guard);
      text = t;
      area = r;
      win = w;
      next = util::time::now() + delay;
      start();
    }

    void tooltip::set_delay (util::time::duration d) {
      delay = d;
    }

    void tooltip::clear () {
      std::unique_lock<std::mutex> lock(guard);
      set_visible(false);
      text = {};
      area = {};
      win = nullptr;
    }

    // --------------------------------------------------------------------------
    // lazy create static
    tooltip& tooltip::get () {
      static tooltip w;
      return w;
    }

  } // ctrl

} // gui

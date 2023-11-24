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
      , delay(std::chrono::seconds(2)) {
      on_paint(draw::paint([&](draw::graphics& graph) {
        graph.clear(color::dark_gray);
        graph.text(draw::text_box(text(), client_geometry(), text_origin_t::center),
                   draw::font::system(), color::white);
      }));
    }

    tooltip::~tooltip () {
      active = false;
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
          active = true;
          while (active) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::lock_guard<std::mutex> lock(tooltip_quard);
            if (active && win && text && (util::time::now() > next) && !is_visible()) {
              show();
            }
          }
        });
      }
    }

    void tooltip::show () {
      win::run_on_main(*win, [&] () {
        std::lock_guard<std::mutex> lock(tooltip_quard);
        create(*win);
        const overlapped_window& overlapped = win->get_overlapped_window();
        const auto length = text().size();
        core::point pt = overlapped.get_current_pointer_pos().dxy(15, 15);
        geometry({pt, core::size{length * 8.0F, 25.0F}}, true, true);
        set_visible(true);
      });
    }

    void tooltip::set_next_tooltip (core::text_source t, win::window* w, const core::rectangle& r) {
      std::lock_guard<std::mutex> lock(tooltip_quard);
      text = t;
      area = r;
      win = w;
      next = util::time::now() + delay;
      start();
    }

    void tooltip::set_delay (util::time::duration d) {
      delay = d;
    }

    void tooltip::clear_tooltip () {
      std::lock_guard<std::mutex> lock(tooltip_quard);
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

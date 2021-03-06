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
 * @brief     table look
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace look {

    template<typename T, draw::frame::drawer F>
    inline void text_cell (const T& t,
                           draw::graphics& graph,
                           const core::rectangle& place,
                           const text_origin_t& align,
                           const os::color& foreground,
                           const os::color& background,
                           const ctrl::item_state& state) {
      text_cell<std::string, draw::frame::no_frame>(util::string::convert::from(t), graph, place, align,
                                                    foreground, background, state);
      F(graph, place);
    }

  } // namespace look

} // namespace gui

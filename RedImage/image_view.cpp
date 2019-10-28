/**
 * @copyright (c) 2016-2019 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    redimage
 *
 * Customer   -
 *
 * @brief     C++ API: red image view
 *
 * @file
 */

#include "image_view.h"
#include "cvmat2pixmap.h"

void view::image_view::set_image_and_scale (const cv::Mat& src) {
  if (src.empty()) {
    super::set_image(gui::draw::pixmap());
  } else {
    auto native_size = gui::core::global::scale(super::size());
    cv::Size sz(native_size.width(), native_size.height());
    cv::Mat target = cv::Mat(sz, src.type());
    cv::resize(src, target, sz, 0, 0, cv::INTER_NEAREST);
    super::set_image(cvMat2pixmap(target));
  }
}

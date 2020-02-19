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

#include "color_range_view.h"
#include "cvmat2pixmap.h"

view::color_range_view::color_range_view () {
  super::on_size([&](const gui::core::size& sz) {
    update();
  });
}

void view::color_range_view::set_hsv_color_range (const cv::Vec3b &mi, const cv::Vec3b &ma) {
  min = mi;
  max = ma;
  update();
}

void view::color_range_view::update () {
  auto native_size = gui::core::global::scale(super::size());
  cv::Size sz(native_size.width(), native_size.height());
  cv::Mat hsv_img(sz, CV_8UC3);

  cv::Vec3b color(max);
  if (max[0] > min[0]) {
    double scale = double(max[0] - min[0]) / hsv_img.cols;
    for (int c = 0; c < hsv_img.cols; c++) {
      color[0] = (uchar)(min[0] + c * scale);
      hsv_img.col(c).setTo(color);
    }
  } else {
    double scale = double(max[0] + 180 - min[0]) / hsv_img.cols;
    for (int c = 0; c < hsv_img.cols; c++) {
      const double v = c * scale + min[0];
      color[0] = (uchar)(v > 180 ? v - 180 : v);
      hsv_img.col(c).setTo(color);
    }
  }

  cv::Mat rgb_img(sz, CV_8UC3);
  cv::cvtColor(hsv_img, rgb_img, cv::COLOR_HSV2BGR);
  super::set_image_and_scale(rgb_img);
}

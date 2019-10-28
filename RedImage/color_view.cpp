
#include "color_view.h"

#include <opencv2/imgproc/imgproc.hpp>

using namespace gui;
using namespace gui::draw;
using namespace gui::win;
using namespace gui::ctrl;

namespace view {

  color_view::color_view ()
    : value(color::black) {
    on_paint(draw::paint(this, &color_view::paint));
  }
  // --------------------------------------------------------------------------
  void color_view::create (const win::container& parent,
                           const core::rectangle& place) {
    super::create(clazz::get(), parent, place);
  }

  // --------------------------------------------------------------------------
  void color_view::paint (const graphics& graph) {
    graph.clear(value);
  }
  // --------------------------------------------------------------------------
  void color_view::set_rgb_color (byte r, byte g, byte b) {
    value = color::calc_rgb(r, g, b);
    invalidate();
  }
  // --------------------------------------------------------------------------
  void color_view::set_hsv_color (byte h, byte s, byte v) {
    cv::Mat hsv(1, 1, CV_8UC3);
    hsv.setTo(cv::Scalar(h, s, v));
    cv::Mat rgb(1, 1, CV_8UC3);
    cv::cvtColor(hsv, rgb, cv::COLOR_HSV2RGB);
    cv::Vec3b v3 = rgb.at<cv::Vec3b>(0, 0);
    set_rgb_color(v3[0], v3[1], v3[2]);
  }

} // namespace view

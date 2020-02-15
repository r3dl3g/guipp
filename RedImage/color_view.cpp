
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
  void color_view::set_rgb_color (const cv::Vec3b& rgb) {
    value = color::calc_rgb(rgb[0], rgb[1], rgb[2]);
    invalidate();
  }
  // --------------------------------------------------------------------------
  void color_view::set_hsv_color (const cv::Vec3b& hsv) {
    cv::Mat hsv_img(1, 1, CV_8UC3);
    hsv_img.setTo(hsv);
    cv::Mat rgb_img(1, 1, CV_8UC3);
    cv::cvtColor(hsv_img, rgb_img, cv::COLOR_HSV2RGB);
    cv::Vec3b rgb = rgb_img.at<cv::Vec3b>(0, 0);
    set_rgb_color(rgb);
  }

} // namespace view

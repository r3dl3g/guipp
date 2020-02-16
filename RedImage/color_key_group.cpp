
#include "color_key_group.h"

namespace view {

  color_key_group::color_key_group (const std::string& name)
    : hue(0, 180)
    , saturation()
    , value()
  {
    set_name(name);
    on_create([&] (gui::win::window*, const gui::core::rectangle&) {
      colors.create(*this);
      hue.create(*this, "Hue");
      saturation.create(*this, "Sat");
      value.create(*this, "Val");
      get_layout().add({&colors, &hue, &saturation, &value});
    });
  }
  // --------------------------------------------------------------------------
  void color_key_group::set (const data::hsv_range& hsv) {
    hue.set(hsv.hue());
    saturation.set(hsv.saturation());
    value.set(hsv.value());
//    set_name(hsv.name());
    update_colors();
  }
  // --------------------------------------------------------------------------
  void color_key_group::add (const data::hsv_range& hsv) {
    if ((hue.get_min() == 0) && (hue.get_max() == 180)) {
      hue.set(hsv.hue());
    } else {
      hue.add(hsv.hue());
    }
    if ((saturation.get_min() == 0) && (saturation.get_max() == 255)) {
      saturation.set(hsv.saturation());
    } else {
      saturation.add(hsv.saturation());
    }
    if ((value.get_min() == 0) && (value.get_max() == 255)) {
      value.set(hsv.value());
    } else {
      value.add(hsv.value());
    }
    update_colors();
  }
  // --------------------------------------------------------------------------
  void color_key_group::reset () {
    hue.set_range(0, 180);
    saturation.set_range(0, 255);
    value.set_range(0, 255);
    update_colors();
  }
  // --------------------------------------------------------------------------
  void color_key_group::update_colors () {
    colors.set_colors(cv::Vec3b(hue.get_min(), saturation.get_min(), value.get_max()),
                      cv::Vec3b(hue.get_max(), saturation.get_max(), value.get_max()));
  }
  // --------------------------------------------------------------------------
  data::hsv_range color_key_group::get () const {
    return data::hsv_range(hue.get(), saturation.get(), value.get(), colors.get_label());
  }
  // --------------------------------------------------------------------------
  void color_key_group::set_name (const std::string& name) {
    colors.set_label(name);
  }
  // --------------------------------------------------------------------------
  cv::Mat color_key_group::calc_mask (const cv::Mat& hsv_image) const {

    cv::Mat mask;
    if (hue.get_min() > hue.get_max()) {
      cv::Mat m1, m2;
      cv::inRange(hsv_image,
                  cv::Scalar(hue.get_min(), saturation.get_min(), value.get_min()),
                  cv::Scalar(180, saturation.get_max(), value.get_max()),
                  m1);
      cv::inRange(hsv_image,
                  cv::Scalar(0, saturation.get_min(), value.get_min()),
                  cv::Scalar(hue.get_max(), saturation.get_max(), value.get_max()),
                  m2);
      mask = m1 + m2;
    } else {
      cv::inRange(hsv_image,
                  cv::Scalar(hue.get_min(), saturation.get_min(), value.get_min()),
                  cv::Scalar(hue.get_max(), saturation.get_max(), value.get_max()),
                  mask);
    }
    return mask;
  }

} // namespace view

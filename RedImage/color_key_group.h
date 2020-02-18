
#pragma once

#include "color_key.h"
#include "color_group.h"
#include "hsv_range.h"

namespace view {

  class color_key_group : public gui::win::group_window<gui::layout::vertical_lineup<20, 0, 1>, gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::vertical_lineup<20, 0, 2>, gui::color::very_very_light_gray> super;

    color_key_group (const std::string& name = std::string());

    void add (const data::hsv_range&);
    void set (const data::hsv_range&);
    data::hsv_range get () const;

    void reset ();
    void update_colors ();

    void set_name (const std::string&);
    std::string get_name () const;

    void set_hsv (const cv::Vec3b& hsv);
    void check_hsv (const cv::Vec3b& hsv);

    cv::Mat calc_mask (const cv::Mat& hsv_image) const;

    void on_content_changed (std::function<void()>&& f);

    void notify_content_changed () const;

    cv::Mat image;
    cv::Mat mask;

    color_group colors;
  private:
    color_key hue;
    color_key saturation;
    color_key value;
    bool checked;
  };

} // namespace view

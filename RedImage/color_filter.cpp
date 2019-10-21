
#include "color_filter.h"

#include <opencv2/imgproc/imgproc.hpp>

color_filter::color_filter ()
{}
//-----------------------------------------------------------------------------
color_filter::color_filter (const data::hsv_range& range)
  : range(range)
{}
//-----------------------------------------------------------------------------
float color_filter::calc (const cv::Mat& hsv_image, bool erode) {
  cv::Mat mask[8];
  return calc(hsv_image, mask, erode);
}
//-----------------------------------------------------------------------------
float color_filter::calc (const cv::Mat& hsv_image, cv::Mat mask[8], bool erode) {
  float portion = 0;
  const auto hue = range.hue();
  const auto sat = range.saturation();
  const auto val = range.value();

  if (hue.min() > hue.max()) {
    cv::inRange(hsv_image,
                cv::Scalar(hue.min(), sat.min(), val.min()),
                cv::Scalar(180, sat.max(), val.max()),
                mask[1]);
    cv::inRange(hsv_image,
                cv::Scalar(0, sat.min(), val.min()),
                cv::Scalar(hue.max(), sat.max(), val.max()),
                mask[2]);
    mask[0] = mask[1] + mask[2];
  } else {
    cv::inRange(hsv_image,
                cv::Scalar(hue.min(), sat.min(), val.min()),
                cv::Scalar(hue.max(), sat.max(), val.max()),
                mask[0]);
    mask[1] = cv::Scalar();
    mask[2] = cv::Scalar();
  }

  if (erode) {
    cv::Mat kernel = (cv::Mat_<uint8_t>(3, 3) << 1, 1, 1,
                                                 1, 0, 1,
                                                 1, 1, 1);
    cv::bitwise_not(mask[0], mask[3]);
    cv::morphologyEx(mask[3], mask[4], cv::MORPH_ERODE, kernel);
    cv::bitwise_and(mask[0], mask[4], mask[5]);
    cv::bitwise_not(mask[5], mask[6]);
    mask[7] = mask[0].clone();
    cv::bitwise_and(mask[7], mask[6], mask[0]);
  } else {
    for (int i = 3; i < 8; ++i) {
      mask[i] = cv::Scalar();
    }
  }

  int mask_count = cv::countNonZero(mask[0]);
  int full_count = mask[0].cols * mask[0].rows;
  portion = (float)((double)mask_count / (double)full_count);
  return portion;
}

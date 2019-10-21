
#include "cvmat2pixmap.h"

using namespace gui;
using namespace gui::draw;

pixmap cvMat2pixmap (const cv::Mat& source) {
  auto sz = source.size();
  core::native_size nsize = {(uint32_t)sz.width, (uint32_t)sz.height};

  if (CV_8UC3 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), source.cols * source.elemSize(), PixelFormat::RGB);

    typedef const_image_data<PixelFormat::RGB> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<PixelFormat::RGB>(img_data(data, info));
    return image;
  } else if (CV_8UC4 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), source.cols * source.elemSize(), PixelFormat::RGBA);

    typedef const_image_data<PixelFormat::RGBA> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<PixelFormat::RGBA>(img_data(data, info));
    return image;
  } else if (CV_8UC1 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), source.cols * source.elemSize(), PixelFormat::GRAY);

    typedef const_image_data<PixelFormat::GRAY> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<PixelFormat::GRAY>(img_data(data, info));
    return image;
  }

  return pixmap();
}

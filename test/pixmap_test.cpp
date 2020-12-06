
#include <iomanip>

#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <testlib/testlib.h>


using namespace gui;
using namespace gui::draw;


inline pixel::rgb color2rgb (os::color c) {
  pixel::rgb p;
  p = c;
  return p;
}

inline pixel::bgr color2bgr (os::color c) {
  pixel::bgr p;
  p = c;
  return p;
}

const pixel::rgb black = color2rgb(color::black);
const pixel::rgb white = color2rgb(color::white);
const pixel::rgb red = color2rgb(color::red);
const pixel::rgb green = color2rgb(color::green);
const pixel::rgb blue = color2rgb(color::blue);

// --------------------------------------------------------------------------
void test_bitmap_black () {

  bitmap img(2, 2);
  graphics(img).clear(color::black);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), pixel_format_t::BW));

  bwmap bw = img.get();

  const_image_data<pixel_format_t::BW> bw_raw = static_cast<const bwmap&>(bw).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::mono::black);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::mono::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::mono::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::mono::black);
}


// --------------------------------------------------------------------------
void test_bitmap_white () {
  bitmap img(2, 2);
  graphics(img).clear(color::white);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), pixel_format_t::BW));

  bwmap bw = img.get();

  const_image_data<pixel_format_t::BW> bw_raw = static_cast<const bwmap&>(bw).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::mono::white);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::mono::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::mono::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::mono::white);
}


// --------------------------------------------------------------------------
void test_bitmap_checked () {
  bitmap img(2, 2);
  graphics(img).clear(color::white).draw_pixel({0,1}, color::black).draw_pixel({1, 0}, color::black);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), pixel_format_t::BW));

  bwmap bw = img.get();

  const_image_data<pixel_format_t::BW> bw_raw = static_cast<const bwmap&>(bw).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::mono::white);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::mono::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::mono::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::mono::white);
}


// --------------------------------------------------------------------------
void test_pixmap_black () {
  const pixel_format_t expected_pixelformat = core::global::get_device_pixel_format();
  pixmap img(2, 2);
  graphics(img).clear(color::black);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), expected_pixelformat);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, expected_pixelformat));

  rgbamap rgb = img.get<pixel_format_t::RGBA>();

  const_image_data<pixel_format_t::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), black);
  EXPECT_EQUAL(raw.pixel(0, 1), black);
  EXPECT_EQUAL(raw.pixel(1, 0), black);
  EXPECT_EQUAL(raw.pixel(0, 1), black);
}


// --------------------------------------------------------------------------
void test_pixmap_red () {
  const pixel_format_t expected_pixelformat = core::global::get_device_pixel_format();
  pixmap img(2, 2);
  graphics(img).clear(color::red);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), expected_pixelformat);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, expected_pixelformat));

  rgbamap rgb = img.get<pixel_format_t::RGBA>();

  const_image_data<pixel_format_t::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), red);
  EXPECT_EQUAL(raw.pixel(0, 1), red);
  EXPECT_EQUAL(raw.pixel(1, 0), red);
  EXPECT_EQUAL(raw.pixel(1, 1), red);
}


// --------------------------------------------------------------------------
void test_pixmap_green () {
  const pixel_format_t expected_pixelformat = core::global::get_device_pixel_format();
  pixmap img(2, 2);
  graphics(img).clear(color::green);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), expected_pixelformat);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, expected_pixelformat));

  rgbamap rgb = img.get<pixel_format_t::RGBA>();

  const_image_data<pixel_format_t::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), green);
  EXPECT_EQUAL(raw.pixel(0, 1), green);
  EXPECT_EQUAL(raw.pixel(1, 0), green);
  EXPECT_EQUAL(raw.pixel(1, 1), green);
}


// --------------------------------------------------------------------------
void test_pixmap_blue () {
  const pixel_format_t expected_pixelformat = core::global::get_device_pixel_format();
  pixmap img(2, 2);
  graphics(img).clear(color::blue);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), expected_pixelformat);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, expected_pixelformat));

  rgbamap rgb = img.get<pixel_format_t::RGBA>();

  const_image_data<pixel_format_t::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), blue);
  EXPECT_EQUAL(raw.pixel(0, 1), blue);
  EXPECT_EQUAL(raw.pixel(1, 0), blue);
  EXPECT_EQUAL(raw.pixel(1, 1), blue);
}


// --------------------------------------------------------------------------
void test_pixmap_white () {
  const pixel_format_t expected_pixelformat = core::global::get_device_pixel_format();
  pixmap img(2, 2);
  graphics(img).clear(color::white);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), expected_pixelformat);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, expected_pixelformat));

  rgbamap rgb = img.get<pixel_format_t::RGBA>();

  const_image_data<pixel_format_t::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), white);
  EXPECT_EQUAL(raw.pixel(0, 1), white);
  EXPECT_EQUAL(raw.pixel(1, 0), white);
  EXPECT_EQUAL(raw.pixel(1, 1), white);

}


// --------------------------------------------------------------------------
void test_pixmap () {
  const pixel_format_t expected_pixelformat = core::global::get_device_pixel_format();
  pixmap img(2, 2);
  {
    graphics g(img);
    g.clear(color::white);
    auto p1 = g.get_pixel({0, 0});
    EXPECT_EQUAL(p1, color::white);
    g.clear(color::black);
    auto p2 = g.get_pixel({0, 0});
    EXPECT_EQUAL(p2, color::black);
  }
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), expected_pixelformat);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, expected_pixelformat));

  rgbamap rgb = img.get<pixel_format_t::RGBA>();

  auto raw = rgb.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), black);
  EXPECT_EQUAL(raw.pixel(0, 1), black);
  EXPECT_EQUAL(raw.pixel(1, 0), black);
  EXPECT_EQUAL(raw.pixel(1, 1), black);
}


// --------------------------------------------------------------------------
void test_pixmap_draw () {
#ifdef X11
# define EMPTY "000000ff"
# define BLUE  "ff0000ff"
#endif // X11
#ifdef WIN32
# define EMPTY "00000000"
# define BLUE  "ff000000"
#endif // WIN32
#ifdef QT_WIDGETS_LIB
# define EMPTY "000000ff"
# define BLUE  "ff0000ff"
#endif // QT_WIDGETS_LIB

  const pixel_format_t expected_pixelformat = core::global::get_device_pixel_format();

  core::global::set_scale_factor(1.0);

  pixmap img(6, 6);
  {
    graphics g(img);
    g.clear(color::black);
    g.draw(draw::rectangle(core::point(2, 2), core::size(2, 2)), color::blue, color::blue);
    EXPECT_EQUAL(g.get_pixel({0, 0}), color::black);
    EXPECT_EQUAL(g.get_pixel({0, 1}), color::black);
    EXPECT_EQUAL(g.get_pixel({1, 0}), color::black);
    EXPECT_EQUAL(g.get_pixel({1, 1}), color::black);
    EXPECT_EQUAL(g.get_pixel({2, 2}), color::blue);
    EXPECT_EQUAL(g.get_pixel({3, 3}), color::blue);
    EXPECT_EQUAL(g.get_pixel({4, 4}), color::black);
    EXPECT_EQUAL(g.get_pixel({5, 5}), color::black);
  }

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(6, 6));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), expected_pixelformat);
  EXPECT_EQUAL(img.get_info(), bitmap_info(6, 6, expected_pixelformat));

#ifdef X11
  XImage* xim = XGetImage(core::global::get_instance(), img.get_id(), 0, 0, 6, 6, AllPlanes, ZPixmap);
  const int height = xim->height;
  const int bytes_per_line = xim->bytes_per_line;
  const auto data = xim->data;
#endif // X11
#ifdef WIN32
  BITMAP bmi;
  GetObject(img.get_id(), sizeof (BITMAP), &bmi);
  blob data;
  data.resize(bmi.bmHeight * bmi.bmWidthBytes);
  GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
  const int height = bmi.bmHeight;
  const int bytes_per_line = bmi.bmWidthBytes;
#endif // WIN32
#ifdef QT_WIDGETS_LIB
  auto pic = img.get_id().toImage();
  auto data = pic.constBits();
  const int height = pic.height();
  const int bytes_per_line = pic.bytesPerLine();
#endif // QT_WIDGETS_LIB

  std::ostringstream buffer;
  for (int y = 0; y < height; ++y) {
    if (y > 0) {
      buffer << '\n';
    }
    for (int x = 0; x < bytes_per_line; x+=4) {
      if (x > 0) {
        buffer << ' ';
      }
      for (int j = 0; j < 4; ++j) {
        buffer << std::setw(2) << std::setfill('0') << std::hex 
               << (unsigned short)(unsigned char)data[y * bytes_per_line + x + j];
      }
    }
  }
#ifdef X11
  XDestroyImage(xim);
#endif // X11

  EXPECT_EQUAL(buffer.str(), EMPTY " " EMPTY " " EMPTY " " EMPTY " " EMPTY " " EMPTY "\n"
                             EMPTY " " EMPTY " " EMPTY " " EMPTY " " EMPTY " " EMPTY "\n"
                             EMPTY " " EMPTY " " BLUE  " " BLUE  " " EMPTY " " EMPTY "\n"
                             EMPTY " " EMPTY " " BLUE  " " BLUE  " " EMPTY " " EMPTY "\n"
                             EMPTY " " EMPTY " " EMPTY " " EMPTY " " EMPTY " " EMPTY "\n"
                             EMPTY " " EMPTY " " EMPTY " " EMPTY " " EMPTY " " EMPTY);

  rgbamap rgb = img.get<pixel_format_t::RGBA>();

  auto raw = rgb.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), black) ;
  EXPECT_NOT_EQUAL(raw.pixel(2, 2), black);

  int red_pix = 0;
  int green_pix = 0;
  int blue_pix = 0;

//  const uint32_t rows = raw.get_info().height;
//  const uint32_t columns = raw.get_info().width;
  for (uint32_t r = 0; r < 6; ++r) {
    auto row = raw.row(r);
    for (uint32_t c = 0; c < 6; ++c) {
      const auto pix = row[c];
      if (pix.red > 0) ++red_pix;
      if (pix.green > 0) ++green_pix;
      if (pix.blue > 0) ++blue_pix;
    }
  }

  EXPECT_EQUAL(red_pix, 0);
  EXPECT_EQUAL(green_pix, 0);
  EXPECT_EQUAL(blue_pix, 4);
}


// --------------------------------------------------------------------------
void test_pixmap2bitmap () {
  pixmap pix(2, 2);
  {
    graphics g(pix);
    g.draw_pixel({0, 0}, color::black);
    g.draw_pixel({0, 1}, color::white);
    g.draw_pixel({1, 0}, color::white);
    g.draw_pixel({1, 1}, color::black);
    g.flush();

    EXPECT_EQUAL(g.get_pixel({0, 0}), color::black);
    EXPECT_EQUAL(g.get_pixel({0, 1}), color::white);
    EXPECT_EQUAL(g.get_pixel({1, 0}), color::white);
    EXPECT_EQUAL(g.get_pixel({1, 1}), color::black);
  }

  bgrmap bgr = pix.get<pixel_format_t::BGR>();
  auto bgr_raw = bgr.get_data();
  EXPECT_EQUAL(bgr_raw.pixel(0, 0), pixel::color<pixel::bgr>::black);
  EXPECT_EQUAL(bgr_raw.pixel(0, 1), pixel::color<pixel::bgr>::white);
  EXPECT_EQUAL(bgr_raw.pixel(1, 0), pixel::color<pixel::bgr>::white);
  EXPECT_EQUAL(bgr_raw.pixel(1, 1), pixel::color<pixel::bgr>::black);

  rgbmap rgb = pix.get<pixel_format_t::RGB>();
  auto rgb_raw = rgb.get_data();
  EXPECT_EQUAL(rgb_raw.pixel(0, 0), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 1), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 0), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 1), pixel::color<pixel::rgb>::black);

  bwmap bw1 = pix.get<pixel_format_t::BW>();

  const_image_data<pixel_format_t::BW> bw_raw = static_cast<const bwmap&>(bw1).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::mono::black);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::mono::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::mono::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::mono::black);

  bitmap img = bw1;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), pixel_format_t::BW));

  {
    graphics g(img);
    EXPECT_EQUAL(g.get_pixel({0, 0}), color::black);
    EXPECT_EQUAL(g.get_pixel({0, 1}), color::white);
    EXPECT_EQUAL(g.get_pixel({1, 0}), color::white);
    EXPECT_EQUAL(g.get_pixel({1, 1}), color::black);
  }

#ifdef X11
  XImage* xim = XGetImage(core::global::get_instance(), img.get_id(), 0, 0, 2, 2, AllPlanes, ZPixmap);
  auto data = xim->data;
#endif // X11
#ifdef WIN32
  BITMAP bmi;
  GetObject(img.get_id(), sizeof (BITMAP), &bmi);
  blob data;
  data.resize(bmi.bmHeight * bmi.bmWidthBytes);
  int result = GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
  if (result != data.size()) {
    std::cerr << "GetBitmapBits returned " << result << " expected:" << data.size() << std::endl;
  }
#endif // WIN32
#ifdef QT_WIDGETS_LIB
  auto pic = img.get_id().toImage();
  auto data = pic.constBits();
#endif // QT_WIDGETS_LIB

  std::ostringstream buffer;
  for (int i = 0; i < IF_WIN32_ELSE(4, 8); ++i) {
    buffer << std::setw(2) << std::setfill('0') << std::hex << (int)data[i] << ' ';
  }
#ifdef X11
  XDestroyImage(xim);
#endif // X11

  EXPECT_EQUAL(buffer.str(), IF_WIN32_ELSE("40 00 80 00 ", "02 00 00 00 01 00 00 00 "));

  bwmap bw2 = img.get();

  const_image_data<pixel_format_t::BW> bw2_raw = static_cast<const bwmap&>(bw2).get_data();
  EXPECT_EQUAL(bw2_raw.pixel(0, 0), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(0, 1), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 0), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 1), pixel::mono::black);

}


// --------------------------------------------------------------------------
void test_bitmap2pixmap () {
  bwmap bw(2,2);
  using raw_type = image_data<pixel_format_t::BW>;
//  using raw_data_type = image_data<pixel_format_t::BW>::raw_type;
//  using row_type = image_data<pixel_format_t::BW>::row_type;
  raw_type bw_raw = bw.get_data();
  bw_raw.pixel(0, 0) = pixel::mono::black;
  bw_raw.pixel(0, 1) = pixel::mono::white;
  bw_raw.pixel(1, 0) = pixel::mono::white;
  bw_raw.pixel(1, 1) = pixel::mono::black;

  pixmap pix = bw;
  rgbmap rgb = pix.get<pixel_format_t::RGB>();
  auto rgb_raw = rgb.get_data();
  EXPECT_EQUAL(rgb_raw.pixel(0, 0), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 1), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 0), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 1), pixel::color<pixel::rgb>::black);

}


// --------------------------------------------------------------------------
void test_bitmap_scale2pixmap () {
  bwmap bw(2,2);
  using raw_type = image_data<pixel_format_t::BW>;
//  using raw_data_type = image_data<pixel_format_t::BW>::raw_type;
//  using row_type = image_data<pixel_format_t::BW>::row_type;
  raw_type bw_raw = bw.get_data();
  bw_raw.pixel(0, 0) = pixel::mono::black;
  bw_raw.pixel(0, 1) = pixel::mono::white;
  bw_raw.pixel(1, 0) = pixel::mono::white;
  bw_raw.pixel(1, 1) = pixel::mono::black;

  bwmap bw2(4, 4);
  bw2.stretch_from(bw);

  raw_type bw2_raw = bw2.get_data();
  EXPECT_EQUAL(bw2_raw.pixel(0, 0), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(0, 1), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(1, 0), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(1, 1), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(0, 2), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(0, 3), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 2), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 3), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(2, 0), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(3, 0), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(2, 1), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(3, 1), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(2, 2), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(2, 3), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(3, 2), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(3, 3), pixel::mono::black);

  pixmap pix = bw2;
  rgbmap rgb = pix.get<pixel_format_t::RGB>();
  auto rgb_raw = rgb.get_data();
  EXPECT_EQUAL(rgb_raw.pixel(0, 0), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 1), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(1, 0), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(1, 1), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 2), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(0, 3), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 2), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 3), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(2, 0), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(3, 0), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(2, 1), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(3, 1), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(2, 2), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(2, 3), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(3, 2), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(3, 3), pixel::color<pixel::rgb>::black);

}

// --------------------------------------------------------------------------
void test_main (const testing::start_params& params) {
  testing::init_gui(params);

  std::cout << "Running pixmap_test" << std::endl;

  run_test(test_bitmap_black);
  run_test(test_bitmap_white);
  run_test(test_bitmap_checked);
  run_test(test_pixmap_black);
  run_test(test_pixmap_red);
  run_test(test_pixmap_green);
  run_test(test_pixmap_blue);
  run_test(test_pixmap_white);
  run_test(test_pixmap);
  run_test(test_pixmap_draw);
  run_test(test_pixmap2bitmap);
  run_test(test_bitmap2pixmap);
  run_test(test_bitmap_scale2pixmap);
}

// --------------------------------------------------------------------------


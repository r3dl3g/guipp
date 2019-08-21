
#include <iomanip>

#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <testlib/testlib.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
DECLARE_TEST(test_bitmap_black);
DECLARE_TEST(test_bitmap_white);
DECLARE_TEST(test_bitmap_checked);
DECLARE_TEST(test_pixmap_black);
DECLARE_TEST(test_pixmap_red);
DECLARE_TEST(test_pixmap_green);
DECLARE_TEST(test_pixmap_blue);
DECLARE_TEST(test_pixmap_white);
DECLARE_TEST(test_pixmap);
DECLARE_TEST(test_pixmap_draw);

DECLARE_TEST(test_pixmap2bitmap);
DECLARE_TEST(test_bitmap2pixmap);
DECLARE_TEST(test_bitmap_scale2pixmap);

// --------------------------------------------------------------------------
TEST_MAIN(pixmap_test)
  RUN_TEST(test_bitmap_black);
  RUN_TEST(test_bitmap_white);
  RUN_TEST(test_bitmap_checked);
  RUN_TEST(test_pixmap_black);
  RUN_TEST(test_pixmap_red);
  RUN_TEST(test_pixmap_green);
  RUN_TEST(test_pixmap_blue);
  RUN_TEST(test_pixmap_white);
  RUN_TEST(test_pixmap);
  RUN_TEST(test_pixmap_draw);
  RUN_TEST(test_pixmap2bitmap);
  RUN_TEST(test_bitmap2pixmap);
  RUN_TEST(test_bitmap_scale2pixmap);
TEST_MAIN_END(pixmap_test)

using namespace gui;
using namespace gui::draw;

inline pixel::rgb_pixel color2rgb (os::color c) {
  pixel::rgb_pixel p;
  p = c;
  return p;
}

inline pixel::bgr_pixel color2bgr (os::color c) {
  pixel::bgr_pixel p;
  p = c;
  return p;
}

const pixel::rgb_pixel black = color2rgb(color::black);
const pixel::rgb_pixel white = color2rgb(color::white);
const pixel::rgb_pixel red = color2rgb(color::red);
const pixel::rgb_pixel green = color2rgb(color::green);
const pixel::rgb_pixel blue = color2rgb(color::blue);

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_black) {

  bitmap img(2, 2);
  graphics(img).clear(color::black);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), PixelFormat::BW));

  bwmap bw = img.get();

  const_image_data<PixelFormat::BW> bw_raw = static_cast<const bwmap&>(bw).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::bw_pixel::black);
}
END_TEST(test_bitmap_black)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_white) {
  bitmap img(2, 2);
  graphics(img).clear(color::white);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), PixelFormat::BW));

  bwmap bw = img.get();

  const_image_data<PixelFormat::BW> bw_raw = static_cast<const bwmap&>(bw).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::bw_pixel::white);
}
END_TEST(test_bitmap_white)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_checked) {
  bitmap img(2, 2);
  graphics(img).clear(color::white).draw_pixel({0,1}, color::black).draw_pixel({1, 0}, color::black);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), PixelFormat::BW));

  bwmap bw = img.get();

  const_image_data<PixelFormat::BW> bw_raw = static_cast<const bwmap&>(bw).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::bw_pixel::white);
}
END_TEST(test_bitmap_checked)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap_black) {
  pixmap img(2, 2);
  graphics(img).clear(color::black);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGBA));

  rgbamap rgb = img.get<PixelFormat::RGBA>();

  const_image_data<PixelFormat::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), black);
  EXPECT_EQUAL(raw.pixel(0, 1), black);
  EXPECT_EQUAL(raw.pixel(1, 0), black);
  EXPECT_EQUAL(raw.pixel(0, 1), black);
}
END_TEST(test_pixmap_black)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap_red) {
  pixmap img(2, 2);
  graphics(img).clear(color::red);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGBA));

  rgbamap rgb = img.get<PixelFormat::RGBA>();

  const_image_data<PixelFormat::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), red);
  EXPECT_EQUAL(raw.pixel(0, 1), red);
  EXPECT_EQUAL(raw.pixel(1, 0), red);
  EXPECT_EQUAL(raw.pixel(1, 1), red);
}
END_TEST(test_pixmap_red)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap_green) {
  pixmap img(2, 2);
  graphics(img).clear(color::green);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGBA));

  rgbamap rgb = img.get<PixelFormat::RGBA>();

  const_image_data<PixelFormat::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), green);
  EXPECT_EQUAL(raw.pixel(0, 1), green);
  EXPECT_EQUAL(raw.pixel(1, 0), green);
  EXPECT_EQUAL(raw.pixel(1, 1), green);
}
END_TEST(test_pixmap_green)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap_blue) {
  pixmap img(2, 2);
  graphics(img).clear(color::blue);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGBA));

  rgbamap rgb = img.get<PixelFormat::RGBA>();

  const_image_data<PixelFormat::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), blue);
  EXPECT_EQUAL(raw.pixel(0, 1), blue);
  EXPECT_EQUAL(raw.pixel(1, 0), blue);
  EXPECT_EQUAL(raw.pixel(1, 1), blue);
}
END_TEST(test_pixmap_blue)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap_white) {
  pixmap img(2, 2);
  graphics(img).clear(color::white);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGBA));

  rgbamap rgb = img.get<PixelFormat::RGBA>();

  const_image_data<PixelFormat::RGBA> raw = static_cast<const rgbamap&>(rgb).get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), white);
  EXPECT_EQUAL(raw.pixel(0, 1), white);
  EXPECT_EQUAL(raw.pixel(1, 0), white);
  EXPECT_EQUAL(raw.pixel(1, 1), white);

}
END_TEST(test_pixmap_white)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap) {
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
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGBA));

  rgbamap rgb = img.get<PixelFormat::RGBA>();

  auto raw = rgb.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), black);
  EXPECT_EQUAL(raw.pixel(0, 1), black);
  EXPECT_EQUAL(raw.pixel(1, 0), black);
  EXPECT_EQUAL(raw.pixel(1, 1), black);
}
END_TEST(test_pixmap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap_draw) {
#ifdef X11
# define EMPTY "000000ff"
# define BLUE  "ff0000ff"
#endif // X11
#ifdef WIN32
# define EMPTY "00000000"
# define BLUE  "ff000000"
#endif // WIN32

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
  EXPECT_EQUAL(img.size(), core::size(6, 6));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(6, 6, 24, PixelFormat::RGBA));

#ifdef X11
  XImage* xim = XGetImage(core::global::get_instance(), img.get_id(), 0, 0, 6, 6, AllPlanes, ZPixmap);
#endif // X11
#ifdef WIN32
  BITMAP bmi;
  GetObject(img.get_id(), sizeof (BITMAP), &bmi);
  blob data;
  data.resize(bmi.bmHeight * bmi.bmWidthBytes);
  GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
#endif // WIN32


  std::ostringstream buffer;
  for (int y = 0; y < IF_WIN32_ELSE(bmi.bmHeight, xim->height); ++y) {
    if (y > 0) {
      buffer << '\n';
    }
    for (int x = 0; x < IF_WIN32_ELSE(bmi.bmWidthBytes, xim->bytes_per_line); x+=4) {
      if (x > 0) {
        buffer << ' ';
      }
      for (int j = 0; j < 4; ++j) {
        buffer << std::setw(2) << std::setfill('0') << std::hex 
               << (unsigned short)(unsigned char)IF_WIN32_ELSE(data, xim->data)[y * IF_WIN32_ELSE(bmi.bmWidthBytes, xim->bytes_per_line) + x + j];
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

  rgbamap rgb = img.get<PixelFormat::RGBA>();

  auto raw = rgb.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), black) ;
  EXPECT_NOT_EQUAL(raw.pixel(2, 2), black);

  int red_pix = 0;
  int green_pix = 0;
  int blue_pix = 0;

  const uint32_t rows = raw.get_info().height;
  const uint32_t columns = raw.get_info().width;
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
END_TEST(test_pixmap_draw)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap2bitmap) {
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

  bgrmap bgr = pix.get<PixelFormat::BGR>();
  auto bgr_raw = bgr.get_data();
  EXPECT_EQUAL(bgr_raw.pixel(0, 0), pixel::color<pixel::bgr_pixel>::black);
  EXPECT_EQUAL(bgr_raw.pixel(0, 1), pixel::color<pixel::bgr_pixel>::white);
  EXPECT_EQUAL(bgr_raw.pixel(1, 0), pixel::color<pixel::bgr_pixel>::white);
  EXPECT_EQUAL(bgr_raw.pixel(1, 1), pixel::color<pixel::bgr_pixel>::black);

  rgbmap rgb = pix.get<PixelFormat::RGB>();
  auto rgb_raw = rgb.get_data();
  EXPECT_EQUAL(rgb_raw.pixel(0, 0), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 1), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 0), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 1), pixel::color<pixel::rgb_pixel>::black);

  bwmap bw1 = pix.get<PixelFormat::BW>();

  const_image_data<PixelFormat::BW> bw_raw = static_cast<const bwmap&>(bw1).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::bw_pixel::black);

  bitmap img = bw1;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), PixelFormat::BW));

  {
    graphics g(img);
    EXPECT_EQUAL(g.get_pixel({0, 0}), color::black);
    EXPECT_EQUAL(g.get_pixel({0, 1}), color::white);
    EXPECT_EQUAL(g.get_pixel({1, 0}), color::white);
    EXPECT_EQUAL(g.get_pixel({1, 1}), color::black);
  }

#ifdef X11
  XImage* xim = XGetImage(core::global::get_instance(), img.get_id(), 0, 0, 2, 2, AllPlanes, ZPixmap);
#endif // X11
#ifdef WIN32
  BITMAP bmi;
  GetObject(img.get_id(), sizeof (BITMAP), &bmi);
  blob data;
  data.resize(bmi.bmHeight * bmi.bmWidthBytes);
  int result = GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
  if (result != data.size()) {
    LogError << "GetBitmapBits returned " << result << " expected:" << data.size();
  }
#endif // WIN32

  std::ostringstream buffer;
  for (int i = 0; i < IF_WIN32_ELSE(4, 8); ++i) {
    buffer << std::setw(2) << std::setfill('0') << std::hex << (int)IF_WIN32_ELSE(data, xim->data)[i] << ' ';
  }
#ifdef X11
  XDestroyImage(xim);
#endif // X11

  EXPECT_EQUAL(buffer.str(), IF_WIN32_ELSE("40 00 80 00 ", "02 00 00 00 01 00 00 00 "));

  bwmap bw2 = img.get();

  const_image_data<PixelFormat::BW> bw2_raw = static_cast<const bwmap&>(bw2).get_data();
  EXPECT_EQUAL(bw2_raw.pixel(0, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw2_raw.pixel(0, 1), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 1), pixel::bw_pixel::black);

}
END_TEST(test_pixmap2bitmap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap2pixmap) {
  bwmap bw(2,2);
  using raw_type = image_data<PixelFormat::BW>;
  using raw_data_type = image_data<PixelFormat::BW>::raw_type;
  using row_type = image_data<PixelFormat::BW>::row_type;
  raw_type bw_raw = bw.get_data();
  bw_raw.pixel(0, 0) = pixel::bw_pixel::black;
  bw_raw.pixel(0, 1) = pixel::bw_pixel::white;
  bw_raw.pixel(1, 0) = pixel::bw_pixel::white;
  bw_raw.pixel(1, 1) = pixel::bw_pixel::black;

  pixmap pix = bw;
  rgbmap rgb = pix.get<PixelFormat::RGB>();
  auto rgb_raw = rgb.get_data();
  EXPECT_EQUAL(rgb_raw.pixel(0, 0), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 1), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 0), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 1), pixel::color<pixel::rgb_pixel>::black);

}
END_TEST(test_bitmap2pixmap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_scale2pixmap) {
  bwmap bw(2,2);
  using raw_type = image_data<PixelFormat::BW>;
  using raw_data_type = image_data<PixelFormat::BW>::raw_type;
  using row_type = image_data<PixelFormat::BW>::row_type;
  raw_type bw_raw = bw.get_data();
  bw_raw.pixel(0, 0) = pixel::bw_pixel::black;
  bw_raw.pixel(0, 1) = pixel::bw_pixel::white;
  bw_raw.pixel(1, 0) = pixel::bw_pixel::white;
  bw_raw.pixel(1, 1) = pixel::bw_pixel::black;

  bwmap bw2(4, 4);
  bw2.stretch_from(bw);

  raw_type bw2_raw = bw2.get_data();
  EXPECT_EQUAL(bw2_raw.pixel(0, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw2_raw.pixel(0, 1), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw2_raw.pixel(1, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw2_raw.pixel(1, 1), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw2_raw.pixel(0, 2), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(0, 3), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 2), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 3), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(2, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(3, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(2, 1), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(3, 1), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(2, 2), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw2_raw.pixel(2, 3), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw2_raw.pixel(3, 2), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw2_raw.pixel(3, 3), pixel::bw_pixel::black);

  pixmap pix = bw2;
  rgbmap rgb = pix.get<PixelFormat::RGB>();
  auto rgb_raw = rgb.get_data();
  EXPECT_EQUAL(rgb_raw.pixel(0, 0), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 1), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(1, 0), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(1, 1), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 2), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(0, 3), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 2), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 3), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(2, 0), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(3, 0), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(2, 1), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(3, 1), pixel::color<pixel::rgb_pixel>::white);
  EXPECT_EQUAL(rgb_raw.pixel(2, 2), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(2, 3), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(3, 2), pixel::color<pixel::rgb_pixel>::black);
  EXPECT_EQUAL(rgb_raw.pixel(3, 3), pixel::color<pixel::rgb_pixel>::black);

}
END_TEST(test_bitmap_scale2pixmap)

// --------------------------------------------------------------------------

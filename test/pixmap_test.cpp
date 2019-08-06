
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
DECLARE_TEST(test_pixmap);
DECLARE_TEST(test_pixmap_draw);

DECLARE_TEST(test_pixmap2bitmap);

// --------------------------------------------------------------------------
TEST_MAIN() {
  RUN_TEST(test_bitmap_black);
  RUN_TEST(test_bitmap_white);
  RUN_TEST(test_pixmap);
  RUN_TEST(test_pixmap_draw);
  RUN_TEST(test_pixmap2bitmap);
} TEST_MAIN_END()

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_black) {
  using namespace gui;
  using namespace gui::draw;

  bitmap img(2, 2);
  graphics(img).clear(color::black);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 4, PixelFormat::BW));

  bwmap bw = img.get();

  const_image_data<PixelFormat::BW> bw_raw = static_cast<const bwmap&>(bw).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::bw_pixel::black);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::black);
}
END_TEST(test_bitmap_black)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_white) {
  using namespace gui;
  using namespace gui::draw;

  bitmap img(2, 2);
  graphics(img).clear(color::white);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 4, PixelFormat::BW));

  bwmap bw = img.get();

  const_image_data<PixelFormat::BW> bw_raw = static_cast<const bwmap&>(bw).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::white);
}
END_TEST(test_bitmap_white)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap) {
  using namespace gui;
  using namespace gui::draw;

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
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BGR);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::BGR));

  bgrmap rgb = img.get<PixelFormat::BGR>();

  auto raw = rgb.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::bgr_pixel::black);
  EXPECT_EQUAL(raw.pixel(0, 1), pixel::bgr_pixel::black);
  EXPECT_EQUAL(raw.pixel(1, 0), pixel::bgr_pixel::black);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::bgr_pixel::black);
}
END_TEST(test_pixmap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap_draw) {
  using namespace gui;
  using namespace gui::draw;

  pixmap img(6, 6);
  {
    graphics g(img);
    g.clear(color::black);
    g.draw(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue, color::blue);
    EXPECT_EQUAL(g.get_pixel({0, 0}), color::black);
    EXPECT_EQUAL(g.get_pixel({0, 1}), color::black);
    EXPECT_EQUAL(g.get_pixel({1, 0}), color::black);
    EXPECT_EQUAL(g.get_pixel({1, 1}), color::black);
    EXPECT_EQUAL(g.get_pixel({2, 2}), color::blue);
    EXPECT_EQUAL(g.get_pixel({3, 3}), color::blue);
    EXPECT_EQUAL(g.get_pixel({4, 4}), color::blue);
    EXPECT_EQUAL(g.get_pixel({5, 5}), color::black);
  }

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(6, 6));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BGR);
  EXPECT_EQUAL(img.get_info(), bitmap_info(6, 6, 20, PixelFormat::BGR));

  XImage* xim = XGetImage(core::global::get_instance(), img.get_id(), 0, 0, 6, 6, AllPlanes, ZPixmap);
  std::ostringstream buffer;
  for (int y = 0; y < xim->height; ++y) {
    if (y > 0) {
      buffer << '\n';
    }
    for (int x = 0; x < xim->bytes_per_line; x+=4) {
      if (x > 0) {
        buffer << ' ';
      }
      for (int j = 0; j < 4; ++j) {
        buffer << std::setw(2) << std::setfill('0') << std::hex << (unsigned short)(unsigned char)xim->data[y * xim->bytes_per_line + x + j];
      }
    }
  }
  XDestroyImage(xim);

  EXPECT_EQUAL(buffer.str(), "000000ff 000000ff 000000ff 000000ff 000000ff 000000ff\n"
                             "000000ff 000000ff 000000ff 000000ff 000000ff 000000ff\n"
                             "000000ff 000000ff ff0000ff ff0000ff ff0000ff 000000ff\n"
                             "000000ff 000000ff ff0000ff ff0000ff ff0000ff 000000ff\n"
                             "000000ff 000000ff ff0000ff ff0000ff ff0000ff 000000ff\n"
                             "000000ff 000000ff 000000ff 000000ff 000000ff 000000ff");

  bgramap rgb = img.get<PixelFormat::BGRA>();

  auto raw = rgb.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::bgra_pixel::black);
  EXPECT_NOT_EQUAL(raw.pixel(2, 2), pixel::bgra_pixel::black);


  int red_pix = 0;
  int green_pix = 0;
  int blue_pix = 0;

  const uint32_t rows = raw.get_info().height;
  const uint32_t columns = raw.get_info().width;
  for (uint32_t r = 0; r < 6; ++r) {
    auto row = raw.row(r);
    for (uint32_t c = 0; c < 6; ++c) {
      const pixel::bgra_pixel pix = row[c];
      if (pix.red > 0) ++red_pix;
      if (pix.green > 0) ++green_pix;
      if (pix.blue > 0) ++blue_pix;
    }
  }

  EXPECT_EQUAL(red_pix, 0);
  EXPECT_EQUAL(green_pix, 0);
  EXPECT_EQUAL(blue_pix, 9);
}
END_TEST(test_pixmap_draw)

// --------------------------------------------------------------------------
DEFINE_TEST(test_pixmap2bitmap) {
  using namespace gui;
  using namespace gui::draw;

  pixmap pix(2, 2);
  {
    graphics g(pix);
    g.clear(color::white);
    EXPECT_EQUAL(g.get_pixel({0, 0}), color::white);
    EXPECT_EQUAL(g.get_pixel({0, 1}), color::white);
    EXPECT_EQUAL(g.get_pixel({1, 0}), color::white);
    EXPECT_EQUAL(g.get_pixel({1, 1}), color::white);
  }

  bgrmap bgr = pix.get<PixelFormat::BGR>();
  auto bgr_raw = bgr.get_data();
  EXPECT_EQUAL(bgr_raw.pixel(0, 0), pixel::bgr_pixel::white);
  EXPECT_EQUAL(bgr_raw.pixel(0, 1), pixel::bgr_pixel::white);
  EXPECT_EQUAL(bgr_raw.pixel(1, 0), pixel::bgr_pixel::white);
  EXPECT_EQUAL(bgr_raw.pixel(0, 1), pixel::bgr_pixel::white);

  bwmap bw1 = pix.get<PixelFormat::BW>();

  const_image_data<PixelFormat::BW> bw_raw = static_cast<const bwmap&>(bw1).get_data();
  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::bw_pixel::white);

  bitmap img = bw1;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 4, PixelFormat::BW));

  {
    graphics g(img);
    for (int y = 0; y < 2; ++y) {
      for (int x = 0; x < 2; ++x) {
        EXPECT_EQUAL(g.get_pixel({x, y}), static_cast<int>(pixel::bw_pixel::white));
      }
    }
  }

  XImage* xim = XGetImage(core::global::get_instance(), img.get_id(), 0, 0, 2, 2, AllPlanes, ZPixmap);
  std::ostringstream buffer;
  for (int i = 0; i < 8; ++i) {
    buffer << std::setw(2) << std::setfill('0') << std::hex << (int)xim->data[i] << ' ';
  }
  XDestroyImage(xim);

  EXPECT_EQUAL(buffer.str(), "03 00 00 00 03 00 00 00 ");

  bwmap bw2 = img.get();

  const_image_data<PixelFormat::BW> bw2_raw = static_cast<const bwmap&>(bw2).get_data();
  EXPECT_EQUAL(bw2_raw.pixel(0, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(0, 1), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 0), pixel::bw_pixel::white);
  EXPECT_EQUAL(bw2_raw.pixel(0, 1), pixel::bw_pixel::white);

}
END_TEST(test_pixmap2bitmap)

// --------------------------------------------------------------------------


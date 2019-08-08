
#include <gui/draw/datamap.h>
#include <gui/draw/graphics.h>
#include <testlib/testlib.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
DECLARE_TEST(test_bw);
DECLARE_TEST(test_gray);
DECLARE_TEST(test_rgb);
DECLARE_TEST(test_bgr);
DECLARE_TEST(test_rgba);
DECLARE_TEST(test_bgra);

DECLARE_TEST(test_gray2rgb);
DECLARE_TEST(test_rgb2gray);
DECLARE_TEST(test_rgb2bgr);

// --------------------------------------------------------------------------
TEST_MAIN(datamap)
  RUN_TEST(test_bw);
  RUN_TEST(test_gray);
  RUN_TEST(test_rgb);
  RUN_TEST(test_bgr);
  RUN_TEST(test_rgba);
  RUN_TEST(test_bgra);
  RUN_TEST(test_gray2rgb);
  RUN_TEST(test_rgb2gray);
  RUN_TEST(test_rgb2bgr);
TEST_MAIN_END(datamap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bw)
  using namespace gui;
  using namespace gui::draw;

  bwmap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 4, PixelFormat::BW));

  image_data<PixelFormat::BW> raw = img.get_data();
  basepp::array_wrapper<pixel::bw_pixel> row = raw.row(0);
  pixel::bw_pixel px = row[0];
  EXPECT_EQUAL(px, pixel::bw_pixel::black);

  row[0] = pixel::bw_pixel::white;
  px = row[0];
  EXPECT_EQUAL(px, pixel::bw_pixel::white);

  px = static_cast<const bwmap&>(img).get_data().pixel(0, 0);
  EXPECT_EQUAL(px, pixel::bw_pixel::white);
END_TEST(test_bw)

// --------------------------------------------------------------------------
DEFINE_TEST(test_gray)
  using namespace gui;
  using namespace gui::draw;

  graymap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::GRAY);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 4, PixelFormat::GRAY));

  image_data<PixelFormat::GRAY> raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::gray_pixel::black);

  raw.pixel(0, 0) = pixel::gray_pixel::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::gray_pixel::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::gray_pixel::black);
END_TEST(test_gray)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgb)
  using namespace gui;
  using namespace gui::draw;

  rgbmap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGB);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGB));

  auto raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::rgb_pixel::black);

  raw.pixel(0, 0) = pixel::rgb_pixel::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::rgb_pixel::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::rgb_pixel::black);
END_TEST(test_rgb)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bgr)
  using namespace gui;
  using namespace gui::draw;

  bgrmap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BGR);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::BGR));

  auto raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::bgr_pixel::black);

  raw.pixel(0, 0) = pixel::bgr_pixel::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::bgr_pixel::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::bgr_pixel::black);
END_TEST(test_bgr)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgba)
  using namespace gui;
  using namespace gui::draw;

  rgbamap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 32);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGBA));

  auto raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::rgba_pixel::black);

  raw.pixel(0, 0) = pixel::rgba_pixel::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::rgba_pixel::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::rgba_pixel::black);
END_TEST(test_rgba)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bgra)
  using namespace gui;
  using namespace gui::draw;

  bgramap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.size(), core::size(2, 2));
  EXPECT_EQUAL(img.depth(), 32);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BGRA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, PixelFormat::BGRA));

  auto raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::bgra_pixel::black);

  raw.pixel(0, 0) = pixel::bgra_pixel::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::bgra_pixel::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::bgra_pixel::black);
END_TEST(test_bgra)

// --------------------------------------------------------------------------
DEFINE_TEST(test_gray2rgb)
  using namespace gui;
  using namespace gui::draw;

  graymap gray(2, 2);
  gray.fill({0x7F});
  auto rgb = gray.convert<PixelFormat::RGB>();

  EXPECT_TRUE(rgb.is_valid());
  EXPECT_EQUAL(rgb.size(), core::size(2, 2));
  EXPECT_EQUAL(rgb.depth(), 24);
  EXPECT_EQUAL(rgb.pixel_format(), PixelFormat::RGB);
  EXPECT_EQUAL(rgb.get_info(), bitmap_info(2, 2, 8, PixelFormat::RGB));

  auto raw = rgb.get_data();
  pixel::rgb_pixel expected = {0x7F, 0x7F, 0x7F};
  EXPECT_EQUAL(raw.pixel(0, 0), expected);

END_TEST(test_gray2rgb)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgb2gray)
  using namespace gui;
  using namespace gui::draw;

  rgbmap rgb(2, 2);
  rgb.fill({0x20, 0x40, 0x60});
  auto gray = rgb.convert<PixelFormat::GRAY>();

  EXPECT_TRUE(gray.is_valid());
  EXPECT_EQUAL(gray.size(), core::size(2, 2));
  EXPECT_EQUAL(gray.depth(), 8);
  EXPECT_EQUAL(gray.pixel_format(), PixelFormat::GRAY);
  EXPECT_EQUAL(gray.get_info(), bitmap_info(2, 2, 4, PixelFormat::GRAY));

  auto raw = gray.get_data();
  pixel::gray_pixel expected = {0x40};
  EXPECT_EQUAL(raw.pixel(0, 0), expected);

END_TEST(test_rgb2gray)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgb2bgr)
  using namespace gui;
  using namespace gui::draw;

  rgbmap rgb(2, 2);
  rgb.fill({0x20, 0x40, 0x60});
  auto bgr = rgb.convert<PixelFormat::BGR>();

  EXPECT_TRUE(bgr.is_valid());
  EXPECT_EQUAL(bgr.size(), core::size(2, 2));
  EXPECT_EQUAL(bgr.depth(), 24);
  EXPECT_EQUAL(bgr.pixel_format(), PixelFormat::BGR);
  EXPECT_EQUAL(bgr.get_info(), bitmap_info(2, 2, 8, PixelFormat::BGR));

  auto raw = bgr.get_data();
  pixel::bgr_pixel expected = {0x60, 0x40, 0x20};
  EXPECT_EQUAL(raw.pixel(0, 0), expected);

END_TEST(test_rgb2bgr)

// --------------------------------------------------------------------------


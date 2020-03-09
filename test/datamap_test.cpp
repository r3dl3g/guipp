
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
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), pixel_format_t::BW));

  image_data<pixel_format_t::BW> raw = img.get_data();
  auto row = raw.row(0);
  pixel::mono px = row[0];
  EXPECT_EQUAL(px, pixel::mono::black);

  row[0] = pixel::mono::white;
  px = row[0];
  EXPECT_EQUAL(px, pixel::mono::white);

  px = static_cast<const bwmap&>(img).get_data().pixel(0, 0);
  EXPECT_EQUAL(px, pixel::mono::white);
END_TEST(test_bw)

// --------------------------------------------------------------------------
DEFINE_TEST(test_gray)
  using namespace gui;
  using namespace gui::draw;

  graymap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::GRAY);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), pixel_format_t::GRAY));

  image_data<pixel_format_t::GRAY> raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::gray>::black);

  raw.pixel(0, 0) = pixel::color<pixel::gray>::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::gray>::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::color<pixel::gray>::black);
END_TEST(test_gray)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgb)
  using namespace gui;
  using namespace gui::draw;

  rgbmap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::RGB);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(6, 8), pixel_format_t::RGB));

  auto raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::rgb>::black);

  raw.pixel(0, 0) = pixel::color<pixel::rgb>::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::color<pixel::rgb>::black);
END_TEST(test_rgb)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bgr)
  using namespace gui;
  using namespace gui::draw;

  bgrmap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BGR);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(6, 8), pixel_format_t::BGR));

  auto raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::bgr>::black);

  raw.pixel(0, 0) = pixel::color<pixel::bgr>::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::bgr>::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::color<pixel::bgr>::black);
END_TEST(test_bgr)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgba)
  using namespace gui;
  using namespace gui::draw;

  rgbamap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::RGBA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, pixel_format_t::RGBA));

  auto raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::rgba>::black);

  raw.pixel(0, 0) = pixel::color<pixel::rgba>::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::rgba>::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::color<pixel::rgba>::black);
END_TEST(test_rgba)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bgra)
  using namespace gui;
  using namespace gui::draw;

  bgramap img(2, 2);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BGRA);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, 8, pixel_format_t::BGRA));

  auto raw = img.get_data();
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::bgra>::black);

  raw.pixel(0, 0) = pixel::color<pixel::bgra>::white;
  EXPECT_EQUAL(raw.pixel(0, 0), pixel::color<pixel::bgra>::white);
  EXPECT_EQUAL(raw.pixel(1, 1), pixel::color<pixel::bgra>::black);
END_TEST(test_bgra)

// --------------------------------------------------------------------------
DEFINE_TEST(test_gray2rgb)
  using namespace gui;
  using namespace gui::draw;

  graymap img(2, 2);
  img.fill({byte(0x7F)});
  auto rgb = img.convert<pixel_format_t::RGB>();

  EXPECT_TRUE(rgb.is_valid());
  EXPECT_EQUAL(rgb.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(rgb.depth(), 24);
  EXPECT_EQUAL(rgb.pixel_format(), pixel_format_t::RGB);
  EXPECT_EQUAL(rgb.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(6, 8), pixel_format_t::RGB));

  auto raw = rgb.get_data();
  pixel::rgb expected = {0x7F, 0x7F, 0x7F};
  EXPECT_EQUAL(raw.pixel(0, 0), expected);

END_TEST(test_gray2rgb)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgb2gray)
  using namespace gui;
  using namespace gui::draw;

  rgbmap rgb(2, 2);
  rgb.fill({0x20, 0x40, 0x60});
  auto img = rgb.convert<pixel_format_t::GRAY>();

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::GRAY);
  EXPECT_EQUAL(img.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(2, 4), pixel_format_t::GRAY));

  auto raw = img.get_data();
  pixel::gray expected = {byte(0x40)};
  EXPECT_EQUAL(raw.pixel(0, 0), expected);

END_TEST(test_rgb2gray)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgb2bgr)
  using namespace gui;
  using namespace gui::draw;

  rgbmap rgb(2, 2);
  rgb.fill({0x20, 0x40, 0x60});
  auto bgr = rgb.convert<pixel_format_t::BGR>();

  EXPECT_TRUE(bgr.is_valid());
  EXPECT_EQUAL(bgr.native_size(), core::native_size(2, 2));
  EXPECT_EQUAL(bgr.depth(), 24);
  EXPECT_EQUAL(bgr.pixel_format(), pixel_format_t::BGR);
  EXPECT_EQUAL(bgr.get_info(), bitmap_info(2, 2, IF_WIN32_ELSE(6, 8), pixel_format_t::BGR));

  auto raw = bgr.get_data();
  pixel::bgr expected = {0x60, 0x40, 0x20};
  EXPECT_EQUAL(raw.pixel(0, 0), expected);

END_TEST(test_rgb2bgr)

// --------------------------------------------------------------------------



#include <gui/draw/datamap.h>
#include <testlib/testlib.h>
#include <testlib/image_test_lib.h>
#include <gui/io/pnm.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
enum stretch {
  stretch_1 = 0,
  stretch_1_6 = 1,
  stretch_2 = 2,
  stretch_3 = 3,
  stretch_0_8 = 4,
  stretch_0_6 = 5
};

std::ostream& operator<< (std::ostream& out, const stretch& s) {
  switch (s) {
    case stretch_1: out << "1"; break;
    case stretch_1_6: out << "1.6"; break;
    case stretch_2: out << "2"; break;
    case stretch_3: out << "3"; break;
    case stretch_0_8: out << "0.8"; break;
    case stretch_0_6: out << "0.6"; break;
  }
  return out;
}

float stretcht_factor(stretch st) {
  static const float factors[] = {1.0F, 1.6F, 2.0F, 3.0F, 0.8F, 0.6F};
  return factors[static_cast<int>(st)];
}

// --------------------------------------------------------------------------
DECLARE_TEST(test_bw);
DECLARE_TEST(test_gray);
DECLARE_TEST(test_rgb);
DECLARE_TEST(test_rgba);

DECLARE_TEST(test_bw_bilinear);
DECLARE_TEST(test_gray_bilinear);
DECLARE_TEST(test_rgb_bilinear);
DECLARE_TEST(test_rgba_bilinear);

stretch stretch_f = stretch_1;

// --------------------------------------------------------------------------
TEST_MAIN(stretch) {
  for (stretch_f = stretch_1; stretch_f <= stretch_0_6; stretch_f = (stretch)(stretch_f + 1)) {
    RUN_TEST(test_bw);
    RUN_TEST(test_gray);
    RUN_TEST(test_rgb);
    RUN_TEST(test_rgba);
  }
  for (stretch_f = stretch_1; stretch_f <= stretch_0_6; stretch_f = (stretch)(stretch_f + 1)) {
    RUN_TEST(test_bw_bilinear);
    RUN_TEST(test_gray_bilinear);
    RUN_TEST(test_rgb_bilinear);
    RUN_TEST(test_rgba_bilinear);
  }
} TEST_MAIN_END(stretch)

// --------------------------------------------------------------------------
using namespace testing;

colormap expected_nearest[] = {
  CM({{_, _, _, _, _},
      {_, W, W, W, _},
      {_, W, _, W, _},
      {_, W, W, W, _},
      {_, _, _, _, _}})
  , CM({{_, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _},
        {_, _, W, W, W, W, _, _},
        {_, _, W, _, _, W, _, _},
        {_, _, W, _, _, W, _, _},
        {_, _, W, W, W, W, _, _},
        {_, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _}})
  , CM({{_, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _},
        {_, _, W, W, W, W, W, W, _, _},
        {_, _, W, W, W, W, W, W, _, _},
        {_, _, W, W, _, _, W, W, _, _},
        {_, _, W, W, _, _, W, W, _, _},
        {_, _, W, W, W, W, W, W, _, _},
        {_, _, W, W, W, W, W, W, _, _},
        {_, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _}})
  , CM({{_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, _, _, _, W, W, W, _, _, _},
        {_, _, _, W, W, W, _, _, _, W, W, W, _, _, _},
        {_, _, _, W, W, W, _, _, _, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _}})
  , CM({{_, _, _, _},
        {_, W, W, _},
        {_, W, W, _},
        {_, _, _, _}})
  , CM({{_, _, _},
        {_, _, _},
        {_, _, _}})
};

// --------------------------------------------------------------------------
DEFINE_TEST(test_bw)
  using namespace gui;
  using namespace gui::draw;

  bwmap img = colormap2datamap<PixelFormat::BW>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);

  auto factor = stretcht_factor(stretch_f);
  LogWarng << "Stretching with factor " << factor;

  auto expected_size = img.native_size() * factor;
  bwmap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 1);
  EXPECT_EQUAL(stretched.pixel_format(), PixelFormat::BW);

  auto buffer = datamap2colormap(stretched);
  EXPECT_EQUAL(buffer, expected_nearest[stretch_f]);
END_TEST(test_bw)

// --------------------------------------------------------------------------
DEFINE_TEST(test_gray) {
  using namespace gui;
  using namespace gui::draw;

  graymap img = colormap2datamap<PixelFormat::GRAY>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::GRAY);

  auto factor = stretcht_factor(stretch_f);
  LogWarng << "Stretching with factor " << factor;

  auto expected_size = img.native_size() * factor;
  graymap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 8);
  EXPECT_EQUAL(stretched.pixel_format(), PixelFormat::GRAY);

  auto buffer = datamap2colormap(stretched);
  EXPECT_EQUAL(buffer, expected_nearest[stretch_f]);
} END_TEST(test_gray)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgb) {
  using namespace gui;
  using namespace gui::draw;

  rgbmap img = colormap2datamap<PixelFormat::RGB>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGB);

  auto factor = stretcht_factor(stretch_f);
  LogWarng << "Stretching with factor " << factor;

  auto expected_size = img.native_size() * factor;
  rgbmap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), PixelFormat::RGB);

  auto buffer = datamap2colormap(stretched);
  EXPECT_EQUAL(buffer, expected_nearest[stretch_f]);
} END_TEST(test_rgb)

// --------------------------------------------------------------------------// --------------------------------------------------------------------------
DEFINE_TEST(test_rgba) {
  using namespace gui;
  using namespace gui::draw;

  rgbamap img = colormap2datamap<PixelFormat::RGBA>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);

  auto factor = stretcht_factor(stretch_f);
  LogWarng << "Stretching with factor " << factor;

  auto expected_size = img.native_size() * factor;
  rgbamap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), PixelFormat::RGBA);

  auto buffer = datamap2colormap(stretched);
  EXPECT_EQUAL(buffer, expected_nearest[stretch_f]);
} END_TEST(test_rgba)

// --------------------------------------------------------------------------// --------------------------------------------------------------------------
graysmap expected_bilinear[] = {
  GM({{0, 0,   0,   0,   0},
      {0, 255, 255, 255, 0},
      {0, 255, 0,   255, 0},
      {0, 255, 255, 255, 0},
      {0, 0,   0,   0,   0}})
  , GM({{0,   0,   0,   0,   0,   0,   0,   0},
        {0, 121, 177, 177, 177, 177, 121,   0},
        {0, 177, 255, 250, 250, 255, 177,   0},
        {0, 177, 250, 192, 192, 250, 177,   0},
        {0, 177, 250, 192, 192, 250, 177,   0},
        {0, 177, 255, 250, 250, 255, 177,   0},
        {0, 121, 177, 177, 177, 177, 121,   0},
        {0,   0,   0,   0,   0,   0,   0,   0}})
  , GM({{0,   0,   0,   0,   0,   0,   0,   0,   0, 0},
        {0,  71, 120, 137, 137, 137, 137, 120,  71, 0},
        {0, 120, 198, 225, 225, 225, 225, 198, 120, 0},
        {0, 137, 225, 248, 233, 233, 248, 225, 137, 0},
        {0, 137, 225, 233, 177, 177, 233, 225, 137, 0},
        {0, 137, 225, 233, 177, 177, 233, 225, 137, 0},
        {0, 137, 225, 248, 233, 233, 248, 225, 137, 0},
        {0, 120, 198, 225, 225, 225, 225, 198, 120, 0},
        {0,  71, 120, 137, 137, 137, 137, 120,  71, 0},
        {0,   0,   0,   0,   0,   0,   0,   0,   0, 0}})
  , GM({{0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0},
        {0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0},
        {0, 0,  94, 130, 156, 156, 156, 156, 156, 156, 156, 130,  94, 0, 0},
        {0, 0, 130, 178, 213, 213, 213, 213, 213, 213, 213, 178, 130, 0, 0},
        {0, 0, 156, 213, 255, 255, 255, 255, 255, 255, 255, 213, 156, 0, 0},
        {0, 0, 156, 213, 255, 242, 228, 213, 228, 242, 255, 213, 156, 0, 0},
        {0, 0, 156, 213, 255, 228, 197, 156, 197, 228, 255, 213, 156, 0, 0},
        {0, 0, 156, 213, 255, 213, 156,   0, 156, 213, 255, 213, 156, 0, 0},
        {0, 0, 156, 213, 255, 228, 197, 156, 197, 228, 255, 213, 156, 0, 0},
        {0, 0, 156, 213, 255, 242, 228, 213, 228, 242, 255, 213, 156, 0, 0},
        {0, 0, 156, 213, 255, 255, 255, 255, 255, 255, 255, 213, 156, 0, 0},
        {0, 0, 130, 178, 213, 213, 213, 213, 213, 213, 213, 178, 130, 0, 0},
        {0, 0,  94, 130, 156, 156, 156, 156, 156, 156, 156, 130,  94, 0, 0},
        {0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0},
        {0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0}})
  , GM({{34,  99,  99, 34},
        {99, 239, 239, 99},
        {99, 239, 239, 99},
        {34,  99,  99, 34}})
  , GM({{94, 156, 94},
        {156, 0, 156},
        {94, 156, 94}})
};

// --------------------------------------------------------------------------
DEFINE_TEST(test_bw_bilinear)
  using namespace gui;
  using namespace gui::draw;

  bwmap img = colormap2datamap<PixelFormat::BW>(expected_nearest[0]);

//  io::ofpnm<io::PNM::P1>(ostreamfmt("mono_" << stretch_f)) << img;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);

  auto factor = stretcht_factor(stretch_f);
  LogWarng << "Stretching with factor " << factor;

  auto expected_size = img.native_size() * factor;
  bwmap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bilinear>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 1);
  EXPECT_EQUAL(stretched.pixel_format(), PixelFormat::BW);

  auto buffer = datamap2graysmap(stretched);
  EXPECT_EQUAL(buffer, expected_bilinear[stretch_f]);
END_TEST(test_bw_bilinear)

// --------------------------------------------------------------------------
DEFINE_TEST(test_gray_bilinear) {
  using namespace gui;
  using namespace gui::draw;

  graymap img = colormap2datamap<PixelFormat::GRAY>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::GRAY);

  auto factor = stretcht_factor(stretch_f);
  LogWarng << "Stretching with factor " << factor;

  auto expected_size = img.native_size() * factor;
  graymap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bilinear>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 8);
  EXPECT_EQUAL(stretched.pixel_format(), PixelFormat::GRAY);

  io::ofpnm<io::PNM::P2>(ostreamfmt("gray_" << stretch_f)) << stretched;

  auto buffer = datamap2graysmap(stretched);
  EXPECT_EQUAL(buffer, expected_bilinear[stretch_f]);
} END_TEST(test_gray_bilinear)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgb_bilinear) {
  using namespace gui;
  using namespace gui::draw;

  rgbmap img = colormap2datamap<PixelFormat::RGB>(expected_nearest[0]);

//  io::ofpnm<io::PNM::P3>(ostreamfmt("rgb_" << stretch_f)) << img;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGB);

  auto factor = stretcht_factor(stretch_f);
  LogWarng << "Stretching with factor " << factor;

  auto expected_size = img.native_size() * factor;
  rgbmap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bilinear>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), PixelFormat::RGB);

  auto buffer = datamap2graysmap(stretched);
  EXPECT_EQUAL(buffer, expected_bilinear[stretch_f]);
} END_TEST(test_rgb_bilinear)

// --------------------------------------------------------------------------
DEFINE_TEST(test_rgba_bilinear) {
  using namespace gui;
  using namespace gui::draw;

  rgbamap img = colormap2datamap<PixelFormat::RGBA>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::RGBA);

  auto factor = stretcht_factor(stretch_f);
  LogWarng << "Stretching with factor " << factor;

  auto expected_size = img.native_size() * factor;
  rgbamap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bilinear>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), PixelFormat::RGBA);

  auto buffer = datamap2graysmap(stretched);
  EXPECT_EQUAL(buffer, expected_bilinear[stretch_f]);
} END_TEST(test_rgba_bilinear)

// --------------------------------------------------------------------------// --------------------------------------------------------------------------

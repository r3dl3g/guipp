
#include <gui/draw/datamap.h>
#include <gui/io/pnm.h>
#include <testlib/testlib.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
DECLARE_TEST(test_bwX1);
//DECLARE_TEST(test_bwX2);
//DECLARE_TEST(test_bwX05);
//DECLARE_TEST(test_bwX1_5);
//DECLARE_TEST(test_bwX3);
//DECLARE_TEST(test_bwX0_75);

DECLARE_TEST(test_grayX1);
//DECLARE_TEST(test_grayX2);
//DECLARE_TEST(test_grayX05);

//DECLARE_TEST(test_rgbX1);
//DECLARE_TEST(test_rgbX2);
//DECLARE_TEST(test_rgbX05);

//DECLARE_TEST(test_rgbaX1);
//DECLARE_TEST(test_bgrX1);
//DECLARE_TEST(test_bgraX1);

// --------------------------------------------------------------------------
TEST_MAIN(stretch)
  RUN_TEST(test_bwX1);
//  RUN_TEST(test_bwX2);
//  RUN_TEST(test_bwX05);
  RUN_TEST(test_grayX1);
//  RUN_TEST(test_grayX2);
//  RUN_TEST(test_grayX05);
//  RUN_TEST(test_rgbX1);
//  RUN_TEST(test_rgbX2);
//  RUN_TEST(test_rgbX05);
TEST_MAIN_END(stretch)

// --------------------------------------------------------------------------
enum class stretch : int {
  stretch_1 = 1000,
  stretch_1_5 = 1500,
  stretch_2 = 2000,
  stretch_3 = 3000,
  stretch_0_75 = 750,
  stretch_0_5 = 500
};

float stretcht_factor(stretch st) {
  return static_cast<float>(st) / 1000.0F;
}

template<typename T, size_t N>
inline std::string make_string (T(&t)[N]) {
  return std::string((const char*)t, N);
}

static const char test_bw_bits[] =
  "P1\n"
  "# pnm created by gui++\n"
  "8 8\n"
  "0 0 0 0 0 0 0 0 \n"
  "0 1 1 1 0 0 0 0 \n"
  "0 1 0 0 1 0 0 0 \n"
  "0 1 0 0 0 1 0 0 \n"
  "0 1 0 0 0 0 1 0 \n"
  "0 0 1 0 0 0 1 0 \n"
  "0 0 0 1 1 1 1 0 \n"
  "0 0 0 0 0 0 0 0 \n";

// --------------------------------------------------------------------------
DEFINE_TEST(test_bwX1)
  using namespace gui;
  using namespace gui::draw;
  using namespace gui::io;

  const stretch stretch_f = stretch::stretch_1;

  std::string in = make_string(test_bw_bits);

  bwmap img;
  std::istringstream(in) >> ipnm<PixelFormat::BW>(img);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(8, 8));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);

  auto expected_size = img.native_size() * stretcht_factor(stretch_f);
  bwmap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(img.native_size(), expected_size);
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::BW);

  std::string out = ostreamfmt((opnm<false, PixelFormat::BW>(stretched)));
  TEST_EQUAL(out, in);
END_TEST(test_bwX1)

static const char test_gray_bits[] =
  "P2\n"
  "# pnm created by gui++\n"
  "8 8 255\n"
  "0 0 0 0 0 0 0 0 \n"
  "0 255 255 255 0 0 0 0 \n"
  "0 255 0 0 255 0 0 0 \n"
  "0 255 0 0 0 255 0 0 \n"
  "0 255 0 0 0 0 255 0 \n"
  "0 0 255 0 0 0 255 0 \n"
  "0 0 0 255 255 255 255 0 \n"
  "0 0 0 0 0 0 0 0 \n";


// --------------------------------------------------------------------------
DEFINE_TEST(test_grayX1)
  using namespace gui;
  using namespace gui::draw;
  using namespace gui::io;

  const stretch stretch_f = stretch::stretch_1;

  std::string in = make_string(test_gray_bits);

  graymap img;
  std::istringstream(in) >> ipnm<PixelFormat::GRAY>(img);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(8, 8));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::GRAY);

  auto expected_size = img.native_size() * stretcht_factor(stretch_f);
  graymap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(img.native_size(), expected_size);
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), PixelFormat::GRAY);

  std::string out = ostreamfmt((opnm<false, PixelFormat::GRAY>(stretched)));
  TEST_EQUAL(out, in);
END_TEST(test_grayX1)

// --------------------------------------------------------------------------



#include <iomanip>

#ifdef GUIPP_QT
# include <QtGui/QBitmap>
#endif //GUIPP_QT

#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/bitmap.h>
#include <gui/draw/brush.h>
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <testlib/testlib.h>
#include <testlib/image_test_lib.h>


using namespace gui;
using namespace gui::draw;
using namespace testing;


inline pixel::rgb color2rgb (os::color c) {
  return pixel::rgb::build(c);
}

inline pixel::bgr color2bgr (os::color c) {
  return pixel::bgr::build(c);
}

const pixel::rgb black = color2rgb(color::black);
const pixel::rgb white = color2rgb(color::white);
const pixel::rgb red = color2rgb(color::red);
const pixel::rgb green = color2rgb(color::green);
const pixel::rgb blue = color2rgb(color::blue);

// --------------------------------------------------------------------------
template<typename T>
std::string data2hex (const T data, std::size_t len) {
  std::ostringstream buffer;
  for (int i = 0; i < len; ++i) {
    if (i) {
      buffer << ' ';
    }
    buffer << std::setw(2) << std::setfill('0') << std::hex << (int)data[i];
  }
  return buffer.str();
}

#ifdef GUIPP_QT
// --------------------------------------------------------------------------
colormap qimage2colormap (const QImage& img) {
  if (img.depth() == 1) {
    QImage i = img.convertToFormat(QImage::Format::Format_RGBA8888);
    return data2colormap((const char*)i.constBits(), i.depth(), i.bytesPerLine(), i.width(), i.height());
  } else {
    return data2colormap((const char*)img.constBits(), img.depth(), img.bytesPerLine(), img.width(), img.height());
  }
}

// --------------------------------------------------------------------------
void test_qpixmap2qimage () {
  QPixmap pix(4, 4);
  QPainter(&pix).fillRect(0, 0, 4, 4, Qt::white);

  EXPECT_EQUAL(qimage2colormap(pix.toImage()),
               CM({{W,W,W,W},
                   {W,W,W,W},
                   {W,W,W,W},
                   {W,W,W,W}}));

  QPainter(&pix).fillRect(1, 1, 2, 2, Qt::black);

  EXPECT_EQUAL(qimage2colormap(pix.toImage()),
               CM({{W,W,W,W},
                   {W,_,_,W},
                   {W,_,_,W},
                   {W,W,W,W}}));
}

// --------------------------------------------------------------------------
void test_qimage2qpixmap () {
  QPixmap pix(4, 4);
  {
    QPainter p(&pix);
    p.fillRect(p.viewport(), Qt::white);
    p.fillRect(1, 1, 2, 2, Qt::black);
  }

  QImage img = pix.toImage();
  EXPECT_EQUAL(qimage2colormap(img),
               CM({{W,W,W,W},
                   {W,_,_,W},
                   {W,_,_,W},
                   {W,W,W,W}}));

  QPainter(&pix).fillRect(0, 0, 4, 4, Qt::white);
  EXPECT_EQUAL(qimage2colormap(pix.toImage()),
               CM({{W,W,W,W},
                   {W,W,W,W},
                   {W,W,W,W},
                   {W,W,W,W}}));

  pix.convertFromImage(img);
  EXPECT_EQUAL(qimage2colormap(pix.toImage()),
               CM({{W,W,W,W},
                   {W,_,_,W},
                   {W,_,_,W},
                   {W,W,W,W}}));

  QPainter(&pix).fillRect(0, 0, 4, 4, Qt::white);
  EXPECT_EQUAL(qimage2colormap(pix.toImage()),
               CM({{W,W,W,W},
                   {W,W,W,W},
                   {W,W,W,W},
                   {W,W,W,W}}));

  std::vector<core::byte> buffer;
  buffer.assign(img.constBits(), img.constBits() +
#if QT_VERSION > QT_VERSION_CHECK(5, 13, 0)
                    img.sizeInBytes());
#else
                    img.byteCount());
#endif

  QImage img2((const core::byte*)buffer.data(), img.width(), img.height(), img.bytesPerLine(), img.format());
  pix.convertFromImage(img2);
  EXPECT_EQUAL(qimage2colormap(pix.toImage()),
               CM({{W,W,W,W},
                   {W,_,_,W},
                   {W,_,_,W},
                   {W,W,W,W}}));

}

// --------------------------------------------------------------------------
void test_qbitmap2qimage () {
  QBitmap pix(4, 4);
  QPainter(&pix).fillRect(0, 0, 4, 4, Qt::white);

  EXPECT_EQUAL(qimage2colormap(pix.toImage()),
               CM({{W,W,W,W},
                   {W,W,W,W},
                   {W,W,W,W},
                   {W,W,W,W}}));

  QPainter(&pix).fillRect(1, 1, 2, 2, Qt::black);

  EXPECT_EQUAL(qimage2colormap(pix.toImage()),
               CM({{W,W,W,W},
                   {W,_,_,W},
                   {W,_,_,W},
                   {W,W,W,W}}));
}

#endif //GUIPP_QT


// --------------------------------------------------------------------------
void test_byte_order_in_rgba () {
  pixel::rgba rgba = pixel::rgba::build(color::red);
  EXPECT_EQUAL(rgba.red, 255);
  EXPECT_EQUAL(rgba.green, 0);
  EXPECT_EQUAL(rgba.blue, 0);
  EXPECT_EQUAL(rgba.alpha, 0);

  gui::byte quad[4];
  memcpy(quad, &rgba, sizeof(rgba));
  EXPECT_EQUAL(quad[0], 255);
  EXPECT_EQUAL(quad[1], 0);
  EXPECT_EQUAL(quad[2], 0);
  EXPECT_EQUAL(quad[3], 0);
}

// --------------------------------------------------------------------------
void test_byte_order_in_argb () {
  pixel::argb rgba = pixel::argb::build(color::red);
  EXPECT_EQUAL(rgba.red, 255);
  EXPECT_EQUAL(rgba.green, 0);
  EXPECT_EQUAL(rgba.blue, 0);
  EXPECT_EQUAL(rgba.alpha, 0);

  gui::byte quad[4];
  memcpy(quad, &rgba, sizeof(rgba));
  EXPECT_EQUAL(quad[0], 0);
  EXPECT_EQUAL(quad[1], 255);
  EXPECT_EQUAL(quad[2], 0);
  EXPECT_EQUAL(quad[3], 0);
}

// --------------------------------------------------------------------------
void test_byte_order_in_bgra () {
  pixel::bgra rgba = pixel::bgra::build(color::red);
  EXPECT_EQUAL(rgba.red, 255);
  EXPECT_EQUAL(rgba.green, 0);
  EXPECT_EQUAL(rgba.blue, 0);
  EXPECT_EQUAL(rgba.alpha, 0);

  gui::byte quad[4];
  memcpy(quad, &rgba, sizeof(rgba));
  EXPECT_EQUAL(quad[0], 0);
  EXPECT_EQUAL(quad[1], 0);
  EXPECT_EQUAL(quad[2], 255);
  EXPECT_EQUAL(quad[3], 0);
}

// --------------------------------------------------------------------------
void test_byte_order_in_abgr () {
  pixel::abgr rgba = pixel::abgr::build(color::red);
  EXPECT_EQUAL(rgba.red, 255);
  EXPECT_EQUAL(rgba.green, 0);
  EXPECT_EQUAL(rgba.blue, 0);
  EXPECT_EQUAL(rgba.alpha, 0);

  gui::byte quad[4];
  memcpy(quad, &rgba, sizeof(rgba));
  EXPECT_EQUAL(quad[0], 0);
  EXPECT_EQUAL(quad[1], 0);
  EXPECT_EQUAL(quad[2], 0);
  EXPECT_EQUAL(quad[3], 255);
}

// --------------------------------------------------------------------------
void test_bitmap_black () {
  core::global::set_scale_factor(1.0);

  bitmap img(4, 4);
  graphics(img).fill(rectangle(core::point::zero, core::size(4, 4)), color::black);
  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(4, 4));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);
  EXPECT_EQUAL(img.get_info(), bitmap_info(4, 4, IF_WIN32_ELSE(2, 4), pixel_format_t::BW));

#ifdef GUIPP_QTxx
  auto pic = img.get_os_bitmap()->toImage();
  auto data = data2hex(pic.constBits(), pic.byteCount());
  EXPECT_EQUAL(data, "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
#endif

#ifdef GUIPP_QT
  QImage qimg = img.get_os_bitmap()->toImage();
  qimg.invertPixels();
  EXPECT_EQUAL(qimage2colormap(qimg),
               CM({{_,_,_,_},
                   {_,_,_,_},
                   {_,_,_,_},
                   {_,_,_,_}}));
#endif

  auto buffer = datamap2colormap(img.get());
  EXPECT_EQUAL(buffer,
               CM({{_,_,_,_},
                   {_,_,_,_},
                   {_,_,_,_},
                   {_,_,_,_}}));

  graphics(img).fill(rectangle(core::point(1, 1), core::size(2, 2)), color::white);

  buffer = datamap2colormap(img.get());
  EXPECT_EQUAL(buffer, CM({{_,_,_,_},
                           {_,W,W,_},
                           {_,W,W,_},
                           {_,_,_,_}}));
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
  const pixel_format_t expected_pixelformat = pixmap::default_pixel_format();
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
  const pixel_format_t expected_pixelformat = pixmap::default_pixel_format();
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
  const pixel_format_t expected_pixelformat = pixmap::default_pixel_format();
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
  const pixel_format_t expected_pixelformat = pixmap::default_pixel_format();
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
  const pixel_format_t expected_pixelformat = pixmap::default_pixel_format();
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
  const pixel_format_t expected_pixelformat = pixmap::default_pixel_format();
  pixmap img(2, 2);
  {
    graphics g(img);
    g.clear(color::white);
    auto p1 = color::remove_transparency(g.get_pixel({0, 0}));
    EXPECT_EQUAL(p1, color::white);
    g.clear(color::black);
    auto p2 = color::remove_transparency(g.get_pixel({0, 0}));
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
#ifdef GUIPP_X11
# define EMPTY "000000ff"
# define BLUE  "ff0000ff"
#elif GUIPP_WIN
# define EMPTY "00000000"
# define BLUE  "ff000000"
#elif GUIPP_QT
# define EMPTY "000000ff"
# define BLUE  "ff0000ff"
#endif

  const pixel_format_t expected_pixelformat = pixmap::default_pixel_format();

  core::global::set_scale_factor(1.0);

  pixmap img(6, 6);
  {
    graphics g(img);
    g.clear(color::black);
    g.draw(draw::rectangle(core::point(2, 2), core::size(2, 2)), color::blue, color::blue);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({0, 0})), color::black);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({0, 1})), color::black);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({1, 0})), color::black);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({1, 1})), color::black);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({2, 2})), color::blue);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({3, 3})), color::blue);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({4, 4})), color::black);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({5, 5})), color::black);
  }

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(6, 6));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), expected_pixelformat);
  EXPECT_EQUAL(img.get_info(), bitmap_info(6, 6, expected_pixelformat));

#ifdef GUIPP_X11
  XImage* xim = XGetImage(core::global::get_instance(), img.get_os_bitmap(), 0, 0, 6, 6, AllPlanes, ZPixmap);
  const int height = xim->height;
  const int bytes_per_line = xim->bytes_per_line;
  const auto data = xim->data;
#elif GUIPP_WIN
  BITMAP bmi;
  GetObject(img.get_os_bitmap(), sizeof (BITMAP), &bmi);
  blob data;
  data.resize(bmi.bmHeight * bmi.bmWidthBytes);
  GetBitmapBits(img.get_os_bitmap(), (LONG)data.size(), data.data());
  const int height = bmi.bmHeight;
  const int bytes_per_line = bmi.bmWidthBytes;
#elif GUIPP_QT
  auto pic = img.get_os_bitmap()->toImage();
  auto data = pic.constBits();
  const int height = pic.height();
  const int bytes_per_line = pic.bytesPerLine();
#endif

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
#ifdef GUIPP_X11
  XDestroyImage(xim);
#endif // GUIPP_X11

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
  core::global::set_scale_factor(1.0);

  pixmap pix(2, 2);
  {
    graphics g(pix);
    g.draw_pixel({0, 0}, color::black);
    g.draw_pixel({0, 1}, color::white);
    g.draw_pixel({1, 0}, color::white);
    g.draw_pixel({1, 1}, color::black);
    g.flush();

    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({0, 0})), color::black);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({0, 1})), color::white);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({1, 0})), color::white);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({1, 1})), color::black);
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
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({0, 0})), color::black);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({0, 1})), color::white);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({1, 0})), color::white);
    EXPECT_EQUAL(color::remove_transparency(g.get_pixel({1, 1})), color::black);
  }

#ifdef GUIPP_X11
  XImage* xim = XGetImage(core::global::get_instance(), img.get_os_bitmap(), 0, 0, 2, 2, AllPlanes, ZPixmap);
  auto data = xim->data;
#elif GUIPP_WIN
  BITMAP bmi;
  GetObject(img.get_os_bitmap(), sizeof (BITMAP), &bmi);
  blob data;
  data.resize(bmi.bmHeight * bmi.bmWidthBytes);
  int result = GetBitmapBits(img.get_os_bitmap(), (LONG)data.size(), data.data());
  if (result != data.size()) {
    std::cerr << "GetBitmapBits returned " << result << " expected:" << data.size() << std::endl;
  }
#elif GUIPP_QT
  auto pic = img.get_os_bitmap()->toImage();
  auto data = pic.constBits();
#endif

  auto buffer = data2hex(data, IF_WIN32_ELSE(4, 8));

#ifdef GUIPP_X11
  XDestroyImage(xim);
#endif // GUIPP_X11

  EXPECT_EQUAL(buffer, IF_WIN32_ELSE("7f ff bf ff", "02 00 00 00 01 00 00 00"));

  bwmap bw2 = img.get();

  const_image_data<pixel_format_t::BW> bw2_raw = static_cast<const bwmap&>(bw2).get_data();
  EXPECT_EQUAL(bw2_raw.pixel(0, 0), pixel::mono::black);
  EXPECT_EQUAL(bw2_raw.pixel(0, 1), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 0), pixel::mono::white);
  EXPECT_EQUAL(bw2_raw.pixel(1, 1), pixel::mono::black);

}


// --------------------------------------------------------------------------
void test_bitmap2pixmap () {
  core::global::set_scale_factor(1.0);

  using raw_type = image_data<pixel_format_t::BW>;

  bwmap bw(2,2);
  raw_type bw_raw = bw.get_data();
  bw_raw.pixel(0, 0) = pixel::mono::black;
  bw_raw.pixel(0, 1) = pixel::mono::white;
  bw_raw.pixel(1, 0) = pixel::mono::white;
  bw_raw.pixel(1, 1) = pixel::mono::black;

  EXPECT_EQUAL(bw_raw.pixel(0, 0), pixel::mono::black);
  EXPECT_EQUAL(bw_raw.pixel(0, 1), pixel::mono::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 0), pixel::mono::white);
  EXPECT_EQUAL(bw_raw.pixel(1, 1), pixel::mono::black);

  pixmap pix = bw;

#ifdef GUIPP_QT
  EXPECT_EQUAL(qimage2colormap(pix.get_os_bitmap()->toImage()),
             CM({{_,W},
                 {W,_}}));
#endif

  rgbmap rgb = pix.get<pixel_format_t::RGB>();
//  auto cnv = datamap<pixel_format_t::RGB>().convert<pixel_format_t::GRAY>();
  auto rgb_raw = rgb.get_data();
  EXPECT_EQUAL(rgb_raw.pixel(0, 0), pixel::color<pixel::rgb>::black);
  EXPECT_EQUAL(rgb_raw.pixel(0, 1), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 0), pixel::color<pixel::rgb>::white);
  EXPECT_EQUAL(rgb_raw.pixel(1, 1), pixel::color<pixel::rgb>::black);

}


// --------------------------------------------------------------------------
void test_bitmap_scale2pixmap () {
  core::global::set_scale_factor(1.0);

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
  testing::log_info("Running pixmap_test");

#ifdef GUIPP_QT
  run_test(test_qpixmap2qimage);
  run_test(test_qbitmap2qimage);
  run_test(test_qimage2qpixmap);
#endif //GUIPP_QT

  run_test(test_byte_order_in_rgba);
  run_test(test_byte_order_in_argb);
  run_test(test_byte_order_in_bgra);
  run_test(test_byte_order_in_abgr);

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


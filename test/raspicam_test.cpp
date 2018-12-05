

#include "raspi/encoder.h"
#include "base/ostreamfmt.h"
#include "base/string_util.h"


#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

struct raw_block {
  uint8_t hr0;
  uint8_t hg1;
  uint8_t hg2;
  uint8_t hb3;
  uint8_t lr0:2;
  uint8_t lg1:2;
  uint8_t lg2:2;
  uint8_t lb3:2;

  uint16_t red () {
    return ((uint16_t)hr0 << 2) | (lr0 & 0x03);
  }

  uint16_t green1 () {
    return ((uint16_t)hg1 << 2) | (lg1 & 0x03);
  }

  uint16_t green2 () {
    return ((uint16_t)hg2 << 2) | (lg2 & 0x03);
  }

  uint16_t blue () {
    return ((uint16_t)hb3 << 2) | (lb3 & 0x03);
  }

};

MMAL_FOURCC_T parse_fourcc (const std::string& str) {
  MMAL_FOURCC_T encoding = MMAL_FOURCC(' ',' ',' ',' ');
  const char* arg = str.c_str() + str.length();
  for (int i = str.length() - 1; i >= 0; --i) {
    encoding = (encoding << 8) | (str[i] & 0xFF);
  }
  LogInfo << "Found encoding:'" << gui::raspi::camera::four_cc(encoding, false) << "'";
  return encoding;
}

gui::raspi::camera::raspi_camera::crop parse_crop (const std::string& str) {
  std::istringstream in(str);
  gui::raspi::camera::raspi_camera::crop crop;
  char comma;
  in >> crop.x >> comma >> crop.y >> comma >> crop.width >> comma >> crop.height;
  LogInfo << "Found crop:'" << crop << "'";
  return crop;
}

gui::raspi::camera::raspi_camera::size parse_size (const std::string& str) {
  std::istringstream in(str);
  gui::raspi::camera::raspi_camera::size size{0, 0};
  char comma;
  in >> size.w >> comma >> size.h;
  LogInfo << "Found size:'" << size << "'";
  return size;
}

// --------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
  LogDebug << "Create raspi_camera";

  using namespace gui::raspi::camera;
  raspi_camera camera;

  MMAL_FOURCC_T encoding = 0;
  bool raw = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (basepp::string::starts_with(arg, "-f")) {
      encoding = parse_fourcc(arg.substr(2));
    } else if (basepp::string::starts_with(arg, "--format")) {
      encoding = parse_fourcc(arg.substr(8));
    } else if (basepp::string::starts_with(arg, "-c")) {
      camera.set_crop(parse_crop(arg.substr(2)));
    } else if (basepp::string::starts_with(arg, "--crop")) {
      camera.set_crop(parse_crop(arg.substr(6)));
    } else if (basepp::string::starts_with(arg, "-s")) {
      camera.set_size(parse_size(arg.substr(2)));
    } else if (basepp::string::starts_with(arg, "--size")) {
      camera.set_size(parse_size(arg.substr(6)));
    } else if ((arg == "-r") || (arg == "--raw")) {
      raw = true;
    } else if ((arg == "-h") || (arg == "--help")) {
        std::cout << "raspicam options:\n"
                     "-f<FOURCC>|--format<FOURCC>: Use <FOURCC> encoding (BMP, PNG(default), PPM, JPEG, GIF, TGA, bRA8)\n"
                     "-c<X,Y,W,H>|--crop<X,Y,W,H>: Crop image by <X,Y,W,H>\n"
                     "-s<W,H>|--size<W,H>        : Use image size <W,H>\n"
                     "-r|--raw                   : Use raw capture (-f:BD10, bRA8, bGA8, BGGR, RGAA, I420, S420, I422, S422, RGBA, rgba\n";
        return 0;
    }
  }

//  camera.set_crop({.x = 0.4F, .y = 0.4F, .width = 0.2F, .height = 0.2F});
  LogInfo << "Camera info: " << camera;

  if (raw) {
    if (!encoding) {
      encoding = MMAL_ENCODING_BAYER_SRGGB10DPCM8;
    }
    raspi_raw_encoder encoder(camera, encoding);
    LogInfo << "capture " << four_cc(encoding);
    encoder.capture(5000);

    raspi_encoder::image_data data = encoder.get_data();

    LogInfo << "get_data size:" << data.length();

    {
      std::ofstream file(ostreamfmt("raspicam_test.raw." << four_cc(encoding)));
      file.write((const char*)data.c_str(), data.length());
    }

//    const char zeros[] = {0, 0, 0};

//    std::ofstream file("raspicam_test.ppm");
//    auto info = camera.get_abs_crop();
//    file << "P5\n" << info.width << " " << info.height << "\n255\n";
//    const uint8_t* bytes = data.c_str();

//    int row_length = 4120;//info.width * 5 / 4;
//    for (int y = 0; y < info.height; ++y) {
//      const uint8_t* row = bytes + y * row_length;
//      const raw_block* blocks = (const raw_block*)row;
//      for (int x = 0; x < info.width; x += 4) {
//        file.write((const char*)blocks, 4);

//        uint8_t rgb[] = {
//          blocks->hb3,
//          (blocks->hg1 + blocks->hg2) / 2,
//          blocks->hr0
//        };
//        file.write((const char*)rgb, 3);
//        file.write((const char*)rgb, 3);
//        file.write((const char*)rgb, 3);
//        file.write((const char*)rgb, 3);

////        file.write((const char*)&blocks->hb3, 1);
////        file.write(zeros, 3); // 2 + 1
////        file.write((const char*)&blocks->hg1, 1);
////        file.write(zeros, 2); // 1 + 1
////        file.write((const char*)&blocks->hg2, 1);
////        file.write(zeros, 3); // 1 + 2
////        file.write((const char*)&blocks->hr0, 1);
//        ++blocks;
//      }
//    }
//    file.close();
  } else {
    if (!encoding) {
      encoding = MMAL_ENCODING_PNG;
    }
    raspi_image_encoder encoder(camera, encoding);
    LogInfo << "capture " << four_cc(encoding);
    encoder.capture(10000);

    auto data = encoder.get_data();

    LogInfo << "get_data size:" << data.length();

    std::ofstream file(ostreamfmt("raspicam_test.img." << four_cc(encoding)));
    file.write((const char*)data.c_str(), data.length());
    file.close();
  }

  return 0;
}

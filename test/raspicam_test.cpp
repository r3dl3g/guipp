

#include <raspi/encoder.h>

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

// --------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
  LogDebug << "Create raspi_camera";
  gui::raspi::camera::raspi_camera camera;

//  camera.set_crop({.x = 0.4F, .y = 0.4F, .width = 0.2F, .height = 0.2F});
  LogDebug << "Camera info: " << camera;

  if (true) {
    gui::raspi::camera::raspi_raw_encoder encoder(camera);
    LogDebug << "capture raw";
    encoder.capture(5000);

    gui::raspi::camera::raspi_encoder::image_data data = encoder.get_data();

    LogDebug << "get_data size:" << data.length();

//    {
//      std::ofstream file("raspicam_test.raw");
//      file.write((const char*)data.c_str(), data.length());
//    }

    std::ofstream file("raspicam_test.ppm");
    auto info = camera.get_abs_crop();
    file << "P5\n" << info.width << " " << info.height << "\n255\n";
    const uint8_t* bytes = data.c_str();

//    const char zeros[] = {0, 0, 0};

    int row_length = 4120;//info.width * 5 / 4;
    for (int y = 0; y < info.height; ++y) {
      const uint8_t* row = bytes + y * row_length;
      const raw_block* blocks = (const raw_block*)row;
      for (int x = 0; x < info.width; x += 4) {
        file.write((const char*)blocks, 4);

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
        ++blocks;
      }
    }
    file.close();
  } else {
    gui::raspi::camera::raspi_image_encoder encoder(camera, MMAL_ENCODING_PPM);
    LogDebug << "capture ppm";
    encoder.capture(10000);

    auto data = encoder.get_data();

    LogDebug << "get_data size:" << data.length();

    std::ofstream file("raspicam_test.img.ppm");
    file.write((const char*)data.c_str(), data.length());
    file.close();
  }

  return 0;
}

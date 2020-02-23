
#include "raspi/encoder.h"
#include "util/ostreamfmt.h"
#include "util/command_line.h"
#include "logging/logger.h"


#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

template<typename T>
T parse_arg (const std::string& str, const std::string& name) {
  T t = T();
  std::istringstream(str) >> t;
  LogTrace << "Found " << name << ":'" << t << "'";
  return t;
}

// --------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
  LogTrace << "Create raspi_camera";

  using namespace gui::raspi::camera;
  raspi_camera camera;

  gui::raspi::core::four_cc encoding = 0;
  bool raw = false;
  std::string outname = "raspicam_test";

  util::command_line::parser("raspicam_test V 0.1.0",
  {
    {"-e", "--encoding", "<FOURCC>", "Use <FOURCC> encoding (BMP, PNG, PPM, JPEG(default), GIF, TGA)",
      [&](const std::string& arg) {
        encoding = parse_arg<gui::raspi::core::four_cc>(arg, "encoding");
      }},
    {"-r", "--raw", {}, "Use raw capture (-f: BD10, bRA8, bGA8, BGGR, RGAA, I420, S420, I422, S422, RGBA, rgba, RGB3, rgb3)",
      [&](const std::string&) {
        raw = true;
        LogInfo << "Enable raw";
      }},
    {"-ss", "--shutter", "<us>", "Set shutter speed in us",
      [&](const std::string& arg) {
        camera.set_shutter_speed(parse_arg<int>(arg, "Shutter speed"));
      }},
    {"-a", "--awb", "<R,B>", "Set AWB gains red (R) and blue (B)",
      [&](const std::string& arg) {
        camera.set_awb_mode(MMAL_PARAM_AWBMODE_OFF);
        camera.set_awb_gains(parse_arg<raspi_camera::awb_gains>(arg, "AWB gains"));
      }},
    {"-c", "--crop", "<X,Y,W,H>", "Crop image by <X,Y,W,H> in normalised coordinates [0.0-1.0]",
      [&](const std::string& arg) {
        camera.set_input_crop(parse_arg<gui::raspi::core::crop>(arg, "Crop"));
      }},
    {"-sz", "--size", "<W,H>", "Use image size <W,H>",
      [&](const std::string& arg) {
        camera.set_size(parse_arg<raspi_camera::size>(arg, "Size"));
      }},
//    {"-rz", "--resize", "<W,H>", "Use image resize <W,H>",
//      [&](const std::string& arg) {
//        auto sz = parse_arg<raspi_camera::size>(arg, "Resize");
//        camera.set_resize({{}, MMAL_RESIZE_CROP, sz.width, sz.height, 0, false, false});
//      }},
    {"-i", "--iso", "<ISO>", "Set sensor ISO",
      [&](const std::string& arg) {
        camera.set_iso(parse_arg<uint32_t>(arg, "ISO"));
      }},
    {"-sh", "--sharpness", "[-1..1]", "Set image sharpness",
      [&](const std::string& arg) {
        camera.set_sharpness(parse_arg<float>(arg, "Sharpness"));
      }},
    {"-co", "--contrast", "[-1..1]", "Set image contrast",
      [&](const std::string& arg) {
        camera.set_contrast(parse_arg<float>(arg, "Contrast"));
      }},
    {"-br", "--brightness", "[0..1]", "Set image brightness",
      [&](const std::string& arg) {
        camera.set_brightness(parse_arg<float>(arg, "Brightness"));
      }},
    {"-sa", "--saturation", "[-1..1]", "Set image saturation",
      [&](const std::string& arg) {
        camera.set_saturation(parse_arg<float>(arg, "Saturation"));
      }},
    {"-o", "--out", "<filename>", "Set output file name",
      [&](const std::string& arg) {
        outname = util::string::trimed(arg);
        LogInfo << "Found output file name:'" << outname << "'";
      }}
  }).process(argc, argv);

  LogTrace << "Camera info: " << camera;

  if (raw) {
    if (!encoding) {
      encoding = MMAL_ENCODING_BGR24;
    }
    raspi_raw_encoder encoder(camera.get_still_output_port(), raspi_raw_encoder::OutEncoding(encoding.type.uint32));
    LogTrace << "raw capture " << encoding;
    encoder.enable();
    encoder.capture(5000);

    const raspi_encoder::image_data& data = encoder.get_data();

    LogInfo << "get_data size:" << data.size();

    std::ofstream file(ostreamfmt(outname << "." << encoding));
    auto sz = camera.get_size();
    file << "P6\n"
         << "# pnm created by raspicam_test\n"
         << sz.width << " " << sz.height << " 255\n";

    auto ppl = data.size() / sz.height;
    for (int y = 0; y < sz.height; ++y) {
      auto ptr = data.data() + y * ppl;
      file.write((const char*)ptr, sz.width * 3);
    }

  } else {
    if (!encoding) {
      encoding = MMAL_ENCODING_JPEG;
    }
    raspi_image_encoder encoder(camera.get_still_output_port(), raspi_image_encoder::OutEncoding(encoding.type.uint32));
    LogTrace << "encoded capture " << encoding;

    encoder.enable();

    encoder.capture(5000);

    auto data = encoder.get_data();

    LogInfo << "get_data size:" << data.size();

    std::ofstream file(ostreamfmt(outname << "." << encoding));
    file.write((const char*)data.data(), data.size());
  }

  logging::core::instance().finish();

  return 0;
}

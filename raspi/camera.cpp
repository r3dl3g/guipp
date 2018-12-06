/**
 * @copyright (c) 2018-2018 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui raspi lib
 *
 * Customer   -
 *
 * @brief     C++ API: Raspi camera interface.
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <sstream>
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/string_util.h>
#include <base/ostreamfmt.h>
#include <base/logger.h>

// --------------------------------------------------------------------------
//
// Project includes
//
#include "camera.h"

#include <interface/mmal/util/mmal_util.h>
#include <interface/mmal/util/mmal_util_params.h>
#include <interface/mmal/util/mmal_default_components.h>

// --------------------------------------------------------------------------
//
// Source:
//

namespace gui {

  namespace raspi {

    namespace camera {
#     define MMAL_CAMERA_PREVIEW_PORT 0
#     define MMAL_CAMERA_VIDEO_PORT   1
#     define MMAL_CAMERA_CAPTURE_PORT 2

#     define check_mmal_status(A) check_mmal_status_(A, #A)

      void check_mmal_status_ (MMAL_STATUS_T status, const char* info) {
        if (status != MMAL_SUCCESS) {
          switch (status) {
            case MMAL_ENOMEM :   throw std::runtime_error(ostreamfmt("Out of memory when calling " << info)); break;
            case MMAL_ENOSPC :   throw std::runtime_error(ostreamfmt("Out of resources (other than memory) when calling " << info)); break;
            case MMAL_EINVAL:    throw std::invalid_argument(ostreamfmt("Argument is invalid when calling " << info)); break;
            case MMAL_ENOSYS :   throw std::runtime_error(ostreamfmt("Function not implemented when calling " << info)); break;
            case MMAL_ENOENT :   throw std::invalid_argument(ostreamfmt("No such file or directory when calling " << info)); break;
            case MMAL_ENXIO :    throw std::invalid_argument(ostreamfmt("No such device or address when calling " << info)); break;
            case MMAL_EIO :      throw std::runtime_error(ostreamfmt("I/O error when calling " << info)); break;
            case MMAL_ESPIPE :   throw std::runtime_error(ostreamfmt("Illegal seek when calling " << info)); break;
            case MMAL_ECORRUPT : throw std::runtime_error(ostreamfmt("Data is corrupt \attention FIXME: not POSIX when calling " << info)); break;
            case MMAL_ENOTREADY :throw std::runtime_error(ostreamfmt("Component is not ready \attention FIXME: not POSIX when calling " << info)); break;
            case MMAL_ECONFIG :  throw std::runtime_error(ostreamfmt("Component is not configured \attention FIXME: not POSIX when calling " << info)); break;
            case MMAL_EISCONN :  throw std::runtime_error(ostreamfmt("Port is already connected when calling " << info)); break;
            case MMAL_ENOTCONN : throw std::runtime_error(ostreamfmt("Port is disconnected when calling " << info)); break;
            case MMAL_EAGAIN :   throw std::runtime_error(ostreamfmt("Resource temporarily unavailable. Try again later when calling " << info)); break;
            case MMAL_EFAULT :   throw std::runtime_error(ostreamfmt("Bad address when calling " << info)); break;
            default :            throw std::runtime_error(ostreamfmt("Unknown status error when calling " << info)); break;
          }
        }
      }

      // --------------------------------------------------------------------------
      std::ostream& operator<< (std::ostream& out, const raspi_camera& cam) {
        cam.show_current(out);
        return out;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::awb_gains& gains) {
        out << "{red:" << gains.r_gain << ", blue:" << gains.b_gain << "}";
        return out;
      }

      std::istream& operator>> (std::istream& in, raspi_camera::awb_gains& gains) {
        char delemiter;
        in >> gains.r_gain >> delemiter >> gains.b_gain;
        return in;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::crop& v) {
        out << "{x:" << v.x << ", y:" << v.y << ", w:" << v.width << ", h:" << v.height << "}";
        return out;
      }

      std::istream& operator>> (std::istream& in, raspi_camera::crop& v) {
        char delemiter;
        in >> v.x >> delemiter >> v.y >> delemiter >> v.width >> delemiter >> v.height;
        return in;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::color_fx& v) {
        out << "{enable:" << v.enable << ", u:" << v.u << ", v:" << v.v << "}";
        return out;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::stereo_mode& v) {
        out << "{mode:" << v.mode << ", decimate:" << v.decimate << ", swap_eyes:" << v.swap_eyes << "}";
        return out;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::size& v) {
        out << v.w << ", " << v.h;
        return out;
      }

      std::istream& operator>> (std::istream& in, raspi_camera::size& sz) {
        char delemiter;
        in >> sz.w >> delemiter >> sz.h;
        return in;
      }

      // --------------------------------------------------------------------------
      static void camera_control_callback (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
        if (buffer->cmd == MMAL_EVENT_PARAMETER_CHANGED) {
          MMAL_EVENT_PARAMETER_CHANGED_T *param = (MMAL_EVENT_PARAMETER_CHANGED_T *)buffer->data;
          switch (param->hdr.id) {
            case MMAL_PARAMETER_CAMERA_SETTINGS: {
              MMAL_PARAMETER_CAMERA_SETTINGS_T *settings = (MMAL_PARAMETER_CAMERA_SETTINGS_T*)param;
              LogDebug << "camera_control_callback for port:'" << port->name
                       << "' encoding:'" << four_cc(port->format->encoding)
                       << "' crop:" << port->format->es->video.crop
                       << " size:" << port->format->es->video.width << " x " << port->format->es->video.width
                       << " color_space:'" << four_cc(port->format->es->video.color_space) << "'";
              // variant:'" << four_cc(port->format->encoding_variant) << "'";
              LogInfo << "Exposure now " << settings->exposure
                      << ", analog gain " << settings->analog_gain.num << "/" << settings->analog_gain.den
                      << ", digital gain " << settings->digital_gain.num << "/" << settings->digital_gain.den;
              LogInfo << "AWB R=" << settings->awb_red_gain.num << "/" << settings->awb_red_gain.den << ", B=" << settings->awb_blue_gain.num << "/" << settings->awb_blue_gain.den;
              break;
            }
            default:
              LogTrace << "camera_control_callback with id:" << param->hdr.id;
              break;
          }
        } else if (buffer->cmd == MMAL_EVENT_ERROR) {
          LogError << "No data received from sensor. Check all connections, including the Sunny one on the camera board";
        } else {
          LogError << "Received unexpected camera control callback event, 0x%08x", buffer->cmd;
        }

        mmal_buffer_header_release(buffer);
      }

      // --------------------------------------------------------------------------
      raspi_camera::raspi_camera (int num)
        : m_camera(nullptr)
        , m_camera_still_port(nullptr)
      {
        check_mmal_status(mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &m_camera));
        if (!m_camera->output_num) {
            throw std::runtime_error("Camera doesn't have output ports");
        }

        set_stereo_mode(stereo_mode{.mode = MMAL_STEREOSCOPIC_MODE_NONE, .decimate = false, .swap_eyes = false});
        set_camera_num(num);
        check_mmal_status(mmal_port_parameter_set_uint32(m_camera->control, MMAL_PARAMETER_CAMERA_CUSTOM_SENSOR_CONFIG, 0));

        MMAL_PARAMETER_CHANGE_EVENT_REQUEST_T change_event_request =
           {{MMAL_PARAMETER_CHANGE_EVENT_REQUEST, sizeof(MMAL_PARAMETER_CHANGE_EVENT_REQUEST_T)},
            MMAL_PARAMETER_CAMERA_SETTINGS, 1};

        check_mmal_status(mmal_port_parameter_set(m_camera->control, &change_event_request.hdr));
        check_mmal_status(mmal_port_enable(m_camera->control, camera_control_callback));

        MMAL_PARAMETER_CAMERA_INFO_CAMERA_T info = get_camera_info(num);

        MMAL_PARAMETER_CAMERA_CONFIG_T camConfig = {
          { MMAL_PARAMETER_CAMERA_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_CONFIG_T) },
          .max_stills_w = info.max_width,
          .max_stills_h = info.max_height,
          .stills_yuv422 = 0,
          .one_shot_stills = 1,
          .max_preview_video_w = 480,
          .max_preview_video_h = 320,
          .num_preview_video_frames = 1,
          .stills_capture_circular_buffer_height = 0,
          .fast_preview_resume = 0,
          .use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC
        };

        set_camera_config(camConfig);

        set_defaults(10000);

        m_camera_still_port = m_camera->output[MMAL_CAMERA_CAPTURE_PORT];

        MMAL_ES_FORMAT_T *format = m_camera_still_port->format;

        format->encoding = MMAL_ENCODING_OPAQUE;
        format->es->video.width = VCOS_ALIGN_UP(camConfig.max_stills_w, 32);
        format->es->video.height = VCOS_ALIGN_UP(camConfig.max_stills_h, 16);
        format->es->video.crop.x = 0;
        format->es->video.crop.y = 0;
        format->es->video.crop.width = camConfig.max_stills_w;
        format->es->video.crop.height = camConfig.max_stills_h;
        format->es->video.frame_rate.num = 0;
        format->es->video.frame_rate.den = 1;

        check_mmal_status(mmal_port_format_commit(m_camera_still_port));
        check_mmal_status(mmal_component_enable(m_camera));

        check_mmal_status(mmal_port_parameter_set_boolean(m_camera_still_port, MMAL_PARAMETER_ENABLE_RAW_CAPTURE, 1));

      }

      raspi_camera::~raspi_camera () {
        if (m_camera) {
          mmal_component_destroy(m_camera);
          m_camera = nullptr;
        }
      }

      // --------------------------------------------------------------------------
      MMAL_PORT_T* raspi_camera::get_camera_still_port () const {
        return m_camera_still_port;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::capture () {
        check_mmal_status(mmal_port_parameter_set_boolean(m_camera_still_port, MMAL_PARAMETER_CAPTURE, 1));
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_defaults (int speed, int iso) {
        set_saturation(0);
        set_sharpness(0);
        set_contrast(0);
        set_brightness(0.5F);
        set_iso(iso);
        set_video_stabilisation(0);
        set_exposure_compensation(0);
        set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_OFF);
        set_flicker_avoid_mode(MMAL_PARAM_FLICKERAVOID_OFF);
        set_metering_mode(MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE);
        set_awb_mode(MMAL_PARAM_AWBMODE_OFF);
        set_awb_gains({1.0F, 1.0F});
        set_image_fx(MMAL_PARAM_IMAGEFX_NONE);
        set_color_fx({false, 128, 128});
        set_rotation(0);
        set_flip(MMAL_PARAM_MIRROR_NONE);
//        set_crop({0.0F, 0.0F, 1.0F, 1.0F});
        set_shutter_speed(speed);
        set_drc(MMAL_PARAMETER_DRC_STRENGTH_OFF);
        set_stats_pass(MMAL_FALSE);
        set_analog_gain(1.0F);
        set_digital_gain(1.0F);
      }

      void raspi_camera::show_current (std::ostream& out) const {
        out << "saturation:" << get_saturation();
        out << ", sharpness:" << get_sharpness();
        out << ", contrast:" << get_contrast();
        out << ", brightness:" << get_brightness();
        out << ", iso:" << get_iso();
        out << ", shutter_speed:" << get_shutter_speed();
        out << ", analog_gain:" << get_analog_gain();
        out << ", digital_gain:" << get_digital_gain();
        out << ", size[w,h]:{" << get_size() << "}";

        out << ", video_stabilisation:" << get_video_stabilisation();
        out << ", exposure_compensation:" << get_exposure_compensation();
        out << ", exposure_mode:" << get_exposure_mode();
        out << ", flicker_avoid_mode:" << get_flicker_avoid_mode();
        out << ", metering_mode:" << get_metering_mode();
        out << ", awb_mode:" << get_awb_mode();
        out << ", awb_gains:" << get_awb_gains();

        out << ", stereo_mode:" << get_stereo_mode();

        out << ", image_fx:" << get_image_fx();
        out << ", color_fx:" << get_color_fx();
        out << ", flip:" << get_flip();
        out << ", crop:" << get_crop();
        out << ", abs_crop:" << get_abs_crop();
        out << ", rotation:" << get_rotation();
        out << ", drc:" << get_drc();
        out << ", stats_pass:" << get_stats_pass();

        out << ", camera_config:" << get_camera_config();
        out << ", camera_info:" << get_camera_info();
//        out << ", resize:" << get_resize();
//        out << ", camera_rx_config:" << get_camera_rx_config();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_rational (uint32_t id, float v) {
         MMAL_RATIONAL_T value = {static_cast<int32_t>(v * 10000.0F), 10000};
         check_mmal_status(mmal_port_parameter_set_rational(m_camera->control, id, value));
      }

      void raspi_camera::set_rational (uint32_t id, float v, float min, float max) {
        if ((min <= v) && (v <= max)) {
           set_rational(id, v);
        } else {
           throw std::invalid_argument(ostreamfmt("Invalid rational value (" << min << " <= " << v << " <= " << max));
        }
      }

      float raspi_camera::get_rational (uint32_t id) const {
        MMAL_RATIONAL_T value = {0, 100};
        check_mmal_status(mmal_port_parameter_get_rational(m_camera->control, id, &value));
        return (float)value.num / (float)value.den;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_saturation (float saturation) {
        set_rational(MMAL_PARAMETER_SATURATION, saturation, -1.0F, 1.0F);
      }

      float raspi_camera::get_saturation () const {
        return get_rational(MMAL_PARAMETER_SATURATION);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_sharpness (float sharpness) {
        set_rational(MMAL_PARAMETER_SHARPNESS, sharpness, -1.0F, 1.0F);
      }

      float raspi_camera::get_sharpness () const {
        return get_rational(MMAL_PARAMETER_SHARPNESS);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_contrast (float contrast) {
        set_rational(MMAL_PARAMETER_CONTRAST, contrast, -1.0F, 1.0F);
      }

      float raspi_camera::get_contrast () const {
        return get_rational(MMAL_PARAMETER_CONTRAST);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_brightness (float brightness) {
        set_rational(MMAL_PARAMETER_BRIGHTNESS, brightness, 0, 1.0F);
      }

      float raspi_camera::get_brightness () const {
        return get_rational(MMAL_PARAMETER_BRIGHTNESS);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_iso (uint32_t iso) {
        check_mmal_status(mmal_port_parameter_set_uint32(m_camera->control, MMAL_PARAMETER_ISO, iso));
      }

      uint32_t raspi_camera::get_iso () const {
        uint32_t iso;
        check_mmal_status(mmal_port_parameter_get_uint32(m_camera->control, MMAL_PARAMETER_ISO, &iso));
        return iso;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_shutter_speed (int speed) {
        check_mmal_status(mmal_port_parameter_set_uint32(m_camera->control, MMAL_PARAMETER_SHUTTER_SPEED, speed));
      }

      int raspi_camera::get_shutter_speed () const {
        uint32_t speed;
        check_mmal_status(mmal_port_parameter_get_uint32(m_camera->control, MMAL_PARAMETER_SHUTTER_SPEED, &speed));
        return speed;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_analog_gain (float gain) {
        set_rational(MMAL_PARAMETER_ANALOG_GAIN, gain);
      }

      float raspi_camera::get_analog_gain () const {
        return get_rational(MMAL_PARAMETER_ANALOG_GAIN);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_digital_gain (float gain) {
        set_rational(MMAL_PARAMETER_DIGITAL_GAIN, gain);
      }

      float raspi_camera::get_digital_gain () const {
        return get_rational(MMAL_PARAMETER_DIGITAL_GAIN);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_video_stabilisation (bool vstabilisation) {
        MMAL_BOOL_T b = vstabilisation ? MMAL_TRUE : MMAL_FALSE;
        check_mmal_status(mmal_port_parameter_set_boolean(m_camera->control, MMAL_PARAMETER_VIDEO_STABILISATION, b));
      }

      bool raspi_camera::get_video_stabilisation () const {
        MMAL_BOOL_T b = MMAL_FALSE;
        check_mmal_status(mmal_port_parameter_get_boolean(m_camera->control, MMAL_PARAMETER_VIDEO_STABILISATION, &b));
        return b;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_stats_pass (bool stats_pass) {
        MMAL_BOOL_T b = stats_pass ? MMAL_TRUE : MMAL_FALSE;
        check_mmal_status(mmal_port_parameter_set_boolean(m_camera->control, MMAL_PARAMETER_CAPTURE_STATS_PASS, b));
      }

      bool raspi_camera::get_stats_pass () const {
        MMAL_BOOL_T b = MMAL_FALSE;
        check_mmal_status(mmal_port_parameter_get_boolean(m_camera->control, MMAL_PARAMETER_CAPTURE_STATS_PASS, &b));
        return b;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_exposure_compensation (int exp_comp) {
        check_mmal_status(mmal_port_parameter_set_int32(m_camera->control, MMAL_PARAMETER_EXPOSURE_COMP, exp_comp));
      }

      int raspi_camera::get_exposure_compensation () const {
        int32_t value;
        check_mmal_status(mmal_port_parameter_get_int32(m_camera->control, MMAL_PARAMETER_EXPOSURE_COMP, &value));
        return value;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_awb_gains (const awb_gains& gains) {
        MMAL_PARAMETER_AWB_GAINS_T param = {{MMAL_PARAMETER_CUSTOM_AWB_GAINS,sizeof(param)}, {0,0}, {0,0}};

        param.r_gain.den = param.b_gain.den = 65536;
        param.r_gain.num = (unsigned int)(gains.r_gain * 65536);
        param.b_gain.num = (unsigned int)(gains.b_gain * 65536);
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &param.hdr));
      }

      auto raspi_camera::get_awb_gains () const -> awb_gains {
        MMAL_PARAMETER_AWB_GAINS_T param = {{MMAL_PARAMETER_CUSTOM_AWB_GAINS,sizeof(param)}, {0,0}, {0,0}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &param.hdr));

        return awb_gains{
          .r_gain = static_cast<float>(param.r_gain.num) / static_cast<float>(param.r_gain.den),
          .b_gain = static_cast<float>(param.b_gain.num) / static_cast<float>(param.b_gain.den)
        };
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_rotation (int rotation) {
        int my_rotation = ((rotation % 360 ) / 90) * 90;

        check_mmal_status(mmal_port_parameter_set_int32(m_camera->output[0], MMAL_PARAMETER_ROTATION, my_rotation));
        check_mmal_status(mmal_port_parameter_set_int32(m_camera->output[1], MMAL_PARAMETER_ROTATION, my_rotation));
        check_mmal_status(mmal_port_parameter_set_int32(m_camera->output[2], MMAL_PARAMETER_ROTATION, my_rotation));
      }

      int raspi_camera::get_rotation () const {
        int32_t my_rotation = 0;
        check_mmal_status(mmal_port_parameter_get_int32(m_camera->output[0], MMAL_PARAMETER_ROTATION, &my_rotation));
        return my_rotation;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_flip (MMAL_PARAM_MIRROR_T mode) {
        MMAL_PARAMETER_MIRROR_T mirror = {{MMAL_PARAMETER_MIRROR, sizeof(MMAL_PARAMETER_MIRROR_T)}, mode};

        check_mmal_status(mmal_port_parameter_set(m_camera->output[0], &mirror.hdr));
        check_mmal_status(mmal_port_parameter_set(m_camera->output[1], &mirror.hdr));
        check_mmal_status(mmal_port_parameter_set(m_camera->output[2], &mirror.hdr));
      }

      MMAL_PARAM_MIRROR_T raspi_camera::get_flip () const {
        MMAL_PARAMETER_MIRROR_T mirror = {{MMAL_PARAMETER_MIRROR, sizeof(MMAL_PARAMETER_MIRROR_T)}, MMAL_PARAM_MIRROR_NONE};
        check_mmal_status(mmal_port_parameter_get(m_camera->output[0], &mirror.hdr));
        return mirror.value;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_crop (const crop& c) {
        MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)},
                                            {static_cast<int32_t>(c.x * 65536.0F), static_cast<int32_t>(c.y * 65536.0F),
                                             static_cast<int32_t>(c.width * 65536.0F), static_cast<int32_t>(c.height * 65536.0F)}};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &crop.hdr));

        int32_t w = static_cast<int32_t>(c.width * m_size.w);
        int32_t h = static_cast<int32_t>(c.height * m_size.h);

        MMAL_VIDEO_FORMAT_T& video = m_camera_still_port->format->es->video;
//        video.crop.x = 0;//static_cast<int32_t>(c.x * m_size.w);
//        video.crop.y = 0;//static_cast<int32_t>(c.y * m_size.h);
        video.crop.width = w;
        video.crop.height = h;
//        video.width = w;
//        video.height = h;
        check_mmal_status(mmal_port_format_commit(m_camera_still_port));
      }

      auto raspi_camera::get_crop () const -> crop {
        MMAL_PARAMETER_INPUT_CROP_T cr = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &cr.hdr));
        auto& c = cr.rect;
        return {c.x / 65536.0F, c.y / 65536.0F, c.width / 65536.0F, c.height / 65536.0F};
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_abs_crop (const MMAL_RECT_T& c) {
        float fx = 65536.0F / (float)m_size.w;
        float fy = 65536.0F / (float)m_size.h;
        MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)},
                                            {static_cast<int32_t>(c.x * fx), static_cast<int32_t>(c.y * fy),
                                             static_cast<int32_t>(c.width * fx), static_cast<int32_t>(c.height * fy)}};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &crop.hdr));

        MMAL_VIDEO_FORMAT_T& video = m_camera_still_port->format->es->video;
        video.crop.x = c.x;
        video.crop.y = c.y;
        video.crop.width = c.width;
        video.crop.height = c.height;
        check_mmal_status(mmal_port_format_commit(m_camera_still_port));
      }

      MMAL_RECT_T raspi_camera::get_abs_crop () const {
        float fx = (float)m_size.w / 65536.0F;
        float fy = (float)m_size.h / 65536.0F;

        MMAL_PARAMETER_INPUT_CROP_T cr = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &cr.hdr));
        auto& c = cr.rect;
        return {static_cast<int32_t>(c.x * fx), static_cast<int32_t>(c.y * fy),
                static_cast<int32_t>(c.width * fx), static_cast<int32_t>(c.height * fy)};
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_color_fx (const color_fx& c) {
        MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT, sizeof(MMAL_PARAMETER_COLOURFX_T)}, c.enable, c.u, c.v};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &colfx.hdr));
      }

      auto raspi_camera::get_color_fx () const -> color_fx {
        MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT, sizeof(MMAL_PARAMETER_COLOURFX_T)}, 0, 0, 0};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &colfx.hdr));
        return color_fx{
          .enable = colfx.enable != 0,
          .u = colfx.u,
          .v = colfx.v
        };
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_stereo_mode (const stereo_mode& sm) {
        MMAL_PARAMETER_STEREOSCOPIC_MODE_T stereo = { {MMAL_PARAMETER_STEREOSCOPIC_MODE, sizeof(MMAL_PARAMETER_STEREOSCOPIC_MODE_T)},
                                    sm.mode, sm.decimate, sm.swap_eyes};
        check_mmal_status(mmal_port_parameter_set(m_camera->output[0], &stereo.hdr));
        check_mmal_status(mmal_port_parameter_set(m_camera->output[1], &stereo.hdr));
        check_mmal_status(mmal_port_parameter_set(m_camera->output[2], &stereo.hdr));
      }

      auto raspi_camera::get_stereo_mode () const -> stereo_mode {
//        MMAL_PARAMETER_STEREOSCOPIC_MODE_T stereo = { {MMAL_PARAMETER_STEREOSCOPIC_MODE, sizeof(MMAL_PARAMETER_STEREOSCOPIC_MODE_T)},
//                                    MMAL_STEREOSCOPIC_MODE_NONE, MMAL_FALSE, MMAL_FALSE };
//        check_mmal_status(mmal_port_parameter_get(m_camera->output[2], &stereo.hdr));
        return stereo_mode{
          .mode = MMAL_STEREOSCOPIC_MODE_NONE/*stereo.mode*/,
          .decimate = MMAL_FALSE/*stereo.decimate != 0*/,
          .swap_eyes = MMAL_FALSE/*stereo.swap_eyes != 0*/
        };
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_drc (MMAL_PARAMETER_DRC_STRENGTH_T strength) {
        MMAL_PARAMETER_DRC_T drc = {{MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION, sizeof(MMAL_PARAMETER_DRC_T)}, strength};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &drc.hdr));
      }

      MMAL_PARAMETER_DRC_STRENGTH_T raspi_camera::get_drc () const {
        MMAL_PARAMETER_DRC_T drc = {{MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION, sizeof(MMAL_PARAMETER_DRC_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &drc.hdr));
        return drc.strength;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_size (const size& sz) {
        m_size = sz;
      }

      auto raspi_camera::get_size () const -> size {
        return m_size;
//        MMAL_PARAMETER_CAMERA_CONFIG_T config = get_camera_config();
//        return size {
//          .w = config.max_stills_w,
//          .h = config.max_stills_h
//        };
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_CONFIG_T raspi_camera::get_camera_config () const {
        MMAL_PARAMETER_CAMERA_CONFIG_T camConfig = {{MMAL_PARAMETER_CAMERA_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_CONFIG_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &camConfig.hdr));
        return camConfig;
      }

      void raspi_camera::set_camera_config (MMAL_PARAMETER_CAMERA_CONFIG_T camConfig) {
        camConfig.hdr = {MMAL_PARAMETER_CAMERA_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_CONFIG_T)};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &camConfig.hdr));
        m_size = {camConfig.max_stills_w, camConfig.max_stills_h};
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_INFO_CAMERA_T raspi_camera::get_camera_info (int num) const {
        if (num < 0) {
          num = get_camera_num();
        }
        MMAL_PARAMETER_CAMERA_INFO_T config = raspi_camera::get_config();
        if (num < config.num_cameras) {
          return config.cameras[num];
        }
        throw std::invalid_argument("camera number is invalid");
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_INFO_T raspi_camera::get_config () const {
        MMAL_COMPONENT_T* component = nullptr;
        try {
          check_mmal_status(mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA_INFO, &component));

          MMAL_PARAMETER_CAMERA_INFO_T config = {{MMAL_PARAMETER_CAMERA_INFO, sizeof(MMAL_PARAMETER_CAMERA_INFO_T) - 4}};
          auto status = mmal_port_parameter_get(component->control, &config.hdr);
          if (status != MMAL_SUCCESS) {
            config.hdr.size = sizeof(MMAL_PARAMETER_CAMERA_INFO_T);
            status = mmal_port_parameter_get(component->control, &config.hdr);
          }
          if (status != MMAL_SUCCESS) {
            config.num_cameras = 1;
            config.cameras[0].max_width = 2592;
            config.cameras[0].max_height = 1944;
          }
          mmal_component_destroy(component);
          return config;
        } catch (std::exception& ex) {
          mmal_component_destroy(component);
          throw ex;
        }
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_RX_CONFIG_T raspi_camera::get_camera_rx_config () const {
        MMAL_PARAMETER_CAMERA_RX_CONFIG_T cam_rx_config = {{MMAL_PARAMETER_CAMERA_RX_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_RX_CONFIG_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &cam_rx_config.hdr));
        return cam_rx_config;
      }

      void raspi_camera::set_camera_rx_config (MMAL_PARAMETER_CAMERA_RX_CONFIG_T cam_rx_config) {
        cam_rx_config.hdr = {MMAL_PARAMETER_CAMERA_RX_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_RX_CONFIG_T)};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &cam_rx_config.hdr));
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_RESIZE_T raspi_camera::get_resize () const {
        MMAL_PARAMETER_RESIZE_T resize = {{MMAL_PARAMETER_RESIZE_PARAMS, sizeof(MMAL_PARAMETER_RESIZE_T)}/*, MMAL_RESIZE_DUMMY, 0*/};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &resize.hdr));
        return resize;
      }

      void raspi_camera::set_resize (MMAL_PARAMETER_RESIZE_T resize) {
        resize.hdr = {MMAL_PARAMETER_RESIZE_PARAMS, sizeof(MMAL_PARAMETER_RESIZE_T)};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &resize.hdr));
      }

      // --------------------------------------------------------------------------
      template<typename T>
      struct mode {
        using type = void;
        static constexpr int id = 0;
      };

      // --------------------------------------------------------------------------
      template<>
      struct mode<MMAL_PARAM_EXPOSUREMETERINGMODE_T> {
        using type = MMAL_PARAMETER_EXPOSUREMETERINGMODE_T;
        static constexpr int id = MMAL_PARAMETER_EXP_METERING_MODE;
      };

      template<>
      struct mode<MMAL_PARAM_EXPOSUREMODE_T> {
        using type = MMAL_PARAMETER_EXPOSUREMODE_T;
        static constexpr int id = MMAL_PARAMETER_EXPOSURE_MODE;
      };

      template<>
      struct mode<MMAL_PARAM_FLICKERAVOID_T> {
        using type = MMAL_PARAMETER_FLICKERAVOID_T;
        static constexpr int id = MMAL_PARAMETER_FLICKER_AVOID;
      };

      template<>
      struct mode<MMAL_PARAM_AWBMODE_T> {
        using type = MMAL_PARAMETER_AWBMODE_T;
        static constexpr int id = MMAL_PARAMETER_AWB_MODE;
      };

      template<>
      struct mode<MMAL_PARAM_IMAGEFX_T> {
        using type = MMAL_PARAMETER_IMAGEFX_T;
        static constexpr int id = MMAL_PARAMETER_IMAGE_EFFECT;
      };

      template<>
      struct mode<int32_t> {
        using type = MMAL_PARAMETER_INT32_T;
        static constexpr int id = MMAL_PARAMETER_CAMERA_NUM;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      void raspi_camera::set_mode (T m) {
        typename mode<T>::type param = {{mode<T>::id, sizeof(param)}, m};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &param.hdr));
      }

      template<typename T>
      T raspi_camera::get_mode () const {
        typename mode<T>::type param = {{mode<T>::id, sizeof(param)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &param.hdr));
        return param.value;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_metering_mode (MMAL_PARAM_EXPOSUREMETERINGMODE_T mode) {
        set_mode<MMAL_PARAM_EXPOSUREMETERINGMODE_T>(mode);
      }

      MMAL_PARAM_EXPOSUREMETERINGMODE_T raspi_camera::get_metering_mode () const {
        return get_mode<MMAL_PARAM_EXPOSUREMETERINGMODE_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_exposure_mode (MMAL_PARAM_EXPOSUREMODE_T mode) {
        set_mode<MMAL_PARAM_EXPOSUREMODE_T>(mode);
      }

      MMAL_PARAM_EXPOSUREMODE_T raspi_camera::get_exposure_mode () const {
        return get_mode<MMAL_PARAM_EXPOSUREMODE_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_flicker_avoid_mode (MMAL_PARAM_FLICKERAVOID_T mode) {
        set_mode<MMAL_PARAM_FLICKERAVOID_T>(mode);
      }

      MMAL_PARAM_FLICKERAVOID_T raspi_camera::get_flicker_avoid_mode () const {
        return get_mode<MMAL_PARAM_FLICKERAVOID_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_awb_mode (MMAL_PARAM_AWBMODE_T awb_mode) {
        set_mode<MMAL_PARAM_AWBMODE_T>(awb_mode);
      }

      MMAL_PARAM_AWBMODE_T raspi_camera::get_awb_mode () const {
        return get_mode<MMAL_PARAM_AWBMODE_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_image_fx (MMAL_PARAM_IMAGEFX_T imageFX) {
        set_mode<MMAL_PARAM_IMAGEFX_T>(imageFX);
      }

      MMAL_PARAM_IMAGEFX_T raspi_camera::get_image_fx () const {
        return get_mode<MMAL_PARAM_IMAGEFX_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_camera_num (int32_t num) {
        set_mode<int32_t>(num);
      }

      int32_t raspi_camera::get_camera_num () const {
        return get_mode<int32_t>();
      }

      // --------------------------------------------------------------------------
      std::ostream& operator<< (std::ostream& out, const four_cc& fourcc) {
        if (fourcc.m_strip) {
          std::string str((const char*)&fourcc.m_type, 4);
          basepp::string::trim(str);
          out << str;
        } else {
          out.write((const char*)&fourcc.m_type, 4);
        }
        return out;
      }

      std::istream& operator>> (std::istream& in, four_cc& fourcc) {
        in >> *((char*)&fourcc.m_type);
        in >> *(((char*)&fourcc.m_type)+1);
        in >> *(((char*)&fourcc.m_type)+2);
        in >> *(((char*)&fourcc.m_type)+3);
        return in;
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // camera

  } // raspi

} // gui

namespace std {
  std::ostream& operator<< (std::ostream& out, const MMAL_RECT_T& v) {
    out << "{x:" << v.x << ", y:" << v.y << ", w:" << v.width << ", h:" << v.height << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_PARAMETER_CAMERA_CONFIG_T& v) {
    out << "{w:" << v.max_stills_w << ", h:" << v.max_stills_w
        << ", pw:" << v.max_preview_video_w  << ", ph:" << v.max_preview_video_h << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_PARAMETER_CAMERA_INFO_CAMERA_T& v) {
    out << "{name:" << v.camera_name << ", lens:" << v.lens_present
        << ", w:" << v.max_width  << ", h:" << v.max_height << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_PARAMETER_CAMERA_RX_CONFIG_T& v) {
    out << "{decode:" << v.decode << ", encode:" << v.encode
        << ", unpack:" << v.unpack << ", pack:" << v.pack
        << ", data_lanes:" << v.data_lanes << ", encode_block_length:" << v.encode_block_length
        << ", embedded_data_lines:" << v.embedded_data_lines << ", image_id:" << v.image_id
        << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_PARAMETER_RESIZE_T& v) {
    out << "{mode:" << v.mode << ", max_bytes:" << v.max_bytes
        << ", w:" << v.max_width << ", h:" << v.max_height
        << ", aspect:" << v.preserve_aspect_ratio << ", upscaling:" << v.allow_upscaling
        << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_RESIZEMODE_T v) {
    switch (v) {
      case MMAL_RESIZE_NONE:  out << "NONE"; break;
      case MMAL_RESIZE_CROP:  out << "CROP"; break;
      case MMAL_RESIZE_BOX:   out << "BOX"; break;
      case MMAL_RESIZE_BYTES: out << "BYTES"; break;
      case MMAL_RESIZE_DUMMY: out << "DUMMY"; break;
      default:                out << "UNKNOWN"; break;
    }
    return out;
  }

}

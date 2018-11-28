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

// --------------------------------------------------------------------------
//
// Project includes
//
#include "camera.h"

#include <interface/mmal/util/mmal_util_params.h>
#include <interface/mmal/util/mmal_default_components.h>

// --------------------------------------------------------------------------
//
// Source:
//

namespace gui {

  namespace raspi {

    namespace camera {

#       define check_mmal_status(A) check_mmal_status_(A, #A)

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
      raspi_camera::raspi_camera (int num)
        : m_camera(nullptr)
        , m_encoder(nullptr)
        , m_encoder_connection(nullptr)
        , m_encoder_pool(nullptr)
        , m_camera_still_port(nullptr)
        , m_encoder_input_port(nullptr)
        , m_encoder_output_port(nullptr)
      {
        check_mmal_status(mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &m_camera));
        set_stereo_mode(MMAL_STEREOSCOPIC_MODE_NONE);
        set_camera_num(num);
        if (!m_camera->output_num) {
            throw std::runtime_error("Camera doesn't have output ports");
        }
        check_mmal_status(mmal_port_parameter_set_uint32(m_camera->control, MMAL_PARAMETER_CAMERA_CUSTOM_SENSOR_CONFIG, 0));
        m_camera_still_port = m_camera->output[MMAL_CAMERA_CAPTURE_PORT];

      }

      raspi_camera::~raspi_camera () {
        if (m_camera) {
          mmal_component_destroy(m_camera);
          m_camera = nullptr;
        }
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

      float raspi_camera::get_rational (uint32_t id) {
        MMAL_RATIONAL_T value = {0, 100};
        check_mmal_status(mmal_port_parameter_get_rational(m_camera->control, id, &value));
        return (float)value.num / (float)value.den;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_saturation (float saturation) {
        set_rational(MMAL_PARAMETER_SATURATION, saturation, -1.0F, 1.0F);
      }

      float raspi_camera::get_saturation () {
        return get_rational(MMAL_PARAMETER_SATURATION);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_sharpness (float sharpness) {
        set_rational(MMAL_PARAMETER_SHARPNESS, sharpness, -1.0F, 1.0F);
      }

      float raspi_camera::get_sharpness () {
        return get_rational(MMAL_PARAMETER_SHARPNESS);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_contrast (float contrast) {
        set_rational(MMAL_PARAMETER_CONTRAST, contrast, -1.0F, 1.0F);
      }

      float raspi_camera::get_contrast () {
        return get_rational(MMAL_PARAMETER_CONTRAST);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_brightness (float brightness) {
        set_rational(MMAL_PARAMETER_BRIGHTNESS, brightness, 0, 1.0F);
      }

      float raspi_camera::get_brightness () {
        return get_rational(MMAL_PARAMETER_BRIGHTNESS);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_iso (uint32_t iso) {
        check_mmal_status(mmal_port_parameter_set_uint32(m_camera->control, MMAL_PARAMETER_ISO, iso));
      }

      uint32_t raspi_camera::get_iso () {
        uint32_t iso;
        check_mmal_status(mmal_port_parameter_get_uint32(m_camera->control, MMAL_PARAMETER_ISO, &iso));
        return iso;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_shutter_speed (int speed) {
        check_mmal_status(mmal_port_parameter_set_uint32(m_camera->control, MMAL_PARAMETER_SHUTTER_SPEED, speed));
      }

      int raspi_camera::get_shutter_speed () {
        uint32_t speed;
        check_mmal_status(mmal_port_parameter_get_uint32(m_camera->control, MMAL_PARAMETER_SHUTTER_SPEED, &speed));
        return speed;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_analog_gain (float gain) {
        set_rational(MMAL_PARAMETER_ANALOG_GAIN, gain);
      }

      float raspi_camera::get_analog_gain () {
        return get_rational(MMAL_PARAMETER_ANALOG_GAIN);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_digital_gain (float gain) {
        set_rational(MMAL_PARAMETER_DIGITAL_GAIN, gain);
      }

      float raspi_camera::get_digital_gain () {
        return get_rational(MMAL_PARAMETER_DIGITAL_GAIN);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_video_stabilisation (bool vstabilisation) {
        MMAL_BOOL_T b = vstabilisation ? MMAL_TRUE : MMAL_FALSE;
        check_mmal_status(mmal_port_parameter_set_boolean(m_camera->control, MMAL_PARAMETER_VIDEO_STABILISATION, b));
      }

      bool raspi_camera::get_video_stabilisation () {
        MMAL_BOOL_T b = MMAL_FALSE;
        check_mmal_status(mmal_port_parameter_get_boolean(m_camera->control, MMAL_PARAMETER_VIDEO_STABILISATION, &b));
        return b;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_stats_pass (bool stats_pass) {
        MMAL_BOOL_T b = stats_pass ? MMAL_TRUE : MMAL_FALSE;
        check_mmal_status(mmal_port_parameter_set_boolean(m_camera->control, MMAL_PARAMETER_CAPTURE_STATS_PASS, b));
      }

      bool raspi_camera::get_stats_pass () {
        MMAL_BOOL_T b = MMAL_FALSE;
        check_mmal_status(mmal_port_parameter_get_boolean(m_camera->control, MMAL_PARAMETER_CAPTURE_STATS_PASS, &b));
        return b;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_exposure_compensation (int exp_comp) {
        check_mmal_status(mmal_port_parameter_set_int32(m_camera->control, MMAL_PARAMETER_EXPOSURE_COMP, exp_comp));
      }

      int raspi_camera::get_exposure_compensation () {
        int32_t value;
        check_mmal_status(mmal_port_parameter_get_int32(m_camera->control, MMAL_PARAMETER_EXPOSURE_COMP, &value));
        return value;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_awb_gains (float r_gain, float b_gain) {
        MMAL_PARAMETER_AWB_GAINS_T param = {{MMAL_PARAMETER_CUSTOM_AWB_GAINS,sizeof(param)}, {0,0}, {0,0}};

        param.r_gain.den = param.b_gain.den = 65536;
        param.r_gain.num = (unsigned int)(r_gain * 65536);
        param.b_gain.num = (unsigned int)(b_gain * 65536);
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &param.hdr));
      }

      void raspi_camera::get_awb_gains (float &r_gain, float &b_gain) {
        MMAL_PARAMETER_AWB_GAINS_T param = {{MMAL_PARAMETER_CUSTOM_AWB_GAINS,sizeof(param)}, {0,0}, {0,0}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &param.hdr));

        r_gain = static_cast<float>(param.r_gain.num) / static_cast<float>(param.r_gain.den);
        b_gain = static_cast<float>(param.b_gain.num) / static_cast<float>(param.b_gain.den);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_rotation (int rotation) {
        int my_rotation = ((rotation % 360 ) / 90) * 90;

        check_mmal_status(mmal_port_parameter_set_int32(m_camera->output[0], MMAL_PARAMETER_ROTATION, my_rotation));
        check_mmal_status(mmal_port_parameter_set_int32(m_camera->output[1], MMAL_PARAMETER_ROTATION, my_rotation));
        check_mmal_status(mmal_port_parameter_set_int32(m_camera->output[2], MMAL_PARAMETER_ROTATION, my_rotation));
      }

      int raspi_camera::get_rotation () {
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

      MMAL_PARAM_MIRROR_T raspi_camera::get_flip () {
        MMAL_PARAMETER_MIRROR_T mirror = {{MMAL_PARAMETER_MIRROR, sizeof(MMAL_PARAMETER_MIRROR_T)}, MMAL_PARAM_MIRROR_NONE};
        check_mmal_status(mmal_port_parameter_get(m_camera->output[0], &mirror.hdr));
        return mirror.value;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_crop (float x, float y, float w, float h) {
        MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};

        crop.rect.x = (65536 * x);
        crop.rect.y = (65536 * y);
        crop.rect.width = (65536 * w);
        crop.rect.height = (65536 * h);

        check_mmal_status(mmal_port_parameter_set(m_camera->control, &crop.hdr));
      }

      void raspi_camera::get_crop (float& x, float& y, float& w, float& h) {
        MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &crop.hdr));
        x = (float)crop.rect.x / 65536.0F;
        y = (float)crop.rect.y / 65536.0F;
        w = (float)crop.rect.width / 65536.0F;
        h = (float)crop.rect.height / 65536.0F;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_color_fx (bool enable, uint32_t u, uint32_t v) {
        MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT, sizeof(MMAL_PARAMETER_COLOURFX_T)}, enable, u, v};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &colfx.hdr));
      }

      void raspi_camera::get_color_fx (bool& enable, uint32_t& u, uint32_t& v) {
        MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT, sizeof(MMAL_PARAMETER_COLOURFX_T)}, 0, 0, 0};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &colfx.hdr));
        enable = colfx.enable != 0;
        u = colfx.u;
        v = colfx.v;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_stereo_mode (MMAL_STEREOSCOPIC_MODE_T mode, bool decimate, bool swap_eyes) {
        MMAL_PARAMETER_STEREOSCOPIC_MODE_T stereo = { {MMAL_PARAMETER_STEREOSCOPIC_MODE, sizeof(MMAL_PARAMETER_STEREOSCOPIC_MODE_T)},
                                    mode, decimate, swap_eyes};
        check_mmal_status(mmal_port_parameter_set(m_camera->output[0], &stereo.hdr));
        check_mmal_status(mmal_port_parameter_set(m_camera->output[1], &stereo.hdr));
        check_mmal_status(mmal_port_parameter_set(m_camera->output[2], &stereo.hdr));
      }

      void raspi_camera::get_stereo_mode (MMAL_STEREOSCOPIC_MODE_T& mode, bool& decimate, bool& swap_eyes) {
        MMAL_PARAMETER_STEREOSCOPIC_MODE_T stereo = { {MMAL_PARAMETER_STEREOSCOPIC_MODE, sizeof(MMAL_PARAMETER_STEREOSCOPIC_MODE_T)},
                                    MMAL_STEREOSCOPIC_MODE_NONE, MMAL_FALSE, MMAL_FALSE };
        check_mmal_status(mmal_port_parameter_get(m_camera->output[0], &stereo.hdr));
        mode = stereo.mode;
        decimate = stereo.decimate != 0;
        swap_eyes = stereo.swap_eyes != 0;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_drc (MMAL_PARAMETER_DRC_STRENGTH_T strength) {
        MMAL_PARAMETER_DRC_T drc = {{MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION, sizeof(MMAL_PARAMETER_DRC_T)}, strength};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &drc.hdr));
      }

      MMAL_PARAMETER_DRC_STRENGTH_T raspi_camera::get_drc () {
        MMAL_PARAMETER_DRC_T drc = {{MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION, sizeof(MMAL_PARAMETER_DRC_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &drc.hdr));
        return drc.strength;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_size (float w, float h) {
        MMAL_PARAMETER_CAMERA_CONFIG_T config = get_camera_config();
        config.max_stills_w = w;
        config.max_stills_h = h;
        set_camera_config(config);
      }

      void raspi_camera::get_size (int& w, int& h) {
        MMAL_PARAMETER_CAMERA_CONFIG_T config = get_camera_config();
        w = config.max_stills_w;
        h = config.max_stills_h;
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_CONFIG_T raspi_camera::get_camera_config () {
        MMAL_PARAMETER_CAMERA_CONFIG_T camConfig = {{MMAL_PARAMETER_CAMERA_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_CONFIG_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &camConfig.hdr));
        return camConfig;
      }

      void raspi_camera::set_camera_config (MMAL_PARAMETER_CAMERA_CONFIG_T camConfig) {
        camConfig.hdr = {MMAL_PARAMETER_CAMERA_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_CONFIG_T)};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &camConfig.hdr));
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_INFO_CAMERA_T raspi_camera::get_camera_info (int num) {
        MMAL_PARAMETER_CAMERA_INFO_T config = raspi_camera::get_config();
        if (num < config.num_cameras) {
          return config.cameras[num];
        }
        throw std::invalid_argument("camera number is invalid");
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_INFO_T raspi_camera::get_config () {
        MMAL_PARAMETER_CAMERA_INFO_T config = {{MMAL_PARAMETER_CAMERA_INFO, sizeof(MMAL_PARAMETER_CAMERA_INFO_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &config.hdr));
        return config;
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
      T raspi_camera::get_mode () {
        typename mode<T>::type param = {{mode<T>::id, sizeof(param)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &param.hdr));
        return param.value;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_metering_mode (MMAL_PARAM_EXPOSUREMETERINGMODE_T mode) {
        set_mode<MMAL_PARAM_EXPOSUREMETERINGMODE_T>(mode);
      }

      MMAL_PARAM_EXPOSUREMETERINGMODE_T raspi_camera::get_metering_mode () {
        return get_mode<MMAL_PARAM_EXPOSUREMETERINGMODE_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_exposure_mode (MMAL_PARAM_EXPOSUREMODE_T mode) {
        set_mode<MMAL_PARAM_EXPOSUREMODE_T>(mode);
      }

      MMAL_PARAM_EXPOSUREMODE_T raspi_camera::get_exposure_mode () {
        return get_mode<MMAL_PARAM_EXPOSUREMODE_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_flicker_avoid_mode (MMAL_PARAM_FLICKERAVOID_T mode) {
        set_mode<MMAL_PARAM_FLICKERAVOID_T>(mode);
      }

      MMAL_PARAM_FLICKERAVOID_T raspi_camera::get_flicker_avoid_mode () {
        return get_mode<MMAL_PARAM_FLICKERAVOID_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_awb_mode (MMAL_PARAM_AWBMODE_T awb_mode) {
        set_mode<MMAL_PARAM_AWBMODE_T>(awb_mode);
      }

      MMAL_PARAM_AWBMODE_T raspi_camera::get_awb_mode () {
        return get_mode<MMAL_PARAM_AWBMODE_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_image_fx (MMAL_PARAM_IMAGEFX_T imageFX) {
        set_mode<MMAL_PARAM_IMAGEFX_T>(imageFX);
      }

      MMAL_PARAM_IMAGEFX_T raspi_camera::get_image_fx () {
        return get_mode<MMAL_PARAM_IMAGEFX_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_camera_num (int32_t num) {
        set_mode<int32_t>(num);
      }

      int32_t raspi_camera::get_camera_num () {
        return get_mode<int32_t>();
      }

      // --------------------------------------------------------------------------

    } // camera

  } // raspi

} // gui

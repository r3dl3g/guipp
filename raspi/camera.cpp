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

// --------------------------------------------------------------------------
//
// Source:
//

namespace gui {

  namespace raspi {

    namespace camera {

      void check_mmal_status (MMAL_STATUS_T status) {
        if (status != MMAL_SUCCESS) {
          switch (status) {
            case MMAL_ENOMEM :   throw std::runtime_error("Out of memory"); break;
            case MMAL_ENOSPC :   throw std::runtime_error("Out of resources (other than memory)"); break;
            case MMAL_EINVAL:    throw std::invalid_argument("Argument is invalid"); break;
            case MMAL_ENOSYS :   throw std::runtime_error("Function not implemented"); break;
            case MMAL_ENOENT :   throw std::invalid_argument("No such file or directory"); break;
            case MMAL_ENXIO :    throw std::invalid_argument("No such device or address"); break;
            case MMAL_EIO :      throw std::runtime_error("I/O error"); break;
            case MMAL_ESPIPE :   throw std::runtime_error("Illegal seek"); break;
            case MMAL_ECORRUPT : throw std::runtime_error("Data is corrupt \attention FIXME: not POSIX"); break;
            case MMAL_ENOTREADY :throw std::runtime_error("Component is not ready \attention FIXME: not POSIX"); break;
            case MMAL_ECONFIG :  throw std::runtime_error("Component is not configured \attention FIXME: not POSIX"); break;
            case MMAL_EISCONN :  throw std::runtime_error("Port is already connected "); break;
            case MMAL_ENOTCONN : throw std::runtime_error("Port is disconnected"); break;
            case MMAL_EAGAIN :   throw std::runtime_error("Resource temporarily unavailable. Try again later"); break;
            case MMAL_EFAULT :   throw std::runtime_error("Bad address"); break;
            default :            throw std::runtime_error("Unknown status error"); break;
          }
        }
      }

      // --------------------------------------------------------------------------
      raspi_camera::raspi_camera ()
        : m_camera(nullptr)
      {}

      raspi_camera::~raspi_camera ()
      {}

      // --------------------------------------------------------------------------
      void raspi_camera::set_rational (uint32_t id, float v) {
         MMAL_RATIONAL_T value = {static_cast<int32_t>(v * 10000.0F), 10000};
         check_mmal_status(mmal_port_parameter_set_rational(m_camera->control, id, value));
      }

      void raspi_camera::set_rational (uint32_t id, float v, float min, float max) {
        if ((min <= v) && (v <= max)) {
           set_rational(id, v);
        } else {
           std::invalid_argument(ostreamfmt("Invalid rational value (" << min << " <= " << v << " <= " << max));
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

//      // --------------------------------------------------------------------------
//      void raspi_camera::set_colourFX (const MMAL_PARAM_COLOURFX_T& colourFX) {
//        MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT,sizeof(colfx)}, colourFX.enable, colourFX.u, colourFX.v};
//        check_mmal_status(mmal_port_parameter_set(m_camera->control, &param.hdr));
//     }

//      MMAL_PARAM_COLOURFX_T raspi_camera::get_colourFX () {
//        MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT,sizeof(colfx)}, 0, 0, 0};
//        check_mmal_status(mmal_port_parameter_get(m_camera->control, &param.hdr));
//        return MMAL_PARAM_COLOURFX_T{colfx.enable, colfx.u, colfx.v};
//      }

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
      void raspi_camera::set_ROI (float x, float y, float w, float h) {
        MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};

        crop.rect.x = (65536 * x);
        crop.rect.y = (65536 * y);
        crop.rect.width = (65536 * w);
        crop.rect.height = (65536 * h);

        check_mmal_status(mmal_port_parameter_set(m_camera->control, &crop.hdr));
      }

      void raspi_camera::get_ROI (float& x, float& y, float& w, float& h) {
        MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};
        check_mmal_status(mmal_port_parameter_get(m_camera->control, &crop.hdr));
        x = (float)crop.rect.x / 65536.0F;
        y = (float)crop.rect.y / 65536.0F;
        w = (float)crop.rect.width / 65536.0F;
        h = (float)crop.rect.height / 65536.0F;
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
      struct mode<MMAL_PARAMETER_DRC_STRENGTH_T> {
        using type = MMAL_PARAMETER_DRC_T;
        static constexpr int id = MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      void raspi_camera::set_mode (T m) {
        typename mode<T>::type param = {{mode<T>::id, sizeof(param)}, m};
        check_mmal_status(mmal_port_parameter_set(m_camera->control, &param.hdr));
      }

      template<typename T>
      T raspi_camera::get_mode () {
        typename mode<T>::type param = {{mode<T>::id, sizeof(param)}, T()};
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
      void raspi_camera::set_imageFX (MMAL_PARAM_IMAGEFX_T imageFX) {
        set_mode<MMAL_PARAM_IMAGEFX_T>(imageFX);
      }

      MMAL_PARAM_IMAGEFX_T raspi_camera::get_imageFX () {
        return get_mode<MMAL_PARAM_IMAGEFX_T>();
      }

//      // --------------------------------------------------------------------------
//      void raspi_camera::set_DRC (MMAL_PARAMETER_DRC_STRENGTH_T strength) {
//        set_mode<MMAL_PARAMETER_DRC_STRENGTH_T>(strength);
//      }

//      MMAL_PARAMETER_DRC_STRENGTH_T raspi_camera::get_DRC () {
//        return get_mode<MMAL_PARAMETER_DRC_STRENGTH_T>();
//      }
      // --------------------------------------------------------------------------

    } // camera

  } // raspi

} // gui

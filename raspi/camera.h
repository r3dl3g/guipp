/**
 * @copyright (c) 2018-2019 Ing. Buero Rothfuss
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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <fstream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/color.h>

// --------------------------------------------------------------------------
//#ifdef RASPI
#include <interface/mmal/mmal.h>
//#else
//typedef struct MMAL_COMPONENT_T {} MMAL_COMPONENT_T;
//#endif

namespace gui {

  namespace raspi {

    namespace camera {

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      class raspi_camera {
      public:
        raspi_camera ();
        ~raspi_camera ();

        void set_saturation (float saturation);
        float get_saturation ();

        void set_sharpness (float sharpness);
        float get_sharpness ();

        void set_contrast (float contrast);
        float get_contrast ();

        void set_brightness (float brightness);
        float get_brightness ();

        void set_iso (uint32_t iso);
        uint32_t get_iso ();

        void set_shutter_speed (int speed);
        int get_shutter_speed ();

        void set_analog_gain (float gain);
        float get_analog_gain ();

        void set_digital_gain (float gain);
        float get_digital_gain ();

        void set_metering_mode (MMAL_PARAM_EXPOSUREMETERINGMODE_T mode);
        MMAL_PARAM_EXPOSUREMETERINGMODE_T get_metering_mode ();

        void set_exposure_mode (MMAL_PARAM_EXPOSUREMODE_T mode);
        MMAL_PARAM_EXPOSUREMODE_T get_exposure_mode ();

        void set_flicker_avoid_mode (MMAL_PARAM_FLICKERAVOID_T mode);
        MMAL_PARAM_FLICKERAVOID_T get_flicker_avoid_mode ();

        void set_awb_mode (MMAL_PARAM_AWBMODE_T awb_mode);
        MMAL_PARAM_AWBMODE_T get_awb_mode ();

        void set_video_stabilisation (bool vstabilisation);
        bool get_video_stabilisation ();

        void set_exposure_compensation (int exp_comp);
        int get_exposure_compensation ();

        void set_awb_gains (float r_gain, float b_gain);
        void get_awb_gains (float& r_gain, float& b_gain);

        void set_imageFX (MMAL_PARAM_IMAGEFX_T imageFX);
        MMAL_PARAM_IMAGEFX_T get_imageFX ();

        void set_colourFX (const MMAL_PARAM_COLOURFX_T& colourFX);
        MMAL_PARAM_COLOURFX_T get_colourFX ();

        void set_rotation (int rotation);
        int get_rotation ();

        void set_flip (MMAL_PARAM_MIRROR_T mode);
        MMAL_PARAM_MIRROR_T get_flip ();

        void set_ROI (PARAM_FLOAT_RECT_T rect);
        PARAM_FLOAT_RECT_T get_ROI ();

        void set_DRC (MMAL_PARAMETER_DRC_STRENGTH_T strength);
        MMAL_PARAMETER_DRC_STRENGTH_T get_DRC ();

        void set_stats_pass (bool stats_pass);
        bool get_stats_pass ();


      private:
        void set_rational (uint32_t id, float v, float min, float max);
        void set_rational (uint32_t id, float v);
        float get_rational (uint32_t id);

        template<typename T>
        void set_mode (T mode);

        template<typename T>
        T get_mode ();

        MMAL_COMPONENT_T* canera_;
      };

    } // namespace camera

  } // namespace raspi

} // namespace gui


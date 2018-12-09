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
 * @brief     C++ API: Raspi encoder interface.
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <sstream>
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//

// --------------------------------------------------------------------------
#include "camera.h"

namespace gui {

  namespace raspi {

    namespace camera {

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      class raspi_encoder {
      public:
        using image_data = std::basic_string<uint8_t>;

        raspi_encoder (core::port camera);
        ~raspi_encoder ();

        image_data get_data () const;

        void clear_data ();

      protected:
        using buffer_type = std::basic_ostringstream<uint8_t>;

        core::port get_camera_still_port () const;

        void init ();
        void fini ();

        void capture (uint32_t timeout);
        void enable ();
        void disable ();

        static void callback_dispatcher (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
        void handle_callback (core::port&, core::buffer&);

        void add_data (const core::buffer&);

        VCOS_SEMAPHORE_T m_complete_semaphore;

        buffer_type m_buffer;
        core::pool m_buffer_pool;

        core::port m_camera;
      };

      // --------------------------------------------------------------------------
      class raspi_raw_encoder : public raspi_encoder {
      public:
        using super = raspi_encoder;

        raspi_raw_encoder (core::port camera, MMAL_FOURCC_T encoding = MMAL_ENCODING_BAYER_SRGGB10DPCM8);
        ~raspi_raw_encoder ();

        void capture (uint32_t timeout = 1000);
        void enable ();
        void disable ();

        core::port get_output_port ();

        void init (MMAL_FOURCC_T encoding);
        void fini ();
      };

      // --------------------------------------------------------------------------
      class raspi_image_encoder : public raspi_encoder {
      public:
        using super = raspi_encoder;

        raspi_image_encoder (core::port camera, MMAL_FOURCC_T encoding = MMAL_ENCODING_PNG);
        ~raspi_image_encoder ();

        void capture (uint32_t timeout = 1000);
        void enable ();
        void disable ();

        core::port& get_output_port ();

        void init (MMAL_FOURCC_T encoding);
        void fini ();

      private:
        core::component m_encoder;
        core::connection m_encoder_connection;

        core::port m_encoder_input_port;
        core::port m_encoder_output_port;
      };

      // --------------------------------------------------------------------------
      class raspi_resize_encoder : public raspi_encoder {
      public:
        using super = raspi_encoder;

        raspi_resize_encoder (core::port camera);
        ~raspi_resize_encoder ();

        void capture (uint32_t timeout = 1000);
        void enable ();
        void disable ();

        core::port& get_output_port ();

        void init (MMAL_FOURCC_T encoding);
        void fini ();

      private:
        core::component m_encoder;
        core::connection m_encoder_connection;

        core::port m_encoder_input_port;
        core::port m_encoder_output_port;
      };

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // namespace camera

  } // namespace raspi

} // namespace gui


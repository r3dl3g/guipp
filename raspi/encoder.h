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

        raspi_encoder (raspi_camera& camera);
        ~raspi_encoder ();

        void add_data (uint8_t *data, size_t size);
        image_data get_data () const;

        void clear_data ();

      protected:
        using buffer_type = std::basic_ostringstream<uint8_t>;

        MMAL_PORT_T* get_camera_still_port () const;

        void capture (uint32_t timeout);

        static void callback_dispatcher (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
        void handle_callback (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);

        VCOS_SEMAPHORE_T m_complete_semaphore;

        buffer_type m_buffer;
        MMAL_POOL_T* m_buffer_pool;

        raspi_camera& m_camera;
      };

      class raspi_raw_encoder : public raspi_encoder {
      public:
        using super = raspi_encoder;

        raspi_raw_encoder (raspi_camera& camera);
        ~raspi_raw_encoder ();

        void capture (uint32_t timeout = 1000);

      };

      class raspi_image_encoder : public raspi_encoder {
      public:
        using super = raspi_encoder;

        raspi_image_encoder (raspi_camera& camera, MMAL_FOURCC_T encoding = MMAL_ENCODING_PNG);
        ~raspi_image_encoder ();

        void capture (uint32_t timeout = 1000);

      private:
        MMAL_COMPONENT_T* m_encoder;
        MMAL_CONNECTION_T* m_encoder_connection;

        MMAL_PORT_T* m_encoder_input_port;
        MMAL_PORT_T* m_encoder_output_port;
      };

    } // namespace camera

  } // namespace raspi

} // namespace gui


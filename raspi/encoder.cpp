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
#include "encoder.h"

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
#     define check_mmal_status(A) check_mmal_status_(A, #A)

      void check_mmal_status_ (MMAL_STATUS_T status, const char* info);

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_encoder::raspi_encoder (raspi_camera& camera)
        : m_camera(camera)
        , m_buffer_pool(nullptr)
      {
        VCOS_STATUS_T vcos_status = vcos_semaphore_create(&m_complete_semaphore, "RaspiStill-sem", 0);
        vcos_assert(vcos_status == VCOS_SUCCESS);
      }

      // --------------------------------------------------------------------------
      raspi_encoder::~raspi_encoder () {
        vcos_semaphore_delete(&m_complete_semaphore);
      }

      // --------------------------------------------------------------------------
      MMAL_PORT_T* raspi_encoder::get_camera_still_port () const {
        return m_camera.get_camera_still_port();
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::capture (uint32_t timeout) {
        clear_data();
        m_camera.capture();
        vcos_semaphore_wait_timeout(&m_complete_semaphore, timeout);
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::add_data (uint8_t *data, size_t size) {
        m_buffer.write(data, size);
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::clear_data () {
        buffer_type temp;
        m_buffer.swap(temp);
      }

      // --------------------------------------------------------------------------
      auto raspi_encoder::get_data () const -> image_data {
        return m_buffer.str();
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::callback_dispatcher (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
//        LogTrace << "handle_callback cmd:" << buffer->cmd << ", length:" << buffer->length;
        raspi_encoder* encoder = (raspi_encoder*)(port->userdata);
        if (encoder && (0 == buffer->cmd)) {
          encoder->handle_callback(port, buffer);
        }
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::handle_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
        mmal_buffer_header_mem_lock(buffer);
        if (buffer->length) {
          add_data(buffer->data + buffer->offset, buffer->length);
        }
        bool complete = (buffer->flags & (MMAL_BUFFER_HEADER_FLAG_FRAME_END | MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED));
        mmal_buffer_header_mem_unlock(buffer);
        mmal_buffer_header_release(buffer);
        if (port->is_enabled && m_buffer_pool) {
          MMAL_BUFFER_HEADER_T *new_buffer = mmal_queue_get(m_buffer_pool->queue);
          if (new_buffer) {
            check_mmal_status(mmal_port_send_buffer(port, new_buffer));
          }
        }

        if (complete) {
          vcos_semaphore_post(&m_complete_semaphore);
        }
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_raw_encoder::raspi_raw_encoder (raspi_camera& camera, MMAL_FOURCC_T encoding)
        : super(camera)
      {
        auto camera_still_port = get_camera_still_port();
        camera_still_port->format->encoding = encoding;
        if (camera_still_port->buffer_size < camera_still_port->buffer_size_min) {
          camera_still_port->buffer_size = camera_still_port->buffer_size_min;
        }
        if (camera_still_port->buffer_num < camera_still_port->buffer_num_min) {
          camera_still_port->buffer_num = camera_still_port->buffer_num_min;
        }
        check_mmal_status(mmal_port_format_commit(camera_still_port));
        m_buffer_pool = mmal_port_pool_create(camera_still_port, camera_still_port->buffer_num, camera_still_port->buffer_size);
      }

      // --------------------------------------------------------------------------
      raspi_raw_encoder::~raspi_raw_encoder () {
        if (m_buffer_pool) {
          mmal_port_pool_destroy(get_camera_still_port(), m_buffer_pool);
          m_buffer_pool = nullptr;
        }
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::capture (uint32_t timeout) {
        auto camera_still_port = get_camera_still_port();

        camera_still_port->userdata = (struct MMAL_PORT_USERDATA_T*)this;
        check_mmal_status(mmal_port_enable(camera_still_port, raspi_encoder::callback_dispatcher));

        int num = mmal_queue_length(m_buffer_pool->queue);
        for (int i = 0; i < num; i++ ) {
          MMAL_BUFFER_HEADER_T *new_buffer = mmal_queue_get(m_buffer_pool->queue);
          if (new_buffer) {
            check_mmal_status(mmal_port_send_buffer(camera_still_port, new_buffer));
          }
        }

        super::capture(timeout);

        check_mmal_status(mmal_port_flush(camera_still_port));
        check_mmal_status(mmal_port_disable(camera_still_port));
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_image_encoder::raspi_image_encoder (raspi_camera& camera, MMAL_FOURCC_T encoding)
        : super(camera)
        , m_encoder(nullptr)
        , m_encoder_connection(nullptr)
        , m_encoder_input_port(nullptr)
        , m_encoder_output_port(nullptr)
      {
        check_mmal_status(mmal_component_create(MMAL_COMPONENT_DEFAULT_IMAGE_ENCODER, &m_encoder));
        if ( !m_encoder->input_num || !m_encoder->output_num) {
          throw std::invalid_argument(ostreamfmt("Encoder has no input/output ports (in:" << m_encoder->input_num << ", out:" << m_encoder->output_num << ")"));
        }

        m_encoder_input_port = m_encoder->input[0];
        m_encoder_output_port = m_encoder->output[0];
        mmal_format_copy(m_encoder_output_port->format, m_encoder_input_port->format);

        m_encoder_output_port->format->encoding = encoding;
        m_encoder_output_port->buffer_size = m_encoder_output_port->buffer_size_recommended;
        if (m_encoder_output_port->buffer_size < m_encoder_output_port->buffer_size_min) {
          m_encoder_output_port->buffer_size = m_encoder_output_port->buffer_size_min;
        }
        m_encoder_output_port->buffer_num = m_encoder_output_port->buffer_num_recommended;
        if (m_encoder_output_port->buffer_num < m_encoder_output_port->buffer_num_min) {
          m_encoder_output_port->buffer_num = m_encoder_output_port->buffer_num_min;
        }

        check_mmal_status(mmal_port_format_commit(m_encoder_output_port));
        check_mmal_status(mmal_component_enable(m_encoder));
        m_buffer_pool = mmal_port_pool_create(m_encoder_output_port, m_encoder_output_port->buffer_num, m_encoder_output_port->buffer_size);

        check_mmal_status(mmal_connection_create(&m_encoder_connection, get_camera_still_port(), m_encoder_input_port,
                                                 MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT));
        check_mmal_status(mmal_connection_enable(m_encoder_connection));
      }

      // --------------------------------------------------------------------------
      raspi_image_encoder::~raspi_image_encoder () {
        if (m_buffer_pool) {
          mmal_port_pool_destroy(m_encoder_output_port, m_buffer_pool);
          m_buffer_pool = nullptr;
        }
        if (m_encoder_connection) {
          mmal_connection_disable(m_encoder_connection);
          mmal_connection_destroy(m_encoder_connection);
          m_encoder_connection = nullptr;
        }
        if (m_encoder) {
          mmal_component_destroy(m_encoder);
          m_encoder = nullptr;
        }
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::capture (uint32_t timeout) {

        if (m_encoder_output_port->is_enabled) {
          throw std::invalid_argument("Encoder putput port has no enabled!");
        }

        m_encoder_output_port->userdata = (struct MMAL_PORT_USERDATA_T*)this;
        check_mmal_status(mmal_port_enable(m_encoder_output_port, raspi_encoder::callback_dispatcher));

        int num = mmal_queue_length(m_buffer_pool->queue);
        for (int i = 0; i < num; i++ ) {
          MMAL_BUFFER_HEADER_T *new_buffer = mmal_queue_get(m_buffer_pool->queue);
          if (new_buffer) {
            check_mmal_status(mmal_port_send_buffer(m_encoder_output_port, new_buffer));
          }
        }

        super::capture(timeout);

        check_mmal_status(mmal_port_flush(m_encoder_output_port));
        check_mmal_status(mmal_port_disable(m_encoder_output_port));
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // camera

  } // raspi

} // gui

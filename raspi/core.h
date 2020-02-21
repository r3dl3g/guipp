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
#include <vector>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>

#include <interface/mmal/util/mmal_util.h>
#include <interface/mmal/util/mmal_util_params.h>
#include <interface/mmal/util/mmal_connection.h>

namespace gui {

  namespace raspi {

    namespace core {

      // --------------------------------------------------------------------------
      struct four_cc {
        four_cc (MMAL_FOURCC_T t = MMAL_FOURCC(' ',' ',' ',' '))
          : type{ .uint32 = t }
        {}

        operator bool () const {
          return type.uint32 != 0;
        }

        operator MMAL_FOURCC_T () const {
          return type.uint32;
        }

        bool operator== (const four_cc& rhs) const {
          return type.uint32 == rhs.type.uint32;
        }

        bool operator!= (const four_cc& rhs) const {
          return type.uint32 != rhs.type.uint32;
        }

        four_cc& operator= (MMAL_FOURCC_T t) {
          type.uint32 = t;
          return *this;
        }

        union {
          char          char4[4];
          MMAL_FOURCC_T uint32;
        } type;
      };

      // --------------------------------------------------------------------------
      struct crop {
        crop (float x_ = 0.0F, float y_ = 0.0F, float w_ = 1.0F, float h_ = 1.0F)
          : x(x_) , y(y_) , width(w_) , height(h_)
        {}

        float x;
        float y;
        float width;
        float height;
      };


      // --------------------------------------------------------------------------
      class buffer {
      public:
        typedef uint8_t* iterator;
        typedef const uint8_t* const_iterator;

        buffer (MMAL_BUFFER_HEADER_T *d)
          : data(d)
        {}

        buffer (const buffer& rhs)
          : data(rhs.data) {
          LogTrace << "Acquire buffer";
          mmal_buffer_header_acquire(data);
        }

        buffer (buffer&& rhs)
          : data(std::move(rhs.data)) {
          rhs.detach();
        }

        ~buffer () {
          release();
        }

        void operator= (buffer&& rhs) {
          if (&rhs != this) {
            data = std::move(rhs.data);
            rhs.detach();
          }
        }

        void release () {
          if (data) {
            LogTrace << "Release buffer";
            mmal_buffer_header_release(data);
            data = nullptr;
          }
        }

        void detach () {
          LogTrace << "Detach buffer";
          data = nullptr;
        }

        void reset () {
          mmal_buffer_header_reset(data);
        }

        void lock () {
          mmal_buffer_header_mem_lock(data);
        }

        void unlock () {
          mmal_buffer_header_mem_unlock(data);
        }

        bool is_valid () const {
          return data != nullptr;
        }

        uint32_t get_command () const {
          return data->cmd;
        }

        const uint8_t* get_data () const {
          return data->data + data->offset;
        }

        uint8_t* get_data () {
          return data->data + data->offset;
        }

        uint32_t get_length () const {
          return data->length;
        }

        iterator begin () {
            return get_data();
        }

        iterator end () {
            return get_data() + get_length();
        }

        const_iterator begin () const {
            return get_data();
        }

        const_iterator end () const {
            return get_data() + get_length();
        }

        uint32_t get_flags () const {
          return data->flags;
        }

        int64_t get_presentation_timestamp () const {
          return data->pts;
        }

        int64_t get_decode_timestamp () const {
          return data->dts;
        }

      private:
        friend class port;
        MMAL_BUFFER_HEADER_T *data;
      };

      // --------------------------------------------------------------------------
      class pool {
      public:
        pool (MMAL_POOL_T *d = nullptr, MMAL_PORT_T* p = nullptr)
          : data(d)
          , port(p)
        {}

        pool (pool&& rhs)
          : data(std::move(rhs.data))
          , port(std::move(rhs.port))
        {
          rhs.detach();
        }

        ~pool () {
          destroy();
        }

        void operator= (pool&& rhs) {
          if (&rhs != this) {
            data = std::move(rhs.data);
            port = std::move(rhs.port);
            rhs.detach();
          }
        }

        void destroy () {
          if (data && port) {
           LogTrace << "Destroy pool";
            mmal_port_pool_destroy(port, data);
            detach();
          }
        }

        void detach () {
          LogTrace << "Detach pool";
          data = nullptr;
          port = nullptr;
        }

        bool is_valid () const {
          return data != nullptr;
        }

        buffer get_buffer () {
          return buffer(mmal_queue_get(data->queue));
        }

        buffer get_buffer (VCOS_UNSIGNED timeout) {
          return buffer(mmal_queue_timedwait(data->queue, timeout));
        }

        buffer get_buffer_wait () {
          return buffer(mmal_queue_wait(data->queue));
        }

        unsigned int queue_length () const {
          return mmal_queue_length(data->queue);
        }

      private:
        MMAL_POOL_T *data;
        MMAL_PORT_T* port;
      };

      // --------------------------------------------------------------------------
      class port;

      // --------------------------------------------------------------------------
      class connection {
      public:
        connection ();

        connection (const connection& rhs);
        connection (connection&& rhs);

        ~connection ();

        void operator= (const connection& rhs);
        void operator= (connection&& rhs);

        MMAL_STATUS_T connect (port& from, port& to, uint32_t flags);

        inline bool is_valid () const {
          return data != nullptr;
        }

        inline bool is_enabled () const {
          return data->is_enabled;
        }

        MMAL_STATUS_T enable ();
        MMAL_STATUS_T disable ();
        MMAL_STATUS_T acquire ();
        MMAL_STATUS_T release ();
        MMAL_STATUS_T destroy ();

        void detach ();

      private:
        MMAL_CONNECTION_T* data;
      };

      // --------------------------------------------------------------------------
      class port {
      public:
        struct size_num {
          uint32_t num;
          uint32_t size;

          void operator |= (const size_num&);
        };

        port (MMAL_PORT_T* d = nullptr)
          : data(d)
        {}

        ~port ();

        bool is_valid () const {
          return data != nullptr;
        }

        MMAL_STATUS_T enable (MMAL_PORT_BH_CB_T cb);

        MMAL_STATUS_T disable ();

        MMAL_STATUS_T flush () {
          return mmal_port_flush(data);
        }

        bool is_pass_through () const {
          return data->capabilities & MMAL_PORT_CAPABILITY_PASSTHROUGH;
        }

        bool wants_allocate_buffer_payloads () const {
          return data->capabilities & MMAL_PORT_CAPABILITY_ALLOCATION;
        }

        bool support_format_change_events () const {
          return data->capabilities & MMAL_PORT_CAPABILITY_SUPPORTS_EVENT_FORMAT_CHANGE;
        }

        MMAL_STATUS_T set (const MMAL_PARAMETER_HEADER_T& param) {
          return mmal_port_parameter_set(data, &param);
        }

        MMAL_STATUS_T get (MMAL_PARAMETER_HEADER_T& param) const {
          return mmal_port_parameter_get(data, &param);
        }

        template<typename T>
        auto get (uint32_t id) const -> decltype(T::value) {
          T t = {{id, sizeof(T)}};
          get(t.hdr);
          return t.value;
        }

        template<typename T>
        MMAL_STATUS_T set (uint32_t id, decltype(T::value) v) {
          T t = {{id, sizeof(T)}, v};
          return set(t.hdr);
        }

        MMAL_STATUS_T set_bool (uint32_t id, bool v);
        bool get_bool (uint32_t id);

        MMAL_STATUS_T set_int32 (uint32_t id, int32_t v) {
          return set<MMAL_PARAMETER_INT32_T>(id, v);
        }

        int32_t get_int32 (uint32_t id) {
          return get<MMAL_PARAMETER_INT32_T>(id);
        }

        MMAL_STATUS_T set_uint32 (uint32_t id, uint32_t v) {
          return set<MMAL_PARAMETER_UINT32_T>(id, v);
        }

        uint32_t get_uint32 (uint32_t id) {
          return get<MMAL_PARAMETER_UINT32_T>(id);
        }

        MMAL_STATUS_T set_float (uint32_t id, float v, int den) {
          return set<MMAL_PARAMETER_RATIONAL_T>(id, {static_cast<int32_t>(v * den), den});
        }

        MMAL_STATUS_T set_float (uint32_t id, float v) {
          return set_float(id, v, 65536);
        }

        float get_float (uint32_t id) {
          const auto v = get<MMAL_PARAMETER_RATIONAL_T>(id);
          return (float)v.num / (float)v.den;
        }

        MMAL_STATUS_T send_buffer (buffer& b) {
          MMAL_STATUS_T r = mmal_port_send_buffer(data, b.data);
          b.detach();
          return r;
        }

        MMAL_STATUS_T connect (port& rhs);

        MMAL_STATUS_T disconnect () {
          LogTrace << "Disconnect port";
          return mmal_port_disconnect(data);
        }

        const char* get_name () const {
          return data->name;
        }

        MMAL_PORT_TYPE_T get_type () const {
          return data->type;
        }

        uint32_t get_bitrate () const {
          return data->format->bitrate;
        }

        bool is_enabled () const {
          return data->is_enabled;
        }

        size_num get_min_buffer_size () const {
          return {data->buffer_num_min, data->buffer_size_min};
        }

        size_num get_recommended_buffer_size () const {
          return {data->buffer_num_recommended, data->buffer_size_recommended};
        }

        size_num get_buffer_size () const {
          return {data->buffer_num, data->buffer_size};
        }

        pool create_buffer_pool () const {
          return pool(mmal_port_pool_create(data, data->buffer_num, data->buffer_size), data);
        }

        MMAL_ES_SPECIFIC_FORMAT_T get_format () const {
          return *data->format->es;
        }

        four_cc get_encoding () const {
          return data->format->encoding;
        }

        void set_buffer_size (size_num sz);
        void set_format (const MMAL_ES_SPECIFIC_FORMAT_T& f);
        void set_encoding (four_cc f);

        MMAL_STATUS_T commit_format_change () {
          LogTrace << "port format commit";
          return mmal_port_format_commit(data);
        }

        void copy_format_from (const port& rhs) {
          mmal_format_copy(data->format, rhs.data->format);
        }

        void set_user_data (void* ptr) {
          data->userdata = (struct MMAL_PORT_USERDATA_T*)ptr;
        }

        MMAL_STATUS_T capture ();

        std::vector<four_cc> get_supported_encodings () const;

        MMAL_PARAMETER_RESIZE_T get_resize () const;
        MMAL_STATUS_T set_resize (MMAL_PARAMETER_RESIZE_T config);

        MMAL_STATUS_T set_crop (const MMAL_RECT_T&);
        MMAL_RECT_T get_crop () const;

      private:
        friend class connection;
        MMAL_PORT_T* data;
      };

      // --------------------------------------------------------------------------
      class component {
      public:
        component (MMAL_COMPONENT_T* d = nullptr)
          : data(d)
        {}

        component (const char *name)
          : data(nullptr)
        {
          create(name);
        }

//        component (const component& rhs)
//          : data (rhs.data)
//        {
//          LogTrace << "Acquire component";
//          mmal_component_acquire(data);
//        }

        ~component () {
          destroy();
        }

        bool is_valid () const {
          return data != nullptr;
        }

        void destroy () {
          if (data) {
            LogTrace << "Destroy component";
            mmal_component_destroy(data);
            data = nullptr;
          }
        }

        MMAL_STATUS_T create (const char *name) {
          if (data) {
            destroy();
          }
          LogTrace << "Create component";
          return mmal_component_create(name, &data);
        }

        MMAL_STATUS_T enable () {
//          if (!data->is_enabled) {
            LogTrace << "Enable component";
            return mmal_component_enable(data);
//          }
//          return MMAL_SUCCESS;
        }

        MMAL_STATUS_T disable () {
          if (data->is_enabled) {
            LogTrace << "Disable component";
            return mmal_component_disable(data);
          }
          return MMAL_SUCCESS;
        }

        const char* get_name () const {
          return data->name;
        }

        bool is_enabled () const {
          return data->is_enabled;
        }

        uint32_t num_output_ports () const {
          return data->output_num;
        }

        uint32_t num_input_ports () const {
          return data->input_num;
        }

        uint32_t num_clock_ports () const {
          return data->clock_num;
        }

        port control_port () const {
          return port(data->control);
        }

        port input_port (int num) const {
          return port(data->input[num]);
        }

        port output_port (int num) const {
          return port(data->output[num]);
        }

        port clock_port (int num) const {
          return port(data->clock[num]);
        }

        port preview_port () const {
          return output_port(0);
        }

        port video_port () const {
          return output_port(1);
        }

        port still_port () const {
          return output_port(2);
        }

      private:
        MMAL_COMPONENT_T* data;
      };

      // --------------------------------------------------------------------------
      class semaphore {
      public:
        semaphore ()
        {}

        semaphore (const char *name, VCOS_UNSIGNED initial_count = 0) {
          create(name, initial_count);
        }

        void create (const char *name, VCOS_UNSIGNED initial_count = 0) {
          LogTrace << "Create semaphore";
          VCOS_STATUS_T vcos_status = vcos_semaphore_create(&m_semaphore, name, initial_count);
          vcos_assert(vcos_status == VCOS_SUCCESS);
        }

        ~semaphore () {
          LogTrace << "Delete semaphore";
          vcos_semaphore_delete(&m_semaphore);
        }

        void wait (VCOS_UNSIGNED timeout) {
          LogTrace << "semaphore::wait(" << timeout << ")";
          vcos_semaphore_wait_timeout(&m_semaphore, timeout);
          LogTrace << "semaphore::wait finish";
        }

        void wait () {
          vcos_semaphore_wait(&m_semaphore);
        }

        void try_wait () {
          vcos_semaphore_trywait(&m_semaphore);
        }

        void post () {
          LogTrace << "semaphore::post";
          vcos_semaphore_post(&m_semaphore);
        }

      private:
        VCOS_SEMAPHORE_T m_semaphore;
      };

      // --------------------------------------------------------------------------
      class mutex {
      public:
        mutex (const char *name) {
          LogTrace << "create mutex";
          VCOS_STATUS_T vcos_status = vcos_mutex_create(&m_mutex, name);
          vcos_assert(vcos_status == VCOS_SUCCESS);
        }

        ~mutex () {
          LogTrace << "Delete mutex";
          vcos_mutex_delete(&m_mutex);
        }

        void lock () {
          vcos_mutex_lock(&m_mutex);
        }

        void unlock () {
          vcos_mutex_unlock(&m_mutex);
        }

        void try_lock () {
          vcos_mutex_trylock(&m_mutex);
        }

        bool is_locked () {
          return vcos_mutex_is_locked(&m_mutex) != 0;
        }

      private:
        VCOS_MUTEX_T m_mutex;
      };

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // namespace core

  } // namespace raspi

} // namespace gui

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
#include <memory>

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

#   define check_mmal_status(A) check_mmal_status_(A, #A)
    bool check_mmal_status_ (MMAL_STATUS_T status, const char* info);

#   define check_vcos_status(A) check_vcos_status_(A, #A)
    bool check_vcos_status_ (VCOS_STATUS_T status, const char* info);

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

        buffer (MMAL_BUFFER_HEADER_T *d = nullptr);

        buffer (const buffer& rhs);
        buffer (buffer&& rhs);

        ~buffer ();

        const buffer& operator= (const buffer& rhs);
        const buffer& operator= (buffer&& rhs);

        void acquire ();
        void release ();
        void detach ();

        void reset ();

        void lock ();
        void unlock ();

        bool is_valid () const;

        uint32_t get_command () const;

        const uint8_t* get_data () const;
        uint8_t* get_data ();
        uint32_t get_length () const;

        iterator begin ();
        iterator end ();
        const_iterator begin () const;
        const_iterator end () const;

        uint32_t get_flags () const;
        int64_t get_presentation_timestamp () const;
        int64_t get_decode_timestamp () const;

      private:
        friend class port;
        MMAL_BUFFER_HEADER_T *data;
      };

      // --------------------------------------------------------------------------
      class port {
      public:
        struct size_num {
          uint32_t num;
          uint32_t size;

          void operator |= (const size_num&);
        };

        port (MMAL_PORT_T* = nullptr);
        ~port ();

        bool is_valid () const;

        bool enable (MMAL_PORT_BH_CB_T cb);
        bool disable ();
        void detach ();

        bool flush ();
        bool is_pass_through () const;
        bool wants_allocate_buffer_payloads () const;
        bool support_format_change_events () const;

        bool set (const MMAL_PARAMETER_HEADER_T& param);
        bool get (MMAL_PARAMETER_HEADER_T& param) const;

        template<typename T>
        auto get (uint32_t id) const -> decltype(T::value) const {
          T t = {{id, sizeof(T)}};
          get(t.hdr);
          return t.value;
        }

        template<typename T>
        bool set (uint32_t id, decltype(T::value) v) {
          T t = {{id, sizeof(T)}, v};
          return set(t.hdr);
        }

        bool set_bool (uint32_t id, bool v);
        bool get_bool (uint32_t id) const;

        bool set_int32 (uint32_t id, int32_t v);
        int32_t get_int32 (uint32_t id) const;

        bool set_uint32 (uint32_t id, uint32_t v);
        uint32_t get_uint32 (uint32_t id) const;

        bool set_float (uint32_t id, float v, int den);
        bool set_float (uint32_t id, float v);
        float get_float (uint32_t id) const;

        bool send_buffer (buffer& b);

        const char* get_name () const;
        bool get_type () const;
        uint32_t get_bitrate () const;
        bool is_enabled () const;

        size_num get_min_buffer_size () const;
        size_num get_recommended_buffer_size () const;
        size_num get_buffer_size () const;
        void set_buffer_size (size_num sz);

        MMAL_ES_SPECIFIC_FORMAT_T get_specific_format () const;
        four_cc get_encoding () const;

        void set_specific_format (const MMAL_ES_SPECIFIC_FORMAT_T& f);
        void set_format (const MMAL_ES_FORMAT_T& f);
        void set_encoding (four_cc f);

        bool commit_format_change ();

        void copy_format_from (const port& rhs);
        void copy_full_format_from (const port& rhs);

        void set_user_data (void* ptr);

        bool capture ();

        std::vector<four_cc> get_supported_encodings () const;

        MMAL_PARAMETER_RESIZE_T get_resize () const;
        bool set_resize (MMAL_PARAMETER_RESIZE_T config);

        bool set_crop (const MMAL_RECT_T&);
        MMAL_RECT_T get_crop () const;

      private:
        friend class connection;
        friend class pool;

        MMAL_PORT_T* data;
      };

      // --------------------------------------------------------------------------
      class pool {
      public:

        pool ();
        ~pool ();

        bool create (port& p);
        void destroy ();
        void detach ();

        bool is_valid () const;

        buffer get_buffer ();
        buffer get_buffer (VCOS_UNSIGNED timeout);
        buffer get_buffer_wait ();
        unsigned int queue_length ();

      private:
        MMAL_POOL_T *data;
        port port_;
      };

      // --------------------------------------------------------------------------
      class connection {
      public:
        connection (MMAL_CONNECTION_T* data = nullptr);
        connection (const connection& rhs);
        connection (connection&& rhs);

        ~connection ();

        void operator= (const connection& rhs);
        void operator= (connection&& rhs);

        bool is_valid () const;
        bool is_enabled () const;

        bool connect (port& from, port& to, uint32_t flags);

        bool enable ();
        bool disable ();
        bool acquire ();
        bool release ();
        void detach ();
        bool destroy ();

      private:
        MMAL_CONNECTION_T* data;
      };

      // --------------------------------------------------------------------------
      class component {
      public:
        component (MMAL_COMPONENT_T* d = nullptr);
        component (const char *name);

        component (const component& rhs);
        component (component&& rhs);

        ~component ();

        void operator= (const component& rhs);
        void operator= (component&& rhs);

        bool is_valid () const;

        bool enable ();
        bool disable ();
        bool acquire ();
        bool release ();
        void detach ();
        bool destroy ();

        bool create (const char *name);

        const char* get_name () const;
        bool is_enabled () const;

        uint32_t num_output_ports () const;
        uint32_t num_input_ports () const;
        uint32_t num_clock_ports () const;

        port control_port () const;
        port input_port (int num) const;
        port output_port (int num) const;
        port clock_port (int num) const;

        port preview_port () const;
        port video_port () const;
        port still_port () const;

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

        bool create (const char *name, VCOS_UNSIGNED initial_count = 0) {
          LogTrace << "Create semaphore";
          return check_vcos_status(vcos_semaphore_create(&m_semaphore, name, initial_count));
        }

        ~semaphore () {
          LogTrace << "Delete semaphore";
          vcos_semaphore_delete(&m_semaphore);
        }

        bool wait (VCOS_UNSIGNED timeout) {
          LogTrace << "semaphore::wait(" << timeout << ")";
          VCOS_STATUS_T vcos_status = vcos_semaphore_wait_timeout(&m_semaphore, timeout);
          LogTrace << "semaphore::wait finish";
          return check_vcos_status(vcos_status);
        }

        void wait () {
          vcos_semaphore_wait(&m_semaphore);
        }

        bool try_wait () {
          return check_vcos_status(vcos_semaphore_trywait(&m_semaphore));
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
          check_vcos_status(vcos_mutex_create(&m_mutex, name));
        }

        ~mutex () {
          LogTrace << "Delete mutex";
          vcos_mutex_delete(&m_mutex);
        }

        bool lock () {
          return check_vcos_status(vcos_mutex_lock(&m_mutex));
        }

        void unlock () {
          vcos_mutex_unlock(&m_mutex);
        }

        bool try_lock () {
          return check_vcos_status(vcos_mutex_trylock(&m_mutex));
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

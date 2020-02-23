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
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <util/ostreamfmt.h>
#include <logging/logger.h>

// --------------------------------------------------------------------------
//
// Project includes
//
#include "core.h"

// --------------------------------------------------------------------------
//
// Source:
//

namespace gui {

  namespace raspi {

    namespace core {

      // --------------------------------------------------------------------------
      void check_null_ptr_ (void* ptr, const char* file, int line) {
        if (!ptr) {
          throw std::invalid_argument(ostreamfmt("pointer can not be null in " << file << " at " << line));
        }
      }

#     define check_null_ptr(ptr) check_null_ptr_(ptr, __FILE__, __LINE__)

      // --------------------------------------------------------------------------
      buffer::buffer (MMAL_BUFFER_HEADER_T *d)
        : data(d)
      {}

      buffer::buffer (const buffer& rhs)
        : data(rhs.data) {
        acquire();
      }

      buffer::buffer (buffer&& rhs)
        : data(std::move(rhs.data)) {
        rhs.detach();
      }

      buffer::~buffer () {
        release();
      }

      const buffer& buffer::operator= (const buffer& rhs) {
        if (&rhs != this) {
          data = rhs.data;
          acquire();
        }
        return *this;
      }

      const buffer& buffer::operator= (buffer&& rhs) {
        if (&rhs != this) {
          data = std::move(rhs.data);
          rhs.detach();
        }
        return *this;
      }

      void buffer::acquire () {
        if (data) {
          LogTrace << "Acquire buffer";
          mmal_buffer_header_acquire(data);
        }
      }

      void buffer::release () {
        if (data) {
          LogTrace << "Release buffer";
          mmal_buffer_header_release(data);
          data = nullptr;
        }
      }

      void buffer::detach () {
        if (data) {
          LogTrace << "Detach buffer";
          data = nullptr;
        }
      }

      void buffer::reset () {
        check_null_ptr(data);
        mmal_buffer_header_reset(data);
      }

      void buffer::lock () {
        check_null_ptr(data);
        mmal_buffer_header_mem_lock(data);
      }

      void buffer::unlock () {
        check_null_ptr(data);
        mmal_buffer_header_mem_unlock(data);
      }

      bool buffer::is_valid () const {
        return data != nullptr;
      }

      uint32_t buffer::get_command () const {
        check_null_ptr(data);
        return data->cmd;
      }

      const uint8_t* buffer::get_data () const {
        check_null_ptr(data);
        return data->data + data->offset;
      }

      uint8_t* buffer::get_data () {
        check_null_ptr(data);
        return data->data + data->offset;
      }

      uint32_t buffer::get_length () const {
        check_null_ptr(data);
        return data->length;
      }

      auto buffer::begin () -> iterator {
          return get_data();
      }

      auto buffer::end () -> iterator {
          return get_data() + get_length();
      }

      auto buffer::begin () const -> const_iterator {
          return get_data();
      }

      auto buffer::end () const -> const_iterator {
          return get_data() + get_length();
      }

      uint32_t buffer::get_flags () const {
        check_null_ptr(data);
        return data->flags;
      }

      int64_t buffer::get_presentation_timestamp () const {
        check_null_ptr(data);
        return data->pts;
      }

      int64_t buffer::get_decode_timestamp () const {
        check_null_ptr(data);
        return data->dts;
      }

      // --------------------------------------------------------------------------
      void port::size_num::operator |= (const port::size_num& rhs) {
        size = std::max(size, rhs.size);
        num = std::max(num, rhs.num);
      }

      // --------------------------------------------------------------------------
      port::port (MMAL_PORT_T* d)
        : data(d)
      {}

      port::~port ()
      {}

      bool port::is_valid () const {
        return data != nullptr;
      }

      bool port::is_enabled () const {
        return is_valid() && data->is_enabled;
      }

      bool port::enable (MMAL_PORT_BH_CB_T cb) {
        check_null_ptr(data);
        if (data->is_enabled) {
          return false;
        }
        LogTrace << "enable port";
        return check_mmal_status(mmal_port_enable(data, cb));
      }

      bool port::disable () {
        check_null_ptr(data);
        if (data->is_enabled) {
          LogTrace << "disable port";
          return check_mmal_status(mmal_port_disable(data));
        }
        return true;
      }

      void port::detach () {
        data = nullptr;
      }

      bool port::flush () {
        check_null_ptr(data);
        return mmal_port_flush(data) == MMAL_SUCCESS;
      }

      bool port::is_pass_through () const {
        check_null_ptr(data);
        return data->capabilities & MMAL_PORT_CAPABILITY_PASSTHROUGH;
      }

      bool port::wants_allocate_buffer_payloads () const {
        check_null_ptr(data);
        return data->capabilities & MMAL_PORT_CAPABILITY_ALLOCATION;
      }

      bool port::support_format_change_events () const {
        check_null_ptr(data);
        return data->capabilities & MMAL_PORT_CAPABILITY_SUPPORTS_EVENT_FORMAT_CHANGE;
      }

      bool port::set (const MMAL_PARAMETER_HEADER_T& param) {
        check_null_ptr(data);
        return check_mmal_status(mmal_port_parameter_set(data, &param));
      }

      bool port::get (MMAL_PARAMETER_HEADER_T& param) const {
        check_null_ptr(data);
        return check_mmal_status(mmal_port_parameter_get(data, &param));
      }

      bool port::set_bool (uint32_t id, bool v) {
        MMAL_PARAMETER_BOOLEAN_T t = {{id, sizeof(MMAL_PARAMETER_BOOLEAN_T)}, v ? MMAL_TRUE : MMAL_FALSE};
        return set(t.hdr);
      }

      bool port::get_bool (uint32_t id) const {
        MMAL_PARAMETER_BOOLEAN_T t = {{id, sizeof(MMAL_PARAMETER_BOOLEAN_T)}};
        get(t.hdr);
        return t.enable != MMAL_FALSE;
      }

      bool port::set_int32 (uint32_t id, int32_t v) {
        return set<MMAL_PARAMETER_INT32_T>(id, v);
      }

      int32_t port::get_int32 (uint32_t id) const {
        return get<MMAL_PARAMETER_INT32_T>(id);
      }

      bool port::set_uint32 (uint32_t id, uint32_t v) {
        return set<MMAL_PARAMETER_UINT32_T>(id, v);
      }

      uint32_t port::get_uint32 (uint32_t id) const {
        return get<MMAL_PARAMETER_UINT32_T>(id);
      }

      bool port::set_float (uint32_t id, float v, int den) {
        return set<MMAL_PARAMETER_RATIONAL_T>(id, {static_cast<int32_t>(v * den), den});
      }

      bool port::set_float (uint32_t id, float v) {
        return set_float(id, v, 65536);
      }

      float port::get_float (uint32_t id) const {
        const auto v = get<MMAL_PARAMETER_RATIONAL_T>(id);
        return (float)v.num / (float)v.den;
      }

      bool port::send_buffer (buffer& b) {
        check_null_ptr(data);
        if (check_mmal_status(mmal_port_send_buffer(data, b.data))) {
          b.detach();
          return true;
        }
        return false;
      }

      const char* port::get_name () const {
        check_null_ptr(data);
        return data->name;
      }

      bool port::get_type () const {
        check_null_ptr(data);
        return data->type;
      }

      uint32_t port::get_bitrate () const {
        check_null_ptr(data);
        return data->format->bitrate;
      }

      port::size_num port::get_min_buffer_size () const {
        check_null_ptr(data);
        return {data->buffer_num_min, data->buffer_size_min};
      }

      port::size_num port::get_recommended_buffer_size () const {
        check_null_ptr(data);
        return {data->buffer_num_recommended, data->buffer_size_recommended};
      }

      port::size_num port::get_buffer_size () const {
        check_null_ptr(data);
        return {data->buffer_num, data->buffer_size};
      }

      void port::set_buffer_size (size_num sz) {
        check_null_ptr(data);
        data->buffer_num = sz.num;
        data->buffer_size = sz.size;
      }

      MMAL_ES_SPECIFIC_FORMAT_T port::get_specific_format () const {
        check_null_ptr(data);
        check_null_ptr(data->format);
        return *(data->format->es);
      }

      four_cc port::get_encoding () const {
        check_null_ptr(data);
        return data->format->encoding;
      }

      void port::set_specific_format (const MMAL_ES_SPECIFIC_FORMAT_T& f) {
        check_null_ptr(data);
        *(data->format->es) = f;
      }

      void port::set_encoding (four_cc f) {
        check_null_ptr(data);
        check_null_ptr(data->format);
        data->format->encoding = f.type.uint32;
        if (MMAL_ENCODING_OPAQUE == data->format->encoding) {
          data->format->encoding_variant = MMAL_ENCODING_I420;
        }
      }

      bool port::commit_format_change () {
        LogTrace << "port format commit";
        check_null_ptr(data);
        return check_mmal_status(mmal_port_format_commit(data));
      }

      void port::copy_format_from (const port& rhs) {
        check_null_ptr(data);
        mmal_format_copy(data->format, rhs.data->format);
      }

      void port::copy_full_format_from (const port& rhs) {
        check_null_ptr(data);
        mmal_format_full_copy(data->format, rhs.data->format);
      }

      void port::set_user_data (void* ptr) {
        check_null_ptr(data);
        data->userdata = (struct MMAL_PORT_USERDATA_T*)ptr;
      }

      bool port::capture () {
        LogTrace << "port::capture()";
        return set_bool(MMAL_PARAMETER_CAPTURE, true);
      }

      std::vector<four_cc> port::get_supported_encodings () const {
        check_null_ptr(data);

        std::vector<four_cc> encodings;

        std::vector<char*> buffer;
        buffer.resize(sizeof(MMAL_PARAMETER_ENCODING_T));
        MMAL_PARAMETER_ENCODING_T* t = (MMAL_PARAMETER_ENCODING_T*)buffer.data();
        t->hdr = {MMAL_PARAMETER_SUPPORTED_ENCODINGS, buffer.size()};
        MMAL_STATUS_T stat = mmal_port_parameter_get(data, &t->hdr);
        while (stat == MMAL_ENOSPC) {
          auto sz = t->hdr.size + sizeof(MMAL_PARAMETER_HEADER_T);
          buffer.resize(sz);
          t = (MMAL_PARAMETER_ENCODING_T*)buffer.data();
          t->hdr = {MMAL_PARAMETER_SUPPORTED_ENCODINGS, buffer.size()};
          stat = mmal_port_parameter_get(data, &t->hdr);
        }
        if (MMAL_SUCCESS == stat) {
          auto count = (t->hdr.size - sizeof(MMAL_PARAMETER_HEADER_T)) / sizeof(MMAL_FOURCC_T);
          for (int i = 0; (i < count) && t->encoding[i]; ++i) {
            encodings.emplace_back(t->encoding[i]);
          }
        }

        encodings.emplace_back(MMAL_ENCODING_OPAQUE);
        return encodings;
      }

      // --------------------------------------------------------------------------
      bool port::set_crop (const MMAL_RECT_T& c) {
//        uint32_t w_up = VCOS_ALIGN_UP(c.width, 32);
//        uint32_t h_up = VCOS_ALIGN_UP(c.height, 16);
//        auto fmt = get_specific_format();
//        fmt.video.crop.x = c.x;
//        fmt.video.crop.y = c.y;
//        fmt.video.crop.width = c.width;
//        fmt.video.crop.height = c.height;
//        fmt.video.width = w_up;
//        fmt.video.height = h_up;
//        set_specific_format(fmt);
//        return MMAL_SUCCESS;

        int32_t x = VCOS_ALIGN_DOWN(c.x, 32);
        int32_t y = VCOS_ALIGN_DOWN(c.y, 16);
        int32_t w = VCOS_ALIGN_UP(c.width, 32);
        int32_t h = VCOS_ALIGN_UP(c.height, 16);
        MMAL_PARAMETER_CROP_T crop = {{MMAL_PARAMETER_CROP, sizeof(MMAL_PARAMETER_CROP_T)}, {x, y, w, h}};
        return set(crop.hdr);
      }

      MMAL_RECT_T port::get_crop () const {
//        return get_specific_format().video.crop;

        MMAL_PARAMETER_CROP_T cr = {{MMAL_PARAMETER_CROP, sizeof(MMAL_PARAMETER_CROP_T)}};
        if (get(cr.hdr) == MMAL_SUCCESS) {
          return cr.rect;
        }
        return {};
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_RESIZE_T port::get_resize () const {
        MMAL_PARAMETER_RESIZE_T resize = {{MMAL_PARAMETER_RESIZE_PARAMS, sizeof(MMAL_PARAMETER_RESIZE_T)}, MMAL_RESIZE_DUMMY, 0};
        get(resize.hdr);
        return resize;
      }

      bool port::set_resize (MMAL_PARAMETER_RESIZE_T resize) {
        resize.hdr = {MMAL_PARAMETER_RESIZE_PARAMS, sizeof(MMAL_PARAMETER_RESIZE_T)};
        return set(resize.hdr);
      }

      // --------------------------------------------------------------------------
      pool::pool ()
        : data(nullptr)
      {}

      pool::~pool () {
        destroy();
      }

      bool pool::create (port& p) {
        destroy();
        port_ = p;
        auto sz = port_.get_buffer_size();
        data = mmal_port_pool_create(port_.data, sz.num, sz.size);
        return is_valid();
      }

      void pool::destroy () {
        if (is_valid()) {
          LogTrace << "Destroy pool";
          mmal_port_pool_destroy(port_.data, data);
          detach();
        }
      }

      void pool::detach () {
        LogTrace << "Detach pool";
        data = nullptr;
        port_.detach();
      }

      bool pool::is_valid () const {
        return (data != nullptr) && port_.is_valid();
      }

      buffer pool::get_buffer () {
        check_null_ptr(data);
        return buffer(mmal_queue_get(data->queue));
      }

      buffer pool::get_buffer (VCOS_UNSIGNED timeout) {
        check_null_ptr(data);
        return buffer(mmal_queue_timedwait(data->queue, timeout));
      }

      buffer pool::get_buffer_wait () {
        check_null_ptr(data);
        return buffer(mmal_queue_wait(data->queue));
      }

      unsigned int pool::queue_length () {
        check_null_ptr(data);
        return mmal_queue_length(data->queue);
      }

      // --------------------------------------------------------------------------
      connection::connection (MMAL_CONNECTION_T* d)
        : data(d)
      {
      }

      bool connection::connect (port& from, port& to, uint32_t flags) {
        LogTrace << "connection::connection";

        // 1. find matching formats
//        MMAL_STATUS_T status = MMAL_SUCCESS;

//        copy_format_from(in);

//        four_cc in_enc = in.get_encoding();

////        std::vector<four_cc> in_encodings = in.get_supported_encodings();
//        std::vector<four_cc> out_encodings = get_supported_encodings();
//        auto i = std::find(out_encodings.begin(), out_encodings.end(), in_enc);
//        if (i != out_encodings.end()) {
//          set_encoding(*i);
//        } else {
//          in.set_encoding(MMAL_ENCODING_OPAQUE);
//          set_encoding(MMAL_ENCODING_OPAQUE);
//        }

//        auto buf_sz = get_buffer_size();
//        buf_sz |= in.get_buffer_size();
//        set_buffer_size(buf_sz);
//        in.set_buffer_size(buf_sz);

//        status = commit_format_change();
//        status = in.commit_format_change();

        release();

        if (!from.is_valid()) {
          throw std::invalid_argument(ostreamfmt("from port must be valid in " __FILE__ " at " << __LINE__));
        }
        if (!to.is_valid()) {
          throw std::invalid_argument(ostreamfmt("to port must be valid in " __FILE__ " at " << __LINE__));
        }

        if (check_mmal_status(mmal_connection_create(&data, from.data, to.data, flags))) {
          return enable();
        }
        return false;
      }

      connection::connection (connection&& rhs)
        : data(std::move(rhs.data))
      {
        rhs.detach();
      }

      connection::connection (const connection& rhs)
        : data(rhs.data)
      {
        acquire();
      }

      connection::~connection () {
        release();
      }

      void connection::operator= (connection&& rhs) {
        release();
        data = std::move(rhs.data);
        rhs.detach();
      }

      void connection::operator= (const connection& rhs) {
        release();
        data = rhs.data;
        acquire();
      }

      bool connection::is_valid () const {
        return data != nullptr;
      }

      bool connection::is_enabled () const {
        return is_valid() && data->is_enabled;
      }

      bool connection::enable () {
        if (is_valid()) {
          LogTrace << "Enable connection";
          return check_mmal_status(mmal_connection_enable(data));
        }
        return false;
      }

      bool connection::disable () {
        if (is_valid()) {
          LogTrace << "Disable connection";
          return check_mmal_status(mmal_connection_disable(data));
        }
        return false;
      }

      bool connection::acquire () {
        if (is_valid()) {
          LogTrace << "Acquire connection";
          mmal_connection_acquire(data);
          return true;
        }
        return false;
      }

      bool connection::release () {
        if (is_valid()) {
          LogTrace << "Release connection";
          if (check_mmal_status(mmal_connection_release(data))) {
            data = nullptr;
            return true;
          }
        }
        return false;
      }

      bool connection::destroy () {
        if (is_valid()) {
          LogTrace << "Destroy connection";
          if (check_mmal_status(mmal_connection_destroy(data))) {
            data = nullptr;
            return true;
          }
        }
        return false;
      }

      void connection::detach () {
        if (is_valid()) {
          LogTrace << "Detach connection";
          data = nullptr;
        }
      }

      // --------------------------------------------------------------------------
      component::component (MMAL_COMPONENT_T* d)
        : data(d)
      {}

      component::component (const char *name)
        : data(nullptr)
      {
        create(name);
      }

      component::component (const component& rhs)
        : data(rhs.data)
      {
        acquire();
      }

      component::component (component&& rhs)
        : data(std::move(rhs.data))
      {
        rhs.detach();
      }

      component::~component () {
        release();
      }

      void component::operator= (component&& rhs) {
        release();
        data = std::move(rhs.data);
        rhs.detach();
      }

      void component::operator= (const component& rhs) {
        release();
        data = rhs.data;
        acquire();
      }

      bool component::is_valid () const {
        return data != nullptr;
      }

      bool component::is_enabled () const {
        return is_valid() && data->is_enabled;
      }

      bool component::enable () {
        if (is_valid()) {
          LogTrace << "Enable component";
          return check_mmal_status(mmal_component_enable(data));
        }
        return false;
      }

      bool component::disable () {
        if (is_valid()) {
          LogTrace << "Disable component";
          return check_mmal_status(mmal_component_disable(data));
        }
        return false;
      }

      bool component::acquire () {
        if (is_valid()) {
          LogTrace << "Acquire component";
          mmal_component_acquire(data);
          return true;
        }
        return false;
      }

      bool component::release () {
        if (is_valid()) {
          LogTrace << "Release component";
          if (check_mmal_status(mmal_component_release(data))) {
            data = nullptr;
            return true;
          }
        }
        return false;
      }

      bool component::destroy () {
        if (is_valid()) {
          LogTrace << "Destroy component";
          if (check_mmal_status(mmal_component_destroy(data))) {
            data = nullptr;
            return true;
          }
        }
        return false;
      }

      void component::detach () {
        if (is_valid()) {
          LogTrace << "Detach component";
          data = nullptr;
        }
      }

      bool component::create (const char *name) {
        destroy();
        LogTrace << "Create component";
        return check_mmal_status(mmal_component_create(name, &data));
      }

      const char* component::get_name () const {
        check_null_ptr(data);
        return data->name;
      }

      uint32_t component::num_output_ports () const {
        check_null_ptr(data);
        return data->output_num;
      }

      uint32_t component::num_input_ports () const {
        check_null_ptr(data);
        return data->input_num;
      }

      uint32_t component::num_clock_ports () const {
        check_null_ptr(data);
        return data->clock_num;
      }

      port component::control_port () const {
        check_null_ptr(data);
        check_null_ptr(data->control);
        return port(data->control);
      }

      port component::input_port (int num) const {
        check_null_ptr(data);
        if (num < data->input_num) {
          check_null_ptr(data->input);
          check_null_ptr(data->input[num]);
          return port(data->input[num]);
        }
        throw std::invalid_argument(ostreamfmt("Requested input port " << num << " is out of range [0, " << data->input_num << "]"));
      }

      port component::output_port (int num) const {
        check_null_ptr(data);
        if (num < data->output_num) {
          check_null_ptr(data->output);
          check_null_ptr(data->output[num]);
          return port(data->output[num]);
        }
        throw std::invalid_argument(ostreamfmt("Requested output port " << num << " is out of range [0, " << data->output_num << "]"));
      }

      port component::clock_port (int num) const {
        check_null_ptr(data);
        if (num < data->clock_num) {
          check_null_ptr(data->clock);
          check_null_ptr(data->clock[num]);
          return port(data->clock[num]);
        }
        throw std::invalid_argument(ostreamfmt("Requested clock port " << num << " is out of range [0, " << data->clock_num << "]"));
      }

      port component::preview_port () const {
        return output_port(0);
      }

      port component::video_port () const {
        return output_port(1);
      }

      port component::still_port () const {
        return output_port(2);
      }

      // --------------------------------------------------------------------------
    } // namespace core

    // --------------------------------------------------------------------------
    bool check_mmal_status_ (MMAL_STATUS_T status, const char* info) {
      if (status == MMAL_SUCCESS) {
        return true;
      }
      switch (status) {
        case MMAL_ENOMEM:    throw std::runtime_error(ostreamfmt("Out of memory when calling " << info)); break;
        case MMAL_ENOSPC:    throw std::runtime_error(ostreamfmt("Out of resources (other than memory) when calling " << info)); break;
        case MMAL_EINVAL:    throw std::invalid_argument(ostreamfmt("Argument is invalid when calling " << info)); break;
        case MMAL_ENOSYS:    throw std::runtime_error(ostreamfmt("Function not implemented when calling " << info)); break;
        case MMAL_ENOENT:    throw std::invalid_argument(ostreamfmt("No such file or directory when calling " << info)); break;
        case MMAL_ENXIO:     throw std::invalid_argument(ostreamfmt("No such device or address when calling " << info)); break;
        case MMAL_EIO:       throw std::runtime_error(ostreamfmt("I/O error when calling " << info)); break;
        case MMAL_ESPIPE:    throw std::runtime_error(ostreamfmt("Illegal seek when calling " << info)); break;
        case MMAL_ECORRUPT:  throw std::runtime_error(ostreamfmt("Data is corrupt \attention FIXME: not POSIX when calling " << info)); break;
        case MMAL_ENOTREADY: throw std::runtime_error(ostreamfmt("Component is not ready \attention FIXME: not POSIX when calling " << info)); break;
        case MMAL_ECONFIG:   throw std::runtime_error(ostreamfmt("Component is not configured \attention FIXME: not POSIX when calling " << info)); break;
        case MMAL_EISCONN:   throw std::runtime_error(ostreamfmt("Port is already connected when calling " << info)); break;
        case MMAL_ENOTCONN:  throw std::runtime_error(ostreamfmt("Port is disconnected when calling " << info)); break;
        case MMAL_EAGAIN:    throw std::runtime_error(ostreamfmt("Resource temporarily unavailable. Try again later when calling " << info)); break;
        case MMAL_EFAULT:    throw std::runtime_error(ostreamfmt("Bad address when calling " << info)); break;
        default:             throw std::runtime_error(ostreamfmt("Unknown status error when calling " << info)); break;
      }
    }

    bool check_vcos_status_ (VCOS_STATUS_T status, const char* info) {
      if (status == VCOS_SUCCESS) {
        return true;
      }
      switch (status) {
        case VCOS_EAGAIN:   return check_mmal_status_(MMAL_EAGAIN, info); break;
        case VCOS_ENOENT:   return check_mmal_status_(MMAL_ENOENT, info); break;
        case VCOS_ENOSPC:   return check_mmal_status_(MMAL_ENOSPC, info); break;
        case VCOS_EINVAL:   return check_mmal_status_(MMAL_EINVAL, info); break;
        case VCOS_EACCESS:  throw std::invalid_argument(ostreamfmt("Invalid access when calling " << info)); break;
        case VCOS_ENOMEM:   return check_mmal_status_(MMAL_ENOMEM, info); break;
        case VCOS_ENOSYS:   return check_mmal_status_(MMAL_ENOSYS, info); break;
        case VCOS_EEXIST:   throw std::invalid_argument(ostreamfmt("Resource does not exist when calling " << info)); break;
        case VCOS_ENXIO:    return check_mmal_status_(MMAL_ENXIO, info); break;
        case VCOS_EINTR:    throw std::invalid_argument(ostreamfmt("Interrupt when calling " << info)); break;
        default:            throw std::runtime_error(ostreamfmt("Unknown status error when calling " << info)); break;
      }
    }

    // --------------------------------------------------------------------------

  } // raspi

} // gui

/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     direct access image_data
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 // --------------------------------------------------------------------------
 //
 // Common includes
 //
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/datamap.h"


namespace gui {

  namespace draw {

    basic_datamap::basic_datamap (const blob& data, const bitmap_info& bmi)
      : info(bmi)
#ifndef GUIPP_USE_XSHM
      , buffer(data)
#else
      , image(nullptr)
#endif // !GUIPP_USE_XSHM
    {
#ifdef GUIPP_USE_XSHM
      prepare(info);
      assign(data.data(), data.size());
#endif // GUIPP_USE_XSHM
    }

    basic_datamap::basic_datamap (blob&& data, bitmap_info&& bmi)
      : info(std::move(bmi))
#ifndef GUIPP_USE_XSHM
      , buffer(std::move(data))
#else
      , image(nullptr)
#endif // !GUIPP_USE_XSHM
    {
#ifdef GUIPP_USE_XSHM
      prepare(info);
      assign(data.data(), data.size());
#endif // GUIPP_USE_XSHM
    }

    basic_datamap::basic_datamap ()
#ifdef GUIPP_USE_XSHM
      : image(nullptr)
#endif // !GUIPP_USE_XSHM
    {}

    basic_datamap::basic_datamap (const basic_datamap& rhs)
      : info(rhs.info)
#ifndef GUIPP_USE_XSHM
      , buffer(rhs.buffer)
#else
      , image(nullptr)
#endif // !GUIPP_USE_XSHM
    {
#ifdef GUIPP_USE_XSHM
      prepare(info);
      assign(rhs.data(), rhs.size());
#endif // GUIPP_USE_XSHM
    }

    basic_datamap& basic_datamap::operator= (const basic_datamap& rhs) {
      if (this == &rhs) {
        return *this;
      }

#ifndef GUIPP_USE_XSHM
      info = rhs.info;
      buffer = rhs.buffer;
#else
      prepare(rhs.get_info());
      assign(rhs.data(), rhs.size());
#endif // GUIPP_USE_XSHM
      return *this;
    }

    basic_datamap::basic_datamap (basic_datamap&& rhs)
      : info(std::move(rhs.info))
#ifndef GUIPP_USE_XSHM
      , buffer(std::move(rhs.buffer))
#else
      , image(std::move(rhs.image))
      , shminfo(std::move(rhs.shminfo))
#endif // !GUIPP_USE_XSHM
    {}

    basic_datamap::~basic_datamap () {
      destroy();
    }

    void basic_datamap::destroy () {
#ifdef GUIPP_USE_XSHM
      if (is_valid()) {
        XShmDetach(core::global::get_instance(), &shminfo);
        shmdt(shminfo.shmaddr);
        shmctl(shminfo.shmid, IPC_RMID, 0);
        XDestroyImage(image);
        image = nullptr;
      }
#endif // !GUIPP_USE_XSHM
    }

    bool basic_datamap::is_valid () const {
#ifndef GUIPP_USE_XSHM
      return !buffer.empty();
#else // GUIPP_USE_XSHM
      return image != nullptr;
#endif // !GUIPP_USE_XSHM
    }

    basic_datamap::operator bool () const {
      return is_valid();
    }

    const bitmap_info& basic_datamap::get_info () const {
      return info;
    }

    bitmap_info& basic_datamap::get_info () {
      return info;
    }

    core::native_size basic_datamap::native_size () const {
      return info.size();
    }

    core::size basic_datamap::scaled_size () const {
      return info.scaled_size();
    }

    byte basic_datamap::depth () const {
      return info.depth();
    }

    pixel_format_t basic_datamap::pixel_format () const {
      return info.pixel_format;
    }

    std::size_t basic_datamap::size () const {
#ifndef GUIPP_USE_XSHM
      return buffer.size();
#else // GUIPP_USE_XSHM
      return is_valid() ? image->bytes_per_line * image->height : 0;
#endif // !GUIPP_USE_XSHM
    }

    byte* basic_datamap::data () {
#ifndef GUIPP_USE_XSHM
      return buffer.data();
#else // GUIPP_USE_XSHM
      return is_valid() ? (byte*)image->data : nullptr;
#endif // !GUIPP_USE_XSHM
    }

    const byte* basic_datamap::data () const {
#ifndef GUIPP_USE_XSHM
      return buffer.data();
#else // GUIPP_USE_XSHM
      return is_valid() ? (const byte*)image->data : nullptr;
#endif // !GUIPP_USE_XSHM
    }

    void basic_datamap::clear () {
#ifndef GUIPP_USE_XSHM
      buffer.clear();
#else // GUIPP_USE_XSHM
      if (is_valid()) {
        memset(image->data, 0, size());
      }
#endif // !GUIPP_USE_XSHM
    }

    void basic_datamap::swap (basic_datamap& rhs) {
      info.swap(rhs.info);
#ifndef GUIPP_USE_XSHM
      buffer.swap(rhs.buffer);
#else // GUIPP_USE_XSHM
      std::swap(image, rhs.image);
      std::swap(shminfo, rhs.shminfo);
#endif // !GUIPP_USE_XSHM
    }

    void basic_datamap::prepare (const bitmap_info& bmi) {
      info = bmi;
#ifndef GUIPP_USE_XSHM
      buffer.resize(info.mem_size());
#else // GUIPP_USE_XSHM
      destroy();
      image = XShmCreateImage(core::global::get_instance(),
                              core::global::x11::get_visual(),
                              core::global::get_device_depth(),
                              ZPixmap, nullptr, &shminfo,
                              info.width, info.height);

      shminfo.shmid = shmget(IPC_PRIVATE, size(), IPC_CREAT | 0777);
      if (shminfo.shmid < 0) {
          logging::error() << "Fehler: shmget fehlgeschlagen\n";
          return;
      }

      shminfo.shmaddr = (char*)shmat(shminfo.shmid, nullptr, 0);
      if (shminfo.shmaddr == (char*)-1) {
          logging::error() << "Fehler: shmat fehlgeschlagen\n";
          return;
      }

      image->data = shminfo.shmaddr;
      shminfo.readOnly = False;

      if (!XShmAttach(core::global::get_instance(), &shminfo)) {
          logging::error() << "Fehler: XShmAttach fehlgeschlagen\n";
          return;
      }

#endif // !GUIPP_USE_XSHM
    }

    void basic_datamap::assign (const byte* ptr, std::size_t sz) {
#ifndef GUIPP_USE_XSHM
      if (buffer.size() < sz) {
        buffer.resize(sz);
      }
      buffer.assign(ptr, ptr + sz);
#else // GUIPP_USE_XSHM
      const size_t imsz = size();
      if (sz <= imsz) {
        memcpy(image->data, ptr, sz);
      }
#endif // !GUIPP_USE_XSHM
    }

    core::array_wrapper<byte> basic_datamap::access () {
#ifndef GUIPP_USE_XSHM
      return core::array_wrapper<byte>(data(), size());
#else // GUIPP_USE_XSHM
      if (is_valid()) {
        const size_t sz = image->bytes_per_line * image->height;
        return core::array_wrapper<byte>((byte*)image->data, sz);
      }
      return core::array_wrapper<byte>(nullptr, 0);
#endif // !GUIPP_USE_XSHM
    }

    core::native_rect checked_area (const bitmap_info& bmi, const core::native_rect& area) {
      const int32_t x = std::max<int32_t>(0, std::min<int32_t>(bmi.width, area.x()));
      const int32_t y = std::max<int32_t>(0, std::min<int32_t>(bmi.height, area.y()));
      const uint32_t w = std::min<uint32_t>(bmi.width - x, area.width());
      const uint32_t h = std::min<uint32_t>(bmi.height - y, area.height());
      return {x, y, w, h};
    }

    core::native_rect checked_area (const bitmap_info& bmi, const core::native_point& pt, const core::native_size& sz) {
      const int32_t x = std::max<int32_t>(0, std::min<int32_t>(bmi.width, pt.x()));
      const int32_t y = std::max<int32_t>(0, std::min<int32_t>(bmi.height, pt.y()));
      const uint32_t w = std::min<uint32_t>(bmi.width - x, sz.width());
      const uint32_t h = std::min<uint32_t>(bmi.height - y, sz.height());
      return {x, y, w, h};
    }

  } // namespace draw

} // namespace gui

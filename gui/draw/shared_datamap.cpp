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
#include "gui/draw/shared_datamap.h"


namespace gui {

  namespace draw {

#ifdef GUIPP_USE_XSHM
    shared_datamap::shared_datamap ()
      : image(nullptr)
      , shminfo{0}
    {}

    shared_datamap::shared_datamap (const shared_datamap& rhs)
      : info(rhs.info)
      , image(nullptr)
      , shminfo{0}
    {
      operator=(rhs);
    }

    shared_datamap::shared_datamap (shared_datamap&& rhs)
      : image(nullptr)
      , shminfo{0}
    {
      swap(rhs);
    }

    shared_datamap::shared_datamap (const core::native_size& sz)
      : image(nullptr)
      , shminfo{0}
    {
      create(sz);
    }

    shared_datamap::shared_datamap (const core::size& sz)
      : image(nullptr)
      , shminfo{0}
    {
      create(sz);
    }

    shared_datamap::shared_datamap (const image_data_type& data)
      : image(nullptr)
      , shminfo{0}
    {
      create(data);
    }

    shared_datamap& shared_datamap::operator= (const shared_datamap& rhs) {
      if (this == &rhs) {
        return *this;
      }

      return operator=(rhs.get_data());
    }

    shared_datamap& shared_datamap::operator= (const image_data_type& rhs) {
      if (!(get_info() == rhs.get_info())) {
        destroy();
        create(rhs.get_info().size());
      }
      get_data().copy_from(rhs);
      return *this;
    }

    shared_datamap::~shared_datamap () {
      destroy();
    }

    bool shared_datamap::is_valid () const {
      return image != nullptr;
    }

    shared_datamap::operator bool () const {
      return is_valid();
    }

    const bitmap_info& shared_datamap::get_info () const {
      return info;
    }

    bitmap_info& shared_datamap::get_info () {
      return info;
    }

    auto shared_datamap::get_data () const -> const image_data_type {
      return image_data_type(core::array_wrapper<byte>(const_cast<byte*>(data()), size()), info);
    }

    auto shared_datamap::get_data () -> image_data_type {
      return image_data_type(core::array_wrapper<byte>(data(), size()), info);
    }

    core::native_size shared_datamap::native_size () const {
      return info.size();
    }

    core::size shared_datamap::scaled_size () const {
      return info.scaled_size();
    }

    byte shared_datamap::depth () const {
      return info.depth();
    }

    pixel_format_t shared_datamap::pixel_format () const {
      return info.pixel_format;
    }

    std::size_t shared_datamap::size () const {
      return is_valid() ? info.mem_size() : 0;
    }

    byte* shared_datamap::data () {
      return is_valid() ? (byte*)image->data : nullptr;
    }

    const byte* shared_datamap::data () const {
      return is_valid() ? (const byte*)image->data : nullptr;
    }

    void shared_datamap::clear () {
      if (is_valid()) {
        memset(image->data, 0, size());
      }
    }

    void shared_datamap::swap (shared_datamap& rhs) {
      info.swap(rhs.info);
      std::swap(image, rhs.image);
      std::swap(shminfo, rhs.shminfo);
    }

    void shared_datamap::create (const core::native_size& sz) {
      destroy();
      image = XShmCreateImage(core::global::get_instance(),
                              core::global::x11::get_visual(),
                              core::global::get_device_depth(),
                              ZPixmap, nullptr, &shminfo,
                              sz.width(), sz.height());
      if (nullptr == image) {
        logging::error() << "XShmCreateImage for " << info << " failed!\n";
        return;
      }

      pixel_format_t fmt = px_fmt;
      switch (image->bits_per_pixel) {
        case 1:
          fmt = pixel_format_t::BW;
       break;
        case 8:
          fmt = pixel_format_t::GRAY;
        break;
        case 24:
          fmt = pixel_format_t::BGR;
        break;
        case 32:
          fmt = pixel_format_t::BGRA;
        break;
      }
      info = {sz, fmt};
      info.bytes_per_line = image->bytes_per_line;

      shminfo.shmid = shmget(IPC_PRIVATE, size(), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
      if (shminfo.shmid < 0) {
          logging::error() << "shmget of size " << size() << " failed!\n";
          return;
      }

      shminfo.shmaddr = (char*)shmat(shminfo.shmid, nullptr, 0);
      if (shminfo.shmaddr == (char*)-1) {
          logging::error() << "shmat for id " << shminfo.shmid << " failed\n";
          return;
      }

      image->data = shminfo.shmaddr;
      shminfo.readOnly = False;

      if (!XShmAttach(core::global::get_instance(), &shminfo)) {
          logging::error() << "XShmAttach for id " << shminfo.shmid << " failed\n";
          return;
      }
    }

    void shared_datamap::destroy () {
      if (is_valid()) {
        XShmDetach(core::global::get_instance(), &shminfo);
        shmdt(shminfo.shmaddr);
        shmctl(shminfo.shmid, IPC_RMID, 0);
        XDestroyImage(image);
        image = nullptr;
      }
    }

    void shared_datamap::create (const core::size& sz) {
      create(core::native_size{sz.os_width(), sz.os_height()});
    }

    void shared_datamap::create (const image_data_type& rhs) {
      create(rhs.get_info().size());
      get_data().copy_from(rhs);
    }

    void shared_datamap::copy_from (const image_data_type& rhs,
                                    const core::native_rect& src_rect,
                                    const core::native_point& dest_pt) {
      bitmap_info bmi = rhs.get_info();

      auto src = checked_area(bmi, src_rect);
      auto dest = checked_area(get_info(), dest_pt, bmi.size());

      if ((dest.width() < 1) || (dest.height() < 1)) {
        return;
      }

      convert::copy::sub<px_fmt>(get_data(), rhs, src.position(), dest);
    }
#endif //GUIPP_USE_XSHM
  } // namespace draw

} // namespace gui

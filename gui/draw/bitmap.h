/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: bitmap definition
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    class bitmap {
    public:
      bitmap ();
      explicit bitmap (os::bitmap id);

      ~bitmap ();

      bool is_valid () const;

      operator bool () const;

      os::bitmap get_id () const;

      void create (const std::vector<char>& data, int w, int h, int bpl, BPP bpp);
      void clear ();
      void invert ();

      void put_data (const std::vector<char>& data, int w, int h, int bpl, BPP bpp);
      void get_data (std::vector<char>& data, int& w, int& h, int& bpl, BPP& bpp) const;

      void put (const bitmap& rhs);

      core::size size () const;
      int depth () const;
      BPP bits_per_pixel () const;

      static int calc_bytes_per_line (int w, BPP bpp);

    protected:
      bitmap (bitmap&& rhs);
      void operator= (bitmap&&);

      void create_compatible (int w, int h);
      void create (int w, int h, BPP bpp);
      void copy_from (const bitmap&);

    private:
      void set_id (os::bitmap);

      os::bitmap id;
      bitmap (const bitmap&) = delete;
    };

    // --------------------------------------------------------------------------
    template<BPP T>
    class datamap : public bitmap {
    public:
      typedef bitmap super;

      datamap ();

      datamap (const datamap& rhs);
      datamap (datamap&& rhs);

      datamap (const bitmap& rhs);
      datamap (int w, int h);
      datamap (int w, int h, int bpl, const std::vector<char>& data);
      datamap (const core::size& sz);

      void operator= (const datamap& rhs);
      void operator= (const bitmap& rhs);
      void operator= (datamap&& rhs);

      void create (int w, int h);
      void create (const core::size& sz);
    };

    // --------------------------------------------------------------------------
    typedef datamap<BPP::BW> maskmap;
    typedef datamap<BPP::GRAY> graymap;
    typedef datamap<BPP::RGB> rgbmap;
    typedef datamap<BPP::RGBA> rgbamap;

    // --------------------------------------------------------------------------
    class memmap : public bitmap {
    public:
      typedef bitmap super;

      memmap ();

      memmap (const memmap& rhs);
      memmap (memmap&& rhs);

      memmap (const bitmap& rhs);
      memmap (int w, int h);
      memmap (const core::size& sz);

      void operator= (const memmap& rhs);
      void operator= (const bitmap& rhs);
      void operator= (memmap&& rhs);

      operator os::drawable() const;

      void create (int w, int h);
      void create (const core::size& sz);
    };

    // --------------------------------------------------------------------------
    class masked_bitmap {
    public:
      typedef memmap super;

      masked_bitmap ();

      masked_bitmap (const masked_bitmap&);
      masked_bitmap (masked_bitmap&&);

      masked_bitmap (const memmap& bmp);
      masked_bitmap (memmap&& bmp);

      masked_bitmap (const memmap& bmp, const maskmap& mask);
      masked_bitmap (memmap&& bmp, maskmap&& mask);

      void operator= (const masked_bitmap& rhs);
      void operator= (masked_bitmap&&);

      void operator= (const memmap& bmp);
      void operator= (memmap&& bmp);

      core::size size () const;

      bool is_valid () const;

      operator bool () const;

      memmap image;
      maskmap mask;
    };

    // --------------------------------------------------------------------------
    // inlines
    inline bitmap::bitmap ()
      : id(0)
    {}

    inline bitmap::bitmap (os::bitmap id)
      : id(id)
    {}

    inline bitmap::~bitmap () {
      clear();
    }

    inline bitmap::bitmap (bitmap&& rhs)
      : id(0) {
      operator= (std::move(rhs));
    }

    inline void bitmap::set_id (os::bitmap rhs) {
      id = rhs;
    }

    inline bool bitmap::is_valid () const {
      return id != 0;
    }

    inline bitmap::operator bool () const {
      return is_valid();
    }

    inline os::bitmap bitmap::get_id () const {
      return id;
    }

    // --------------------------------------------------------------------------
    template<BPP T>
    inline datamap<T>::datamap ()
    {}

    template<BPP T>
    inline datamap<T>::datamap (const datamap& rhs) {
      operator= (rhs);
    }

    template<BPP T>
    inline datamap<T>::datamap (const bitmap& rhs) {
      operator= (rhs);
    }

    template<BPP T>
    inline datamap<T>::datamap (datamap&& rhs)
      : super(std::move(rhs))
    {}

    template<BPP T>
    inline datamap<T>::datamap (int w, int h) {
      create(w, h);
    }

    template<BPP T>
    inline datamap<T>::datamap (int w, int h, int bpl, const std::vector<char>& data) {
      super::create(data, w, h, bpl, T);
    }

    template<BPP T>
    inline datamap<T>::datamap (const core::size& sz) {
      create(sz);
    }

    template<BPP T>
    inline void datamap<T>::operator= (const datamap& rhs) {
      operator= (static_cast<const bitmap&>(rhs));
    }

    template<BPP T>
    inline void datamap<T>::operator= (const bitmap& rhs) {
      if (this != &rhs) {
        if (rhs) {
          create(rhs.size());
          put(rhs);
        } else {
          clear();
        }
      }
    }

    template<BPP T>
    inline void datamap<T>::operator= (datamap&& rhs) {
      super::operator= (std::move(rhs));
    }

    template<BPP T>
    inline void datamap<T>::create (int w, int h) {
      super::create(w, h, T);
    }

    template<BPP T>
    inline void datamap<T>::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    // --------------------------------------------------------------------------
    inline memmap::memmap ()
    {}

    inline memmap::memmap (const memmap& rhs) {
      operator= (rhs);
    }

    inline memmap::memmap (const bitmap& rhs) {
      operator= (rhs);
    }

    inline memmap::memmap (memmap&& rhs)
      : super(std::move(rhs))
    {}

    inline memmap::memmap (int w, int h) {
      create(w, h);
    }

    inline memmap::memmap (const core::size& sz) {
      create(sz);
    }

    inline void memmap::operator= (const memmap& rhs) {
      operator= (static_cast<const bitmap&>(rhs));
    }

    inline void memmap::operator= (const bitmap& rhs) {
      if (this != &rhs) {
        if (rhs) {
          create(rhs.size());
          put(rhs);
        } else {
          clear();
        }
      }
    }

    inline void memmap::operator= (memmap&& rhs) {
      super::operator= (std::move(rhs));
    }

    inline memmap::operator os::drawable() const {
      return get_id();
    }

    inline void memmap::create (int w, int h) {
      create_compatible(w, h);
    }

    inline void memmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    // --------------------------------------------------------------------------
    inline masked_bitmap::masked_bitmap ()
    {}

    inline masked_bitmap::masked_bitmap (const masked_bitmap& rhs)
      : image(rhs.image)
      , mask(rhs.mask)
    {}

    inline masked_bitmap::masked_bitmap (masked_bitmap&& rhs) {
      operator= (std::move(rhs));
    }

    inline masked_bitmap::masked_bitmap (const memmap& rhs) {
      operator= (rhs);
    }

    inline masked_bitmap::masked_bitmap (memmap&& rhs) {
      operator= (std::move(rhs));
    }

    inline masked_bitmap::masked_bitmap (const memmap& image, const maskmap& mask)
      : image(image)
      , mask(mask)
    {}

    inline masked_bitmap::masked_bitmap (memmap&& img, maskmap&& msk) {
      std::swap(image, img);
      std::swap(mask, msk);
    }

    inline core::size masked_bitmap::size () const {
      return image.size();
    }

    inline bool masked_bitmap::is_valid () const {
      return image.is_valid();
    }

    inline masked_bitmap::operator bool () const {
      return image.is_valid();
    }

    // --------------------------------------------------------------------------
  }

}

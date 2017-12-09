/**
* @copyright (c) 2015-2017 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    standard lib
*
* Customer   -
*
* @brief     C++ API:logger
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/logger.h>
#include <gui_version.h>
#include <gui++-export.h>

#if defined(BASEPP_BUILT_AS_STATIC)
//# if !defined(GUIPP_BUILT_AS_STATIC)
#pragma message ("Export basepp::get_logging_core in gui++ DLL")
DEFINE_LOGGING_CORE(GUIPP_EXPORT)
//# endif
#endif

GUIPP_EXPORT std::string guipp_version = gui_PRODUCT_VERSION;

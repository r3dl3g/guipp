/**
* @copyright (c) 2015-2021 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    gui++ lib
*
* @brief     Version info and logger core.
*
* @license   MIT license. See accompanying file LICENSE.
*/

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <logging/core.h>
#include <gui_version.h>
#include "gui++-export.h"

#if defined(LOGGING_BUILT_AS_STATIC_LIB)
DEFINE_LOGGING_CORE(GUIPP_EXPORT)
#endif

GUIPP_EXPORT std::string guipp_version = GUIPP_PRODUCT_VERSION;

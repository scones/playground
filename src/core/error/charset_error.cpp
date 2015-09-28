/*
 * charset_error.cpp
 *
 *  Created on: 21.09.2014
 *      Author: scn
 */

#include "core/error/charset_error.h"


#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "core/error/fterrorstruct.h"

namespace core {

  namespace error {

    charset_error::charset_error(int error_code) : error() {
      char error_buffer[BUFFER_SIZE];
      std::snprintf(error_buffer, BUFFER_SIZE, "freetype error (0x%02x): %s", FT_Errors[error_code].code, FT_Errors[error_code].message);
      log_error(error_buffer);
    }


    charset_error::charset_error(std::string const& prefix, int error_code) {
      char error_buffer[BUFFER_SIZE];
      std::snprintf(error_buffer, BUFFER_SIZE, "%s: freetype error (0x%02x): %s", prefix.c_str(), FT_Errors[error_code].code, FT_Errors[error_code].message);
      log_error(error_buffer);
    }

  }

}


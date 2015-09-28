/*
 * charset_error.h
 *
 *  Created on: 21.09.2014
 *      Author: scn
 */

#ifndef CHARSET_ERROR_H_
#define CHARSET_ERROR_H_


#include "core/error/error.h"

#include <stdexcept>
#include <cstdio>
#include <string>


namespace core {

  namespace error {

    class charset_error : public core::error::error {
      public:


      charset_error(int error_code);
      charset_error(std::string const& prefix, int error_code);


      private:


      static const unsigned int BUFFER_SIZE = 512;
    };

  }

}

#endif /* CHARSET_ERROR_H_ */


/*
 * shader_error.h
 *
 *  Created on: 07.08.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_ERROR_SHADER_ERROR_H_
#define INCLUDE_CORE_ERROR_SHADER_ERROR_H_


#include "core/error/error.h"


namespace core {

  namespace error {

    class shader_error : public core::error::error {
      public:

      shader_error(std::string const& what) : core::error::error(what) {}
    };

  }

}


#endif /* INCLUDE_CORE_ERROR_SHADER_ERROR_H_ */


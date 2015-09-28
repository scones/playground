/*
 * shader_compilation_rrror.h
 *
 *  Created on: 13.08.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_ERROR_SHADER_COMPILATION_ERROR_H_
#define INCLUDE_CORE_ERROR_SHADER_COMPILATION_ERROR_H_


#include "core/error/error.h"


namespace core {

  namespace error {

    class shader_compilation_error : public error {

      public:


      shader_compilation_error(std::string const& what, std::string const& source);
    };
  }
}


#endif /* INCLUDE_CORE_ERROR_SHADER_COMPILATION_ERROR_H_ */


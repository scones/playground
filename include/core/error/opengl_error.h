/*
 * opengl_error.h
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_ERROR_OPENGL_ERROR_H_
#define INCLUDE_CORE_ERROR_OPENGL_ERROR_H_


#include "core/error/error.h"


#include <cinttypes>
#include <unordered_map>
#include <string>


namespace core {

  namespace error {

    class opengl_error : public error {

      public:

      opengl_error(std::string const& what) : core::error::error(what) {}

      static void callback(std::uint32_t source, std::uint32_t type, std::uint32_t id, std::uint32_t severity, std::int32_t length, const char* message, const void* userParam);


      protected:


      static const std::unordered_map<std::uint32_t, std::string> m_source_map;
      static const std::unordered_map<std::uint32_t, std::string> m_type_map;
      static const std::unordered_map<std::uint32_t, std::string> m_severity_map;
    };

  }

}


#endif /* INCLUDE_CORE_ERROR_OPENGL_ERROR_H_ */


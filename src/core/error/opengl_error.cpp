/*
 * opengl_error.cpp
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */


#include "core/error/opengl_error.h"


#include <gl/glew.h>
#include <iostream>


namespace core {

  namespace error {

    const std::unordered_map<std::uint32_t, std::string> opengl_error::m_source_map({
      {GL_DEBUG_SOURCE_API, "OpenGL"},
      {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System"},
      {GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler"},
      {GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party"},
      {GL_DEBUG_SOURCE_APPLICATION, "Application"},
      {GL_DEBUG_SOURCE_OTHER, "Other Source"}
    });
    const std::unordered_map<std::uint32_t, std::string> opengl_error::m_type_map({
      {GL_DEBUG_TYPE_ERROR, "error"},
      {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "deprecated behaviour"},
      {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "undefined behaviour"},
      {GL_DEBUG_TYPE_PORTABILITY, "portability"},
      {GL_DEBUG_TYPE_PERFORMANCE, "performance"},
      {GL_DEBUG_TYPE_OTHER, "other type"},
      {GL_DEBUG_TYPE_MARKER, "marker"},
      {GL_DEBUG_TYPE_PUSH_GROUP, "push group"},
      {GL_DEBUG_TYPE_POP_GROUP, "pop group"}
    });
    const std::unordered_map<std::uint32_t, std::string> opengl_error::m_severity_map({
      {GL_DEBUG_SEVERITY_HIGH, "high"},
      {GL_DEBUG_SEVERITY_MEDIUM, "medium"},
      {GL_DEBUG_SEVERITY_LOW, "low"},
      {GL_DEBUG_SEVERITY_NOTIFICATION, "notification"}
    });

    void opengl_error::callback(std::uint32_t source_id, std::uint32_t type_id, std::uint32_t id, std::uint32_t severity_id, std::int32_t length, const char* message, const void* userParam) {
      static const std::uint32_t BUFFER_SIZE = 1024;
      char buffer[BUFFER_SIZE];
      std::string source = m_source_map.at(source_id),
                  type = m_type_map.at(type_id),
                  severity = m_severity_map.at(severity_id);

      std::snprintf(buffer, BUFFER_SIZE, "%s: %s(%s) %d: %s", source.c_str(), type.c_str(), severity.c_str(), id, message);
      if (GL_DEBUG_SOURCE_SHADER_COMPILER != source_id && type_id != GL_DEBUG_TYPE_PERFORMANCE && type_id != GL_DEBUG_TYPE_MARKER && type_id != GL_DEBUG_TYPE_PUSH_GROUP && type_id != GL_DEBUG_TYPE_POP_GROUP) {
        throw core::error::opengl_error(buffer);
      } else {
        std::cerr << buffer << std::endl;
      }
    }

  }

}


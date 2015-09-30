/*
 * shader.cpp
 *
 *  Created on: 07.08.2015
 *      Author: scones
 */


#include <core/shader.h>
#include "core/error/shader_error.h"
#include "core/error/shader_compilation_error.h"
#include "core/file.h"


#include <gl/glew.h>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <ostream>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>


namespace core {


  char shader::buffer[BUFFER_SIZE];


  void shader::init() {
    // create program
    m_program_name = glCreateProgram();

    // create shaders
    std::vector<std::uint32_t> shader_names;
    std::uint32_t type_bits(0);
    for (auto const& definition_pair : m_shader_list) {
      shader_names.push_back(compile_shader(definition_pair.first, definition_pair.second));
      type_bits |= bits_for_shader_type(definition_pair.first);
      glAttachShader(m_program_name, shader_names.back());
    }

    // apply transform feedback varyings if requested
    if (0 < m_varyings.size()) {
      char const **buffer = new char const *[m_varyings.size()];
      for(std::uint32_t i(0); i < m_varyings.size(); ++i) {
        buffer[i] = m_varyings[i].c_str();
      }
      glTransformFeedbackVaryings(m_program_name, m_varyings.size(), buffer, m_transform_feedback_buffer_mode);
      delete[] buffer;
    }

    // link shaders
    glLinkProgram(m_program_name);
    linkage_control();

    // clean up
    for (auto const& shader_name : shader_names) {
      glDetachShader(m_program_name, shader_name);
      glDeleteShader(shader_name);
    }

    // bind all uniforms
    for (auto const& uniform_name : m_uniform_names)
      m_mapped_uniform_locations[uniform_name] = glGetUniformLocation(m_program_name, uniform_name.c_str());
  }


  void shader::shutdown() {
    m_mapped_uniform_locations.clear();

    if (0 != m_program_name) {
      glDeleteProgram(m_program_name);
      m_program_name = 0;
    }
  }


  std::int32_t shader::get_uniform_location(std::string const& uniform_name) {
    try {
      return m_mapped_uniform_locations.at(uniform_name);
    } catch (std::out_of_range const& e) {
      throw new core::error::error("unknown uniform: " + uniform_name);
    }
  }


  std::uint32_t shader::compile_shader(std::uint32_t type, std::string const& shader_file_name) {
    const static std::uint32_t BUFFER_SIZE = 1024;

    // load shader content
    std::string shader_content = file::content(shader_file_path_name(type, shader_file_name));
    if (0 == shader_content.size()) {
      char buffer[BUFFER_SIZE];
      std::snprintf(buffer, BUFFER_SIZE, "file is empty '%s'", const_cast<char*>(shader_file_path_name(type, shader_file_name).c_str()));
      throw core::error::shader_error((char*)buffer);
    }

    // grab a name
    auto name = glCreateShader(type);
    if (!name)
      throw core::error::shader_error("glCreateShader failed");

    // preprocess the source
    auto precompiled_shader_content = precompile_shader(shader_content);
//    auto precompiled_shader_content = shader_content;
    // load the shader source and compile
    GLchar const* const c_shader_content = precompiled_shader_content.c_str();
    glShaderSource(name, 1, &c_shader_content, nullptr);
    glCompileShader(name);

    // check it
    compilation_control(name, precompiled_shader_content);

    return name;
  }


  void shader::compilation_control(std::uint32_t shader_name, std::string const& shader_content) {
    std::int32_t log_length = 0;
    std::int32_t compiled_successfully = GL_TRUE;
    glGetShaderiv(shader_name, GL_INFO_LOG_LENGTH, &log_length);
    glGetShaderiv(shader_name, GL_COMPILE_STATUS, &compiled_successfully);
    if (GL_FALSE == compiled_successfully || 0 != log_length) {
      // get specific error
      std::string error_text;
      if (0 != log_length) {
        log_length = std::min(log_length, (std::int32_t)BUFFER_SIZE);
        std::memset(buffer, 0, BUFFER_SIZE);
        glGetShaderInfoLog(shader_name, log_length, 0, buffer);
        error_text += buffer;
      } else {
        error_text += "unknown compiling error";
      }
      glDeleteShader(shader_name);
      throw core::error::shader_compilation_error(error_text, shader_content);
    }
  }


  // basically same as the compilation variant, but with subtle differences (names and stuff)
  void shader::linkage_control() {
    std::int32_t log_length = 0;
    std::int32_t compiled_successfully = GL_TRUE;
    glGetProgramiv(m_program_name, GL_INFO_LOG_LENGTH, &log_length);
    glGetProgramiv(m_program_name, GL_LINK_STATUS, &compiled_successfully);
    if (GL_FALSE == compiled_successfully || 0 != log_length) {
      // get specific error
      std::string error_text;
      if (0 != log_length) {
        log_length = std::min(log_length, (std::int32_t)BUFFER_SIZE);
        std::memset(buffer, 0, BUFFER_SIZE);
        glGetProgramInfoLog(m_program_name, log_length, 0, buffer);
        error_text = buffer;
      } else {
        error_text = "unknown linking error";
      }
      glDeleteShader(m_program_name);
      throw core::error::shader_error(error_text);
    }
  }


  std::string shader::precompile_shader(std::string const& shader_content) {
    std::ostringstream output_stream;
    output_stream
      << "#version " << m_version << " core\n\n"
      << "precision highp float;\n"
      << "precision highp int;\n"
      << "#pragma optionNV(fastmath off)\n"
      << "#pragma optionNV(fastprecision off)\n";

    return precompile_shader_source(output_stream, shader_content);
  }


  std::string shader::precompile_shader_source(std::ostringstream& output_stream, std::string const& source) {
    std::istringstream content_stream(source);
    std::string content_line;
    std::uint64_t pos;            // npos got upgraded to 64bit oO
    std::uint32_t line_count(0);
    while (std::getline(content_stream, content_line, '\n')) {
      if (std::string::npos != (pos = content_line.find("#include"))) {
        pos += 8;
        if (' ' == content_line[pos] && '<' == content_line[pos + 1]) {
          auto end_pos = content_line.find('>', pos + 2);
          if (std::string::npos != end_pos) {
            std::string include_file_name = content_line.substr(pos + 2, end_pos - (pos + 2));
            auto bad_pos = include_file_name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_/");
            if (std::string::npos != bad_pos) {
              std::snprintf(buffer, BUFFER_SIZE, "line %u: bad include file name '%s'", line_count, include_file_name.c_str());
              throw core::error::shader_compilation_error(buffer, source);
            }

            std::string inclusion_content = core::file::content(std::string("data/shader/include/") + include_file_name);
            precompile_shader_source(output_stream, inclusion_content);
          } else {
            std::snprintf(buffer, BUFFER_SIZE, "bad include syntax[01]:%d: %s\n", line_count, content_line.c_str());
            throw core::error::shader_compilation_error(buffer, source);
          }
        } else {
          std::snprintf(buffer, BUFFER_SIZE, "bad include syntax[02]:%d: %s\n", line_count, content_line.c_str());
          throw core::error::shader_compilation_error(buffer, source);
        }
      } else {
        output_stream << content_line << "\n";
      }
      ++line_count;
    }
    return output_stream.str();
  }


  std::uint32_t shader::bits_for_shader_type(std::uint32_t shader_type) {
    static const std::unordered_map<std::uint32_t, std::uint32_t> s_bit_map({
      {GL_VERTEX_SHADER,           GL_VERTEX_SHADER_BIT},
      {GL_TESS_CONTROL_SHADER,     GL_TESS_CONTROL_SHADER_BIT},
      {GL_TESS_EVALUATION_SHADER,  GL_TESS_EVALUATION_SHADER_BIT},
      {GL_GEOMETRY_SHADER,         GL_GEOMETRY_SHADER_BIT},
      {GL_FRAGMENT_SHADER,         GL_FRAGMENT_SHADER_BIT},
      {GL_COMPUTE_SHADER,          GL_COMPUTE_SHADER_BIT}
    });

    return s_bit_map.at(shader_type);
  }


  std::string shader::shader_file_path_name(std::uint32_t type, std::string const& shader_name) {
    static const std::unordered_map<std::uint32_t, std::string> s_suffix_map({
      {GL_VERTEX_SHADER, ".v"},
      {GL_TESS_CONTROL_SHADER, ".tc"},
      {GL_TESS_EVALUATION_SHADER, ".te"},
      {GL_GEOMETRY_SHADER, ".g"},
      {GL_FRAGMENT_SHADER, ".f"},
      {GL_COMPUTE_SHADER, ".c"}
    });

    return (std::string("data/shader/") + shader_name + s_suffix_map.at((unsigned int)type));
  }


  shader::activation_object shader::activate() {
    return shader::activation_object(m_program_name);
  }


  shader::activation_object::activation_object(std::uint32_t name)
    :
      m_name(name)
  {
    glUseProgram(m_name);
  }


  shader::activation_object::activation_object(activation_object&& right) {
    m_name = right.m_name;
    right.m_name = 0;
  }


  shader::activation_object::~activation_object() {
    if (m_name) {
      glUseProgram(0);
      m_name = 0;
    }
  }
}


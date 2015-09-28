/*
 * shader_compilation_error.cpp
 *
 *  Created on: 13.08.2015
 *      Author: scones
 */


#include "core/error/shader_compilation_error.h"


#include <ostream>
#include <sstream>


namespace core {

  namespace error {

    shader_compilation_error::shader_compilation_error(std::string const& what, std::string const& source) {
      static const std::uint32_t BUFFER_SIZE = 512;
      std::ostringstream prefixed_lines;
      std::istringstream source_stream(source);
      std::string source_line;

      char buffer[BUFFER_SIZE];
      std::uint32_t line_number(1);
      while (std::getline(source_stream, source_line, '\n')) {
        std::snprintf(buffer, BUFFER_SIZE, "%03d# %s", line_number, source_line.c_str());
        prefixed_lines << buffer << '\n';

        ++line_number;
      }
      error(what + prefixed_lines.str());
    }

  }

}


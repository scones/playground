/*
 * config.cpp
 *
 *  Created on: 09.08.2015
 *      Author: scones
 */


#include "config.h"
#include "core/error/error.h"

#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <cstdio>


std::string const config::s_default_config_file("etc/default.conf");
std::string const config::s_user_config_file("etc/user.conf");


config::config(std::uint32_t argc, char** argv) {
  boost::filesystem::path default_config_file_path("etc/default.conf");
  boost::filesystem::path user_config_file_path("etc/user.conf");

  boost::program_options::options_description default_options("default options");
  default_options.add_options()
    ("verbose,v", "be verbose")
    ("help,h", "display this")
    ("seed,s", boost::program_options::value<std::uint64_t>(&m_seed), "root seed for random generation")
    ("fullscreen,f", "enables full screen mode")
    ("width,w", boost::program_options::value<std::uint32_t>(&m_resolution.x), "screen width")
    ("height,h", boost::program_options::value<std::uint32_t>(&m_resolution.y), "screen height")
    ("x_position,x", boost::program_options::value<std::int32_t>(&m_position.x), "x position")
    ("y_position,x", boost::program_options::value<std::int32_t>(&m_position.y), "y position")
    ("antialiasing_level,a", boost::program_options::value<std::uint32_t>(&m_antialiasing_level), "set the antialiasing level")
    ("thread_count,t", boost::program_options::value<std::uint32_t>(&m_thread_count), "number of threads")
    ("current_test,c", boost::program_options::value<std::uint32_t>(&m_current_test), "current test number")
#ifdef DEBUG
    ("render_mode,r", boost::program_options::value<std::uint32_t>(&m_render_mode), "set the render mode")
#endif
  ;

  // thingy to store the options
  boost::program_options::variables_map vm;

  // parse options in order of importance (most important first)

  // parse command line
  boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(default_options).run(), vm);
  boost::program_options::notify(vm);

  // parse user config file options
  boost::filesystem::ifstream user_config_stream(user_config_file_path);
  if (!user_config_stream) {
    char buffer[1024];
    std::snprintf(buffer, 1024, "file not found: '%s'", (char*)user_config_file_path.c_str());
    throw core::error::error(buffer);
  }
  boost::program_options::store(boost::program_options::parse_config_file(user_config_stream, default_options), vm);
  boost::program_options::notify(vm);

  // parse default config file options
  boost::filesystem::ifstream default_config_stream(default_config_file_path);
  if (!default_config_stream) {
    char buffer[1024];
    std::snprintf(buffer, 1024, "file not found: '%s'", (char*)default_config_file_path.c_str());
    throw core::error::error(buffer);
  }
  boost::program_options::store(boost::program_options::parse_config_file(default_config_stream, default_options), vm);
  boost::program_options::notify(vm);


  std::vector<std::string> value_options({"seed", "width", "height", "antialiasing_level", "x_position", "y_position", "thread_count", "current_test"});
  for (auto const& option : value_options) {
    if (0 == vm.count(option.c_str())) {
      char buffer[1024];
      std::snprintf(buffer, 1024, "'%s' option not set", option.c_str());
      throw core::error::error(buffer);
    }
  }
#ifdef DEBUG
  std::vector<std::string> debug_value_options({"render_mode"});
  for (auto const& option : debug_value_options) {
    if (0 == vm.count(option.c_str())) {
      char buffer[1024];
      std::snprintf(buffer, 1024, "'%s' option not set", option.c_str());
      throw core::error::error(buffer);
    }
  }
#endif

  m_fullscreen = (0 != vm.count("fullscreen"));
#ifndef DEBUG
  m_render_mode = app::RENDER_MODE::FILL_MODE;
#endif
}


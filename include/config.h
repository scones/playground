/*
 * config.h
 *
 *  Created on: 09.08.2015
 *      Author: scones
 */

#ifndef INCLUDE_CONFIG_H_
#define INCLUDE_CONFIG_H_


#include "core/types.h"


#include <cinttypes>
#include <string>


class config {
  public:


  config(std::uint32_t argc, char** argv);
  ~config() {};


  std::uint64_t const get_seed() const { return m_seed; }
  core::t_vec2i32 const get_position() const { return m_position; }
  core::t_vec2ui32 const get_resolution() const { return m_resolution; }
  std::uint32_t const get_antialiasing_level() const { return m_antialiasing_level; }
  std::uint32_t const get_render_mode() const { return m_render_mode; }
  bool const get_fullscreen() const { return m_fullscreen; }
  std::uint32_t const get_thread_count() const { return m_thread_count; }
  std::uint32_t const get_current_test() const { return m_current_test; }

  void set_seed(std::uint64_t const& seed) { m_seed = seed; }
  void set_position(core::t_vec2i32 const& position) { m_position = position; }
  void set_resolution(core::t_vec2ui32 const& resolution) { m_resolution = resolution; }
  void set_antialiasing_level(std::uint32_t antialiasing_level) { m_antialiasing_level = antialiasing_level; }
  void set_render_mode(std::uint32_t render_mode) { m_render_mode = render_mode; }
  void set_fullscreen(bool fullscreen) { m_fullscreen = fullscreen; }
  void set_thread_count(std::uint32_t thread_count) { m_thread_count = thread_count; }
  void set_current_test(std::uint32_t const current_test) { m_current_test = current_test; }


  protected:


  static const std::string s_default_config_file;
  static const std::string s_user_config_file;

  std::uint64_t m_seed;
  core::t_vec2ui32 m_resolution;
  core::t_vec2i32 m_position;
  std::uint32_t m_antialiasing_level;
  std::uint32_t m_render_mode;
  bool m_fullscreen;
  std::uint32_t m_thread_count;
  std::uint32_t m_current_test;
};


#endif /* INCLUDE_CONFIG_H_ */


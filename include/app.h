/*
 * app.h
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */

#ifndef INCLUDE_APP_H_
#define INCLUDE_APP_H_


#include "core/glfw.h"
#include "core/types.h"
#include "core/window.h"

#include "config.h"
#include "test.h"


#include <cinttypes>
#include <string>


class app {

  public:

  app(std::int32_t argc, char** argv);
  ~app();


  void run();
  void update();


  static config* const& get_config() { return s_config; }


  protected:


  static config* s_config;


  void initialize_test();
  void error_callback(std::int32_t error, const char* description);
  void key_callback(GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods);
  void window_resize_callback(core::t_vec2ui32 const& resolution);
  void window_reposition_callback(core::t_vec2i32 const& position);
  void mouse_button_callback(std::int32_t button, std::int32_t action, std::int32_t modifiers);
  void cursor_position_callback(core::t_vec2d const& position);
  void scroll_callback(core::t_vec2d const& offset);


  static void global_error_callback(std::int32_t error, const char* description);
  static void global_key_callback(GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods);
  static void global_window_resize_callback(GLFWwindow* pwindow, std::int32_t width, std::int32_t height);
  static void global_window_reposition_callback(GLFWwindow* pwindow, std::int32_t xpos, std::int32_t ypos);
  static void global_mouse_button_callback(GLFWwindow* pwindow, std::int32_t button, std::int32_t action, std::int32_t modifiers);
  static void global_cursor_position_callback(GLFWwindow* pwindow, double xpos, double ypos);
  static void global_scroll_callback(GLFWwindow* pwindow, double xoffset, double yoffset);


  core::glfw m_glfw;
  test* m_pcurrent_test;
  bool m_active;
  std::string m_current_test_name;
};


#endif /* INCLUDE_APP_H_ */


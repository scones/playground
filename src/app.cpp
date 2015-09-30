/*
 * app.cpp
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */


#include "app.h"
#include "core/window.h"
#include "core/error/error.h"

#include "test/_001_dsa_buffer.h"
#include "test/_002_dsa_vao.h"
#include "test/_003_attrib_format.h"


#include <iostream>


app*    pplayground = nullptr;
core::window* pprimary_window = nullptr;

config* app::s_config;


namespace {
  enum TEST_TYPE {
    DSA_BUFFER = 0,
    DSA_VAO,
    ATTRIB_FORMAT,
    MAX
  };
}


app::app(std::int32_t argc, char** argv)
  :
    m_glfw(),
    m_pcurrent_test(nullptr),
    m_active(true)
{
  m_glfw.init();
  s_config = new config(argc, argv);

  initialize_test();
}


void app::initialize_test() {
  delete m_pcurrent_test;
  m_pcurrent_test = nullptr;
  switch (s_config->get_current_test()) {
    case 1:
      m_pcurrent_test = new _001_dsa_buffer();
      break;
    case 2:
      m_pcurrent_test = new _002_dsa_vao();
      break;
    case 3:
      m_pcurrent_test = new _003_attrib_format();
      break;
    default:
      throw core::error::error("unknown test");
  }

  pprimary_window->set_position(s_config->get_position());
  pprimary_window->set_resolution(s_config->get_resolution());
  pprimary_window->set_fullscreen(s_config->get_fullscreen());
  pprimary_window->set_window_hints({
    {GLFW_SAMPLES, s_config->get_antialiasing_level()},
    {GLFW_CONTEXT_VERSION_MAJOR, 4},
    {GLFW_CONTEXT_VERSION_MINOR, 5}
  });

  pprimary_window->init();
  pprimary_window->set_key_callback(&app::global_key_callback);
  pprimary_window->set_resize_callback(&app::global_window_resize_callback);
  pprimary_window->set_reposition_callback(&app::global_window_reposition_callback);
  pprimary_window->set_mouse_button_callback(&app::global_mouse_button_callback);
  pprimary_window->set_scroll_callback(&app::global_scroll_callback);
  pprimary_window->set_cursor_position_callback(&app::global_cursor_position_callback);
  pprimary_window->make_current();

  m_pcurrent_test->init();
}


void app::run() {
  do {
    glfwPollEvents();
    update();

    if (!pprimary_window)
      continue;

    m_pcurrent_test->render();

    pprimary_window->swap_buffers();
  } while ((!pprimary_window || !pprimary_window->should_close()) && m_active);
}


void app::update() {

}


/**
 * actually used callback methods
 */
void app::error_callback(std::int32_t error, const char* description) {
  char buffer[1024];
  std::snprintf(buffer, 1024, "GLFW error (%d): %s", error, description);
  throw core::error::error(buffer);
}


void app::key_callback(GLFWwindow *window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t modifiers) {
  switch (key) {
    case GLFW_KEY_ESCAPE:
      m_active = false;
      break;
    case GLFW_KEY_PAGE_UP:
      s_config->set_current_test((s_config->get_current_test() + 1) % TEST_TYPE::MAX + 1);
      initialize_test();
      break;
    case GLFW_KEY_PAGE_DOWN:
      s_config->set_current_test((s_config->get_current_test() - 1) % TEST_TYPE::MAX + 1);
      initialize_test();
      break;
  }
}


void app::window_resize_callback(core::t_vec2ui32 const& resolution) {
  s_config->set_resolution(resolution);
  if (pprimary_window) {
    pprimary_window->set_resolution(resolution);
  }
}


void app::window_reposition_callback(core::t_vec2i32 const& position) {
  s_config->set_position(position);
  pprimary_window->set_position(position);
}


void app::mouse_button_callback(std::int32_t button, std::int32_t action, std::int32_t modifiers) {
//  m_mouse_buttons[button] = GLFW_PRESS == action;
//  m_mouse_modifiers = modifiers;
}


void app::scroll_callback(core::t_vec2d const& offset) {
}


void app::cursor_position_callback(core::t_vec2d const& position) {
}


/**
 * methods to send to glfw
 */
void app::global_error_callback(std::int32_t error, const char* description) {
  pplayground->error_callback(error, description);
}


void app::global_key_callback(GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {
  pplayground->key_callback(window, key, scancode, action, mods);
}


void app::global_window_resize_callback(GLFWwindow* pwindow, std::int32_t width, std::int32_t height) {
  pplayground->window_resize_callback(core::t_vec2ui32(width, height));
}


void app::global_window_reposition_callback(GLFWwindow* pwindow, std::int32_t xpos, std::int32_t ypos) {
  pplayground->window_reposition_callback(core::t_vec2i32(xpos, ypos));
}


void app::global_mouse_button_callback(GLFWwindow* pwindow, std::int32_t button, std::int32_t action, std::int32_t modifiers) {
  pplayground->mouse_button_callback(button, action, modifiers);
}


void app::global_cursor_position_callback(GLFWwindow *pwindow, double xpos, double ypos) {
  pplayground->cursor_position_callback(core::t_vec2d(xpos, ypos));
}


void app::global_scroll_callback(GLFWwindow* pwindow, double xoffset, double yoffset) {
  pplayground->scroll_callback(core::t_vec2d(xoffset, yoffset));
}


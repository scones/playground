/*
 * window.h
 *
 *  Created on: 16.08.2013
 *      Author: scn
 */

#ifndef WINDOW_H_
#define WINDOW_H_


#include "core/types.h"


#include <string>
#include <cinttypes>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace core {

  class window {
    typedef std::unordered_map<std::int32_t, std::int32_t> t_hint_map;


    public:


    window(std::string const & title, t_hint_map = t_hint_map(), GLFWwindow* parent = nullptr, GLFWmonitor* monitor = nullptr);
    ~window() {
      shutdown();
    }
    window(window&)  = delete;
    window(window&& right) {
      this->m_handle = right.m_handle;
      this->m_parent = right.m_parent;
      this->m_monitor = right.m_monitor;
      this->m_title = right.m_title;
      m_resolution = std::move(right.m_resolution);
      m_position = std::move(right.m_position);
      this->m_fullscreen = right.m_fullscreen;
      m_hint_map = std::move(right.m_hint_map);

      right.m_handle = nullptr;
      right.m_parent = nullptr;
      right.m_monitor = nullptr;
    }


    void init();
    void shutdown();

    GLFWwindow * const get_handle() const { return m_handle; }
    void set_key_callback(GLFWkeyfun key_callback_function);
    void set_resize_callback(GLFWwindowsizefun resize_callback_function);
    void set_reposition_callback(GLFWwindowposfun reposition_callback_function);
    void set_mouse_button_callback(GLFWmousebuttonfun mouse_button_callback);
    void set_cursor_position_callback(GLFWcursorposfun cursor_position_callback);
    void set_scroll_callback(GLFWscrollfun scroll_callback);
    void make_current();
    bool should_close();
    void swap_buffers();

    float get_inverse_width() const { return 1.0f/static_cast<float>(m_resolution.x); }
    float get_inverse_height() const { return 1.0f/static_cast<float>(m_resolution.y); }
    core::t_vec2d const get_cursor_position() const;


    void set_input_mode(std::uint32_t);
    void set_resolution(core::t_vec2ui32 const& resolution) { m_resolution = resolution; }
    void set_position(core::t_vec2i32 const& position) { m_position = position; }
    void set_fullscreen(bool fullscreen);
    void set_window_hints(window::t_hint_map const& hints);

    core::t_vec2ui32 const get_resolution() const { return m_resolution; }
    core::t_vec2i32 const get_position() const { return m_position; }


    protected:


    static t_hint_map s_defaults;

    GLFWwindow*  m_handle;

    GLFWwindow*  m_parent;
    GLFWmonitor* m_monitor;
    std::string  m_title;

    core::t_vec2ui32 m_resolution;
    core::t_vec2i32  m_position;

    bool m_fullscreen;

    t_hint_map m_hint_map;
  };

}

#endif /* WINDOW_H_ */


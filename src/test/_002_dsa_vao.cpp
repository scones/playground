/*
 * _002_dsa_vao.cpp
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */


#include "test/_002_dsa_vao.h"

#include "core/types.h"


#include <iostream>
#include <cstring>


extern core::window* pprimary_window;


_002_dsa_vao::_002_dsa_vao()
  :
    m_vao(),
    m_buffer(1),
    m_shader(
      450,
      {
        {GL_VERTEX_SHADER, "blank"},
        {GL_FRAGMENT_SHADER, "blank"}
      },
      {"world_matrix"}
    ),
    m_buffer_name(0)
{
  pprimary_window = new core::window("001 dsa buffer test");
}


void _002_dsa_vao::init() {
  m_vao.init();
  m_shader.init();

  auto v = m_vao.activate();

  core::t_vec4f vertices[] = {
    {-1.0, -1.0, -1.0, 1.0},
    { 1.0, -1.0, -1.0, 1.0},
    { 0.0,  1.0, -1.0, 1.0}
  };
  m_buffer.init();
  m_buffer.data(0, sizeof(vertices), 0, GL_STATIC_DRAW);
  void* memory = m_buffer.map(0, GL_WRITE_ONLY);
  std::memcpy(memory, vertices, sizeof(vertices));
  m_buffer.unmap(0);


  glClearColor(0.2f, 0.2f, 0.23f, 0.0f);
}


void _002_dsa_vao::shutdown() {
  glDeleteBuffers(1, &m_buffer_name);

  m_shader.shutdown();
  m_vao.shutdown();

  delete pprimary_window;
  pprimary_window = nullptr;
}


void _002_dsa_vao::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto s = m_shader.activate();
  auto v = m_vao.activate();

  glEnableVertexAttribArray(0);
  m_buffer.bind(0, GL_ARRAY_BUFFER);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);
}


/*
 * _004_lua_test.cpp
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */


#include "test/_004_lua_test.h"

#include "core/types.h"


#include <iostream>
#include <cstring>


extern core::window* pprimary_window;


namespace {

  namespace BUFFER {
    enum type {
      POSITION = 0,
      MAX
    };
  }

  namespace ATTRIBUTES {
    enum type {
      POSITION = 0,
      MAX
    };
  }

  namespace BINDINGS {
    enum {
      PRIMARY = 0,
      MAX
    };
  }

}


_004_lua_test::_004_lua_test()
  :
    m_vao(),
    m_buffer(BUFFER::MAX),
    m_shader(
      450,
      {
        {GL_VERTEX_SHADER, "blank"},
        {GL_FRAGMENT_SHADER, "blank"}
      },
      {"world_matrix"}
    ),
    m_lua_state(nullptr)
{
  pprimary_window = new core::window("001 dsa buffer test");
}


void _004_lua_test::init() {
  m_shader.init();
  m_vao.init();

  // init buffer with stuff
  core::t_vec4f vertices[] = {
    {-1.0, -1.0, -1.0, 1.0},
    { 1.0, -1.0, -1.0, 1.0},
    { 0.0,  1.0, -1.0, 1.0}
  };

  m_buffer.init();
  m_buffer.data(ATTRIBUTES::POSITION, sizeof(vertices), 0, GL_STATIC_DRAW);
  void* memory = m_buffer.map(ATTRIBUTES::POSITION, GL_WRITE_ONLY);
  std::memcpy(memory, vertices, sizeof(vertices));
  m_buffer.unmap(ATTRIBUTES::POSITION);

  // init vao with attrib formats
  {
    auto v = m_vao.activate();

    // setup attribute format
    glVertexAttribBinding(ATTRIBUTES::POSITION, BINDINGS::PRIMARY);
    glVertexAttribFormat(ATTRIBUTES::POSITION, 4, GL_FLOAT, GL_FALSE, 0);

    // enable attrib array
    glEnableVertexAttribArray(ATTRIBUTES::POSITION);

    // bind buffer
    glBindVertexBuffer(BINDINGS::PRIMARY, m_buffer.get_name(BUFFER::POSITION), 0, sizeof(core::t_vec4f));

    // order between the 3 steps does not matter, but every step has to exist.
  }

  glClearColor(0.2f, 0.2f, 0.23f, 0.0f);

  m_lua_state = luaL_newstate();
  luaL_openlibs(m_lua_state);

  std::cout << "start run lua file" << std::endl;
  luaL_dofile(m_lua_state, "data/script/manipulator.lua");
  std::cout << "stop run lua file" << std::endl;
  lua_close(m_lua_state);
}


void _004_lua_test::shutdown() {
  m_shader.shutdown();
  m_vao.shutdown();

  delete pprimary_window;
  pprimary_window = nullptr;
}


void _004_lua_test::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto s = m_shader.activate();
  auto v = m_vao.activate();

  glDrawArrays(GL_TRIANGLES, 0, 3);
}


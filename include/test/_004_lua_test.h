/*
 * _004_lua_test.h
 *
 *  Created on: 01.11.2015
 *      Author: scones
 */


#ifndef INCLUDE_TEST__004_LUA_TEST_H_
#define INCLUDE_TEST__004_LUA_TEST_H_


#include "test.h"

#include "core/window.h"
#include "core/buffer.h"
#include "core/shader.h"
#include "core/vao.h"

extern "C" {
  #include <luajit-2.0/lua.hpp>
}


class _004_lua_test : public test {

  public:

  _004_lua_test();
  ~_004_lua_test() {
    shutdown();
  }


  void init();
  void shutdown();

  void render();

  protected:


  core::vao m_vao;
  core::buffer m_buffer;
  core::shader m_shader;

  lua_State *m_lua_state;
};


#endif /* INCLUDE_TEST__004_LUA_TEST_H_ */


/*
 * _005_js_test.h
 *
 *  Created on: 01.11.2015
 *      Author: scones
 */


#ifndef INCLUDE_TEST__005_JS_TEST_H_
#define INCLUDE_TEST__005_JS_TEST_H_


#include "test.h"

#include "core/window.h"
#include "core/buffer.h"
#include "core/shader.h"
#include "core/vao.h"

extern "C" {
  #include <duktape/duktape.h>
}


class _005_js_test : public test {

  public:

  _005_js_test();
  ~_005_js_test() {
    shutdown();
  }


  void init();
  void shutdown();

  void render();

  protected:


  core::vao m_vao;
  core::buffer m_buffer;
  core::shader m_shader;

  duk_context *m_duk_context;
};


#endif /* INCLUDE_TEST__005_JS_TEST_H_ */


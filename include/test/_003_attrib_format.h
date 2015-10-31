/*
 * _003_attrib_format.h
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */

#ifndef INCLUDE_TEST__003_ATTRIB_FORMAT_H_
#define INCLUDE_TEST__003_ATTRIB_FORMAT_H_


#include "test.h"

#include "core/window.h"
#include "core/buffer.h"
#include "core/shader.h"
#include "core/vao.h"


class _003_attrib_format : public test {

  public:

  _003_attrib_format();
  ~_003_attrib_format() {
    shutdown();
  }


  void init();
  void shutdown();

  void render();

  protected:


  core::vao m_vao;
  core::buffer m_buffer;
  core::shader m_shader;
};


#endif /* INCLUDE_TEST__003_ATTRIB_FORMAT_H_ */


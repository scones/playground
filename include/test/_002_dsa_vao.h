/*
 * _002_dsa_vao.h
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */

#ifndef INCLUDE_TEST__002_DSA_VAO_H_
#define INCLUDE_TEST__002_DSA_VAO_H_


#include "test.h"

#include "core/window.h"
#include "core/buffer.h"
#include "core/shader.h"
#include "core/vao.h"


class _002_dsa_vao : public test {

  public:

  _002_dsa_vao();
  ~_002_dsa_vao() {
    shutdown();
  }


  void init();
  void shutdown();

  void render();

  protected:


  core::vao m_vao;
  core::buffer m_buffer;
  core::shader m_shader;
  std::uint32_t m_buffer_name;
};


#endif /* INCLUDE_TEST__002_DSA_VAO_H_ */


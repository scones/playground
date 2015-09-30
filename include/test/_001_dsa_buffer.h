/*
 * _001_dsa_buffer.h
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */

#ifndef INCLUDE_TEST__001_DSA_BUFFER_H_
#define INCLUDE_TEST__001_DSA_BUFFER_H_


#include "test.h"

#include "core/window.h"
#include "core/buffer.h"
#include "core/shader.h"
#include "core/vertex_array.h"


class _001_dsa_buffer : public test {

  public:

  _001_dsa_buffer();
  ~_001_dsa_buffer() {
    shutdown();
  }


  void init();
  void shutdown();

  void render();

  protected:


  core::vertex_array m_vertex_array;
  core::buffer m_buffer;
  core::shader m_shader;
  std::uint32_t m_buffer_name;
};


#endif /* INCLUDE_TEST__001_DSA_BUFFER_H_ */


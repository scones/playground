/*
 * vao.cpp
 *
 *  Created on: 30.09.2015
 *      Author: scones
 */


#include "core/vao.h"


#include <GL/glew.h>
#include <iostream>


namespace core {
  void vao::init() {
    glCreateVertexArrays(m_count, &m_name);
  }


  void vao::shutdown() {
    if (m_name) {
      glDeleteVertexArrays(m_count, &m_name);
      m_name = 0;
    }
  }


  vao::activation_object vao::activate() {
    return vao::activation_object(m_name);
  }


  vao::activation_object::activation_object(std::uint32_t name) : m_name(name) {
    glBindVertexArray(m_name);
  }


  vao::activation_object::activation_object(vao::activation_object&& right) {
    m_name = right.m_name;
    right.m_name = 0;
  }


  vao::activation_object::~activation_object() {
    glBindVertexArray(0);
  }
}


/*
 * vertex_array.cpp
 *
 *  Created on: 29.12.2014
 *      Author: scones
 */


#include "core/vertex_array.h"


#include <GL/glew.h>
#include <iostream>


namespace core {
  void vertex_array::init() {
    glGenVertexArrays(m_count, &m_name);
  }


  void vertex_array::shutdown() {
    if (m_name) {
      glDeleteVertexArrays(m_count, &m_name);
      m_name = 0;
    }
  }


  vertex_array::activation_object vertex_array::activate() {
    return vertex_array::activation_object(m_name);
  }



  vertex_array::activation_object::activation_object(std::uint32_t name) : m_name(name) {
    glBindVertexArray(m_name);
  }


  vertex_array::activation_object::activation_object(vertex_array::activation_object&& right) {
    m_name = right.m_name;
    right.m_name = 0;
  }


  vertex_array::activation_object::~activation_object() {
    glBindVertexArray(0);
  }
}


/*
 * buffer.cpp
 *
 *  Created on: 11.09.2015
 *      Author: scones
 */


#include "core/buffer.h"

#include <GL/glew.h>
#include <iostream>


namespace core {

  void buffer::init() {
    if (m_count > m_names.size()) {
      m_names.clear();
      m_names.resize(m_count);
      glCreateBuffers(m_count, m_names.data());
    }
  }


  void buffer::shutdown() {
    if (m_count == m_names.size()) {
      glDeleteBuffers(m_count, m_names.data());
      m_names.clear();
    }
  }


  void buffer::bind(std::uint32_t index, std::uint32_t type) const {
    glBindBuffer(type, m_names[index]);
  }


  void buffer::data(std::uint32_t index, std::uint64_t size, void* data, std::uint32_t usage) {
    glNamedBufferData(m_names[index], size, data, usage);
  }


  void* buffer::map(std::uint32_t index, std::uint32_t access_type) {
    return glMapNamedBuffer(m_names[index], access_type);
  }


  void buffer::unmap(std::uint32_t index) {
    glUnmapNamedBuffer(m_names[index]);
  }


  buffer::activation_object buffer::activate(std::uint32_t index, std::uint32_t type) {
    return buffer::activation_object(type, m_names[index]);
  }


  buffer::activation_object::activation_object(std::uint32_t type, std::uint32_t name) : m_type(type), m_name(name) {
    glBindBuffer(m_type, m_name);
  }


  buffer::activation_object::~activation_object() {
    if (m_name) {
      glBindBuffer(m_type, 0);
    }
  }


  buffer::activation_object::activation_object(activation_object&& right) {
    m_type = right.m_type;
    m_name = right.m_name;
    right.m_name = 0;
    right.m_type = 0;
  }
}


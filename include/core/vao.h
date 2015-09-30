/*
 * vao.h
 *
 *  Created on: 30.09.2015
 *      Author: scones
 */

#ifndef VAO_H_
#define VAO_H_


#include <cinttypes>


namespace core {

  class vao {

    class activation_object {
      public:


      activation_object(std::uint32_t name);
      activation_object(activation_object&& right);
      ~activation_object();


      protected:


      std::uint32_t m_name;
    };


    public:


    explicit vao(std::uint32_t count = 1) : m_count(count), m_name(0) {
    };
    ~vao() {
      shutdown();
    }


    void init();
    void shutdown();


    activation_object activate();
    std::uint32_t const get_name() const { return m_name; }


    protected:


    std::uint32_t m_count;
    std::uint32_t m_name;
  };

}


#endif /* VAO_H_ */


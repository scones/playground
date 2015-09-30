/*
 * vertex_array.h
 *
 *  Created on: 20.10.2013
 *      Author: scn
 */

#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_


#include <cinttypes>


namespace core {

  class vertex_array {

    class activation_object {
      public:


      activation_object(std::uint32_t name);
      activation_object(activation_object&& right);
      ~activation_object();


      protected:


      std::uint32_t m_name;
    };


    public:


    explicit vertex_array(std::uint32_t count = 1) : m_count(count), m_name(0) {
    };
    ~vertex_array() {
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

#endif /* VERTEX_ARRAY_H_ */


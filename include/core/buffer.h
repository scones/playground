/*
 * buffer.h
 *
 *  Created on: 11.09.2015
 *      Author: scones
 */

#ifndef INCLUDE_BUFFER_H_
#define INCLUDE_BUFFER_H_


#include <vector>
#include <cinttypes>


namespace core {

  class buffer {

    public:


    class activation_object {
      public:


      activation_object(std::uint32_t type, std::uint32_t name);
      activation_object(activation_object&& right);
      ~activation_object();


      protected:


      std::uint32_t m_type;
      std::uint32_t m_name;
    };


    buffer(std::uint32_t count) : m_count(count), m_names() {}
    ~buffer() {
      shutdown();
    }


    void init();
    void shutdown();

    std::uint32_t get_name(std::uint32_t index) const { return m_names[index]; }


    activation_object activate(std::uint32_t index, std::uint32_t buffer_type);
    void bind(std::uint32_t index, std::uint32_t type) const;
    void data(std::uint32_t index, std::uint64_t size, void* data, std::uint32_t usage);
    void* map(std::uint32_t index, std::uint32_t access_type);
    void unmap(std::uint32_t index);


    protected:


    std::uint32_t              m_count;
    std::vector<std::uint32_t> m_names;
  };

}



#endif /* INCLUDE_BUFFER_H_ */


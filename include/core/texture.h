/*
 * texture.h
 *
 *  Created on: 20.09.2014
 *      Author: scn
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_


#include <cinttypes>


namespace core {

  class texture {
    public:


    explicit texture(std::uint32_t unit_id) : m_id(unit_id), m_handle(0) {}
    ~texture() {
      shutdown();
    }


    void init();
    void shutdown();

    void activate();
    int get_unit_id() const { return m_id; };


    protected:


    std::uint32_t m_id;
    std::uint32_t m_handle;
  };

}


#endif /* TEXTURE_H_ */


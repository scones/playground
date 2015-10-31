/*
 * resources.h
 *
 *  Created on: 10.09.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_RESOURCES_H_
#define INCLUDE_CORE_RESOURCES_H_


#include "core/charset.h"


#include <cinttypes>


namespace core {

  class resources {
    public:



    ~resources() = default;

    static core::charset*& get_font(std::uint32_t size) {
      try {
        return s_presources->m_charset_map.at(size);
      } catch(std::out_of_range & e) {
        s_presources->m_charset_map[size] = new charset(C_FONT_NAME, size);
        s_presources->m_charset_map[size]->init();
        return s_presources->m_charset_map.at(size);
      }
    }

    static void init() {
      s_presources = new resources();
    }

    static void shutdown() {
      if (s_presources) {
        for (auto font_pair : s_presources->m_charset_map) {
          delete font_pair.second;
        }
        delete s_presources;
        s_presources = nullptr;
      }
    }
    static void reset() {
      shutdown();
      init();
    }


    protected:


    resources()
      :
        m_charset_map()
    {
    }

    static resources* s_presources;
    static constexpr char C_FONT_NAME[] = "data/fonts/FreeSans.ttf";


    std::unordered_map<std::uint32_t, core::charset*> m_charset_map;
  };
}


#endif /* INCLUDE_CORE_RESOURCES_H_ */


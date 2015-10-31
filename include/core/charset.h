/*
 * charset.h
 *
 *  Created on: 21.09.2014
 *      Author: scn
 */

#ifndef CHARSET_H_
#define CHARSET_H_

#include "core/vertex_array.h"
#include "core/shader.h"
#include "core/buffer.h"
#include "core/texture.h"


#include <string>
#include <vector>
#include <cinttypes>


namespace core {

  class charset {
    public:


    enum ALIGNMENT {
      LEFT = 0,
      RIGHT
    };

    static int const MAX_WIDTH=1024;
    static int const MAP_SIZE=256;


    struct glyph {
      float advance_x, advance_y;
      float bitmap_width, bitmap_height;
      float bitmap_left, bitmap_top;
      float texture_x, texture_y;

      glyph()
        :
          advance_x(0.0f),
          advance_y(0.0f),
          bitmap_width(0.0f),
          bitmap_height(0.0f),
          bitmap_left(0.0f),
          bitmap_top(0.0f),
          texture_x(0.0f),
          texture_y(0.0f)
      {}
      glyph(float ax, float ay, float bw, float bh, float bl, float bt, float tx, float ty)
        :
          advance_x(ax),
          advance_y(ay),
          bitmap_width(bw),
          bitmap_height(bh),
          bitmap_left(bl),
          bitmap_top(bt),
          texture_x(tx),
          texture_y(ty)
      {}
    };

    charset(std::string const& font_name, unsigned int height);
    ~charset() {
      shutdown();
    }


    void init();
    void shutdown();

    void render_text(std::string const& text, float x, float y);
    void render_text_ortho(std::string const& text, std::uint32_t x, std::uint32_t y, std::uint32_t alignment = charset::ALIGNMENT::LEFT);
    std::vector<std::string> fit_text_into_box(std::string const& input_text, unsigned int box_width);


    protected:


    std::string m_font_name;
    unsigned int m_height;
    core::vertex_array m_vertex_array;
    core::shader m_shader;
    core::buffer m_vertex_buffer;
    core::buffer m_uv_buffer;
    core::texture m_texture;
    std::vector<glyph> m_glyphs;
    int m_texture_width;
    int m_texture_height;
    int m_texture_uniform_location;
    std::int32_t m_projection_matrix_location_id;
  };

}


#endif /* CHARSET_H_ */


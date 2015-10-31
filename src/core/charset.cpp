/*
 * charset.cpp
 *
 *  Created on: 21.09.2014
 *      Author: scn
 */

#include "core/charset.h"
#include "core/error/charset_error.h"
#include "core/types.h"
#include "core/window.h"
#include "app.h"


#include <gl/glew.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace core {


  int const charset::MAP_SIZE;
  int const charset::MAX_WIDTH;


  charset::charset(std::string const& font_name, std::uint32_t height)
    :
      m_font_name(font_name),
      m_height(height),
      m_vertex_array(),
      m_shader(
        450,
        {
          {GL_VERTEX_SHADER, "debug_font"},
          {GL_FRAGMENT_SHADER, "debug_font"}
        },
        {"font_sampler", "projection_matrix"}
      ),
      m_vertex_buffer(1),
      m_uv_buffer(1),
      m_texture(0),
      m_glyphs(),
      m_texture_width(0),
      m_texture_height(0),
      m_texture_uniform_location(0),
      m_projection_matrix_location_id(0)
  {
    m_glyphs.resize(MAP_SIZE);
  }


  void charset::init() {
    m_vertex_array.init();
    auto v = m_vertex_array.activate();
    m_shader.init();
    m_texture_uniform_location = m_shader.get_uniform_location("font_sampler");
  //  m_projection_matrix_location_id = m_shader.get_uniform_location("projection_matrix");

    m_vertex_buffer.init();
    m_uv_buffer.init();

    int ft_error;
    FT_Library library;
    if ((ft_error = FT_Init_FreeType(&library)))
      throw core::error::charset_error(std::string("FT_Init_FreeType"), ft_error);

    FT_Face face;
    if ((ft_error = FT_New_Face(library, m_font_name.c_str(), 0, &face)))
      throw core::error::charset_error(std::string("FT_New_Face - ") + m_font_name, ft_error);

    FT_Set_Pixel_Sizes(face, 0, m_height);
    FT_GlyphSlot g = face->glyph;

    unsigned int roww = 0;
    unsigned int rowh = 0;

    // Find minimum size for a texture holding all visible ASCII characters
    for (int i = 0; i < MAP_SIZE; i++) {
      if ((ft_error = FT_Load_Char(face, i, FT_LOAD_RENDER)))
        throw core::error::charset_error(std::string("FT_Load_Char"), ft_error);

      if (roww + g->bitmap.width + 1 >= MAX_WIDTH) {
          m_texture_width = std::max(static_cast<unsigned int>(m_texture_width), roww);
          m_texture_height += rowh;
          roww = 0;
          rowh = 0;
      }
      roww += g->bitmap.width + 1;
      rowh = std::max(rowh, static_cast<unsigned int>(g->bitmap.rows));
    }

    m_texture_width = std::max(static_cast<unsigned int>(m_texture_width), roww);
    m_texture_height += rowh;

    m_texture.init();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texture_width, m_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // We require 1 byte alignment when uploading texture data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Clamping to edges is important to prevent artifacts when scaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Linear filtering usually looks best for text
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Paste all glyph bitmaps into the texture, remembering the offset
    int ox = 0;
    int oy = 0;

    rowh = 0;

    for (int i = 0; i < 256; i++) {
      if ((ft_error = FT_Load_Char(face, i, FT_LOAD_RENDER)))
        throw core::error::charset_error(std::string("FT_Load_Char"), ft_error);

      if (ox + g->bitmap.width + 1 >= MAX_WIDTH) {
        oy += rowh;
        rowh = 0;
        ox = 0;
      }

      const int buffer_size = g->bitmap.width*g->bitmap.rows*4;
      unsigned char *buffer = new unsigned char[buffer_size];
      memset(buffer, 0, buffer_size);
      for (unsigned int i = 0; i < g->bitmap.width; ++i) {
        for (unsigned int j = 0; j < g->bitmap.rows; ++j) {
          buffer[i*g->bitmap.rows*4 + j*4 + 3] = g->bitmap.buffer[i*g->bitmap.rows + j];
        }
      }

      glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
      delete[] buffer;
      m_glyphs[i] = glyph(
        g->advance.x >> 6,
        g->advance.y >> 6,
        g->bitmap.width,
        g->bitmap.rows,
        g->bitmap_left,
        g->bitmap_top,
        (float)ox / (float)m_texture_width,
        (float)oy / (float)m_texture_height
      );

      rowh = std::max(rowh, g->bitmap.rows);
      ox += g->bitmap.width + 1;
    }
  }


  void charset::shutdown() {
    m_vertex_buffer.shutdown();
    m_uv_buffer.shutdown();
    m_vertex_array.shutdown();
    m_shader.shutdown();
    m_texture.shutdown();
    m_glyphs.clear();
    m_texture_uniform_location = 0;
  }


  void charset::render_text(std::string const& text, float x, float y) {
    glEnable(GL_BLEND);
    static const unsigned int points_layout_id = 0;
    static const unsigned int uvs_layout_id = 1;
    float sx = 2.0f / static_cast<float>(app::get_config()->get_resolution().x);
    float sy = 2.0f / static_cast<float>(app::get_config()->get_resolution().y);

    auto vertex_arrqy_activation_object = m_vertex_array.activate();
    auto shader_activation_object = m_shader.activate();

    // Use the texture containing the atlas
    glProgramUniform1i(m_shader.get_program_name(), m_texture_uniform_location, m_texture.get_unit_id());

    // projection matrix (identity)
    glm::highp_mat4x4 projection_matrix(1.0f);
    glProgramUniformMatrix4fv(m_shader.get_program_name(), m_projection_matrix_location_id, 1, GL_FALSE, &projection_matrix[0][0]);

    m_texture.activate();

    int count = 0;

    std::vector<core::t_vec2f> points;
    std::vector<core::t_vec2f> uvs;

    // Loop through all characters
    for (char c : text) {
      // Calculate the vertex and texture coordinates
      float x2 = x + m_glyphs[c].bitmap_left * sx;
      float y2 = -y - m_glyphs[c].bitmap_top * sy;
      float w = m_glyphs[c].bitmap_width * sx;
      float h = m_glyphs[c].bitmap_height * sy;

      // Advance the cursor to the start of the next character
      x += m_glyphs[c].advance_x * sx;
      y += m_glyphs[c].advance_y * sy;

      // Skip glyphs that have no pixels
      if (!w || !h)
        continue;

      points.push_back(core::t_vec2f(x2       , -y2));
      points.push_back(core::t_vec2f(x2       , -y2 - h));
      points.push_back(core::t_vec2f(x2 +    w, -y2));
      points.push_back(core::t_vec2f(x2 +    w, -y2));
      points.push_back(core::t_vec2f(x2       , -y2 - h));
      points.push_back(core::t_vec2f(x2 +    w, -y2 - h));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x                                             , m_glyphs[c].texture_y));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x                                             , m_glyphs[c].texture_y + (float)m_glyphs[c].bitmap_height / (float)m_texture_height));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x + (float)m_glyphs[c].bitmap_width / (float)m_texture_width, m_glyphs[c].texture_y));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x + (float)m_glyphs[c].bitmap_width / (float)m_texture_width, m_glyphs[c].texture_y));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x                                             , m_glyphs[c].texture_y + (float)m_glyphs[c].bitmap_height / (float)m_texture_height));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x + (float)m_glyphs[c].bitmap_width / (float)m_texture_width, m_glyphs[c].texture_y + (float)m_glyphs[c].bitmap_height / (float)m_texture_height));

      count += 6;
    }
    // activate attribute arrays
    glEnableVertexAttribArray(points_layout_id);
    glEnableVertexAttribArray(uvs_layout_id);

    // set data
    auto vertex_buffer_activation_object = m_vertex_buffer.activate(0, GL_ARRAY_BUFFER);
    m_vertex_buffer.data(0, sizeof(core::t_vec2f) * points.size(), points.data(), GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(core::t_vec2f) * points.size(), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(points_layout_id, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    auto uv_buffer_activation_object = m_uv_buffer.activate(0, GL_ARRAY_BUFFER);
    m_uv_buffer.data(0, sizeof(core::t_vec2f) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(core::t_vec2f) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(uvs_layout_id, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Draw all the character on the screen in one go
    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(points_layout_id);
    glDisableVertexAttribArray(uvs_layout_id);
    glDisable(GL_BLEND);
  }


  void charset::render_text_ortho(std::string const& text, std::uint32_t x, std::uint32_t y, std::uint32_t alignment) {
    static const unsigned int points_layout_id = 0;
    static const unsigned int uvs_layout_id = 1;

    auto vertex_array_activation_object = m_vertex_array.activate();
    auto shader_activation_object = m_shader.activate();

    // Use the texture containing the atlas
    glProgramUniform1i(m_shader.get_program_name(), m_texture_uniform_location, m_texture.get_unit_id());

    // projection matrix (identity)
    glm::highp_mat4x4 projection_matrix(glm::ortho<float>(0, app::get_config()->get_resolution().x, app::get_config()->get_resolution().y, 0, -1.0f, 1.0e20f));
    glProgramUniformMatrix4fv(m_shader.get_program_name(), m_projection_matrix_location_id, 1, GL_FALSE, &projection_matrix[0][0]);

    m_texture.activate();

    int count = 0;

    std::vector<core::t_vec2f> points;
    std::vector<core::t_vec2f> uvs;

    float fx = (float)x;
    float fy = (float)y;

    // calculate x offset for align right
    if (charset::ALIGNMENT::RIGHT == alignment) {
      for (auto c : text) {
        fx -= m_glyphs[c].advance_x;
      }
    }

    // Loop through all characters
    for (char c : text) {
      // Calculate the vertex and texture coordinates
      float left   = fx + (float)m_glyphs[c].bitmap_left;
      float right  = fx + (float)m_glyphs[c].bitmap_left + m_glyphs[c].bitmap_width;
      float top    = fy + ((float)m_height - (float)m_glyphs[c].bitmap_top);
      float bottom = fy + ((float)m_height - (float)m_glyphs[c].bitmap_top)  + m_glyphs[c].bitmap_height;
      fx += m_glyphs[c].advance_x;
      fy += m_glyphs[c].advance_y;

      points.push_back(core::t_vec2f(left , top));
      points.push_back(core::t_vec2f(left , bottom));
      points.push_back(core::t_vec2f(right, top));
      points.push_back(core::t_vec2f(right, top));
      points.push_back(core::t_vec2f(left , bottom));
      points.push_back(core::t_vec2f(right, bottom));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x                                             , m_glyphs[c].texture_y));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x                                             , m_glyphs[c].texture_y + (float)m_glyphs[c].bitmap_height / (float)m_texture_height));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x + (float)m_glyphs[c].bitmap_width / (float)m_texture_width, m_glyphs[c].texture_y));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x + (float)m_glyphs[c].bitmap_width / (float)m_texture_width, m_glyphs[c].texture_y));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x                                             , m_glyphs[c].texture_y + (float)m_glyphs[c].bitmap_height / (float)m_texture_height));
      uvs.push_back(core::t_vec2f(m_glyphs[c].texture_x + (float)m_glyphs[c].bitmap_width / (float)m_texture_width, m_glyphs[c].texture_y + (float)m_glyphs[c].bitmap_height / (float)m_texture_height));

      count += 6;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // activate attribute arrays
    glEnableVertexAttribArray(points_layout_id);
    glEnableVertexAttribArray(uvs_layout_id);

    // set data
    auto vertex_buffer_activation_object = m_vertex_buffer.activate(0, GL_ARRAY_BUFFER);
    m_vertex_buffer.data(0, sizeof(core::t_vec2f) * points.size(), points.data(), GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(core::t_vec2f) * points.size(), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(points_layout_id, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    auto uv_buffer_activation_object = m_uv_buffer.activate(0, GL_ARRAY_BUFFER);
    m_uv_buffer.data(0, sizeof(core::t_vec2f) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(core::t_vec2f) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(uvs_layout_id, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Draw all the character on the screen in one go
    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(points_layout_id);
    glDisableVertexAttribArray(uvs_layout_id);

    glDisable(GL_BLEND);
  }


  std::vector<std::string> charset::fit_text_into_box(std::string const& input_text, unsigned int const box_width) {
    unsigned int current_line_width = 0;
    std::string current_word;
    std::string current_line;
    std::vector<std::string> lines(0);
    for (auto const token : input_text) {
      unsigned int token_width = m_glyphs[token].bitmap_width;
      if ( ' ' == token ) {
        if ( box_width < current_line_width + token_width ) {

        } else {
          current_line_width += token_width;
        }
      } else {
        if ( box_width < current_line_width + token_width ) {
        } else {
          current_line_width += token_width;
        }
      }
    }
    return lines;
  }

}



layout(location = 0) in vec2 text_pos;
layout(location = 1) in vec2 vertex_uv;


out gl_PerVertex {
  vec4 gl_Position;
};


out block {
  vec2 uv;
} Out;


uniform mat4 projection_matrix;
uniform sampler2D font_sampler;


void main(void) {
//  vec4 position = projection_matrix * vec4(text_pos.xy, 1.0, 1.0);
  vec4 position = projection_matrix * vec4(text_pos.xy, 0.0, 1.0);
  gl_Position = position;

  Out.uv = vertex_uv;
}


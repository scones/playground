
in block {
  vec2 uv;
};


layout (location = 0, index = 0) out vec4 color;


uniform sampler2D font_sampler;


void main(void) {
  color = vec4(0.9, 0.9, 0.9, texture(font_sampler, uv).a);// * vec4(0.0, 1.0, 0.0, 1.0);
//  color = vec4(uv.xy, 0.0, 1.0);
}


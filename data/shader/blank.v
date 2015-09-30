layout(location = 0) in vec4 position;

uniform mat4 world_matrix;


out gl_PerVertex {
  vec4 gl_Position;
};

void main() {
  gl_Position = position;
}


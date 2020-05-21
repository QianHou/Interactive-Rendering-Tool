#version 330

layout(location = 0) in vec3 posVertex;
layout(location = 1) in vec3 colorsVertex;
layout(location = 2) in vec2 textureIndexVertex;

uniform mat4 viewMatrix;
out vec4 colors;
out vec2 textureIndex;

void main() {
  gl_Position = viewMatrix * vec4(posVertex, 1.0f);
  colors = vec4(colorsVertex, 1.0f);
  textureIndex = textureIndexVertex;
}
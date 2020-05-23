#version 330

layout(location = 0) in vec3 posVertex;
layout(location = 1) in vec2 textureIndexVertex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
out vec2 textureIndex;

void main() {
  gl_Position = viewMatrix * modelMatrix * vec4(posVertex, 1.0f);
  textureIndex = textureIndexVertex;
}
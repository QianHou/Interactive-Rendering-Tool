#version 330

layout(location = 0) in vec3 posVertex;
layout(location = 1) in vec2 textureIndexVertex;
layout(location = 2) in vec3 normalVectorVertex;

out vec2 textureIndex;
out vec3 fragPosition;
out vec3 normalVector;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main() {
  gl_Position = viewMatrix * modelMatrix * vec4(posVertex, 1.0f);
  fragPosition = vec3(modelMatrix * vec4(posVertex, 1.0));
  normalVector = normalVectorVertex;
  textureIndex = textureIndexVertex;
}
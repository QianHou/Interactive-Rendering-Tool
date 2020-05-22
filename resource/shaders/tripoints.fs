#version 330

in vec2 textureIndex;

out vec4 fragColor;

uniform sampler2D textureImage;

void main() {
  fragColor = texture(textureImage, textureIndex);
}

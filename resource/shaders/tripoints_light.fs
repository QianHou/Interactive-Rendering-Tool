#version 330

in vec4 colors;
in vec2 textureIndex;
in vec3 normalVector;
in vec3 fragPosition;

out vec4 fragColor;

uniform sampler2D textureImage;
uniform vec3      lightColor;
uniform vec3      lightPosition;
uniform vec3      lightIntensity;

void main() {
  vec3 ambient = lightIntensity * lightColor;

  vec3 normalizeNormal = normalize(normalVector);
  vec3 lightDirection  = normalize(lightPosition - fragPosition);
  float diff = max(dot(normalizeNormal, lightDirection), 0.0);
  vec3 diffuse = diff * lightColor;

  vec4 light_result = vec4(ambient + diffuse, 1.0f);
  fragColor = light_result * texture(textureImage, textureIndex);
}

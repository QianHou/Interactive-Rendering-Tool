#version 330

in vec4 colors;
in vec2 textureIndex;
in vec3 normalVector;
in vec3 fragPosition;

out vec4 fragColor;

uniform sampler2D textureImage;
uniform vec3      lightAmbient;

uniform vec3      lightColor1;
uniform vec3      lightPosition1;
uniform vec3      lightIntensity1;

uniform vec3      lightColor2;
uniform vec3      lightPosition2;
uniform vec3      lightIntensity2;

void main() {
  float constantAttenuation = 1.0;
  float linearAttenuation = 0.027;
  float expAttenuation = 0.0028;

  vec3 normalizeNormal = normalize(normalVector);

  vec3 lightDirection1  = normalize(lightPosition1 - fragPosition);
  float lightDistance1  = length(lightPosition1 - fragPosition);
  float diff1 = max(dot(normalizeNormal, lightDirection1), 0.0);
  float lightAttenuation1 = 1.0 / (constantAttenuation + linearAttenuation * lightDistance1 + expAttenuation * lightDistance1 * lightDistance1);
  vec3 projColor1 = diff1 * lightColor1 * lightIntensity1 * lightAttenuation1;

  vec3 lightDirection2  = normalize(lightPosition2 - fragPosition);
  float lightDistance2  = length(lightPosition2 - fragPosition);
  float diff2 = max(dot(normalizeNormal, lightDirection2), 0.0);
  float lightAttenuation2 = 1.0 / (constantAttenuation + linearAttenuation * lightDistance2 + expAttenuation * lightDistance2 * lightDistance2);
  vec3 projColor2 = diff2 * lightColor2 * lightIntensity2 * lightAttenuation2;

  vec4 light_result = vec4(lightAmbient + projColor1 + projColor2, 1.0f);
  fragColor = light_result * texture(textureImage, textureIndex);
}

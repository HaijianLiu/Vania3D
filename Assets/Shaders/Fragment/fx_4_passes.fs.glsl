
#version 330 core
layout (location = 0) out vec4 albedoPass;

in vec3 position;
in vec2 uv;
in vec3 normal;

uniform sampler2D albedoMap;

void main() {

	albedoPass = vec4(pow(texture(albedoMap, uv).rgb, vec3(2.2)), 1);
}

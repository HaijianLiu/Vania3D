
#version 330 core
layout (location = 0) out vec4 outlinePass;

in vec2 uv;

uniform sampler2D normalPass;
uniform sampler2D positionPass;

void main() {
	// passes
	vec3 n = texture(normalPass, uv).rgb;
	vec3 position = texture(positionPass, uv).rgb;

	outlinePass = vec4(position, 1.0);
}

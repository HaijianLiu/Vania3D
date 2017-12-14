
#version 330 core
out vec4 lutPass;

in vec2 uv;

uniform sampler2D combinePass;

void main() {
	vec3 combineColor = texture(combinePass, uv).rgb;
	lutPass = vec4(combineColor, 1);
}

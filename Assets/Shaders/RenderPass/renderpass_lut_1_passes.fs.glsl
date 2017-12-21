
#version 330 core
out vec4 lutPass;

in vec2 uv;

uniform sampler2D combinePass;
uniform sampler2D lut;

void main() {
	vec3 combineColor = texture(combinePass, uv).rgb;

	vec2 uvLut = vec2(0.5, 0.5);
	float blue = floor(combineColor.b * 15);
	float red = combineColor.r;
	float green = combineColor.g;

	uvLut.x = blue / 16;
	uvLut.y = 1 - green;

	vec3 lutColor = texture(lut, uvLut).rgb;

	// lutPass = vec4(lutColor, 1);
	lutPass = vec4(0, 0, combineColor.b, 1);
}

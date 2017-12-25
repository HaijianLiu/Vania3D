
#version 330 core
out vec4 lutPass;

in vec2 uv;

uniform sampler2D combinePass;
uniform sampler2D lut;

void main() {
	vec3 combineColor = texture(combinePass, uv).rgb;

	// blue channel index
	float blueIndex = floor(combineColor.b * 16.0); // 0 ~ 15

	// lut mapping
	highp vec2 uvMapping;
	uvMapping.x = (blueIndex + 0.03125 + 0.9375 * combineColor.r) * 0.0625;
	uvMapping.y = 0.03125 + 0.9375 * combineColor.g;

	lutPass = vec4(texture(lut, uvMapping).rgb, 1);
}

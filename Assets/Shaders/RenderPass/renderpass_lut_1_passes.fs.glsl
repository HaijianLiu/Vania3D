
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
	uvMapping.x = blueIndex / 16.0 + 1.0 / 512.0 + 15.0 * combineColor.r / 256.0;
	uvMapping.y = 1.0 / 32.0 + 15.0 * combineColor.g / 16.0;

	lutPass = vec4(texture(lut, uvMapping).rgb, 1);
}

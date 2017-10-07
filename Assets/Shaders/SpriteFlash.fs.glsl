
#version 330 core
layout (location = 0) out vec4 colorPass;

in vec2 texCoord;

uniform sampler2D texColor;
uniform vec4 spriteColor;

void main() {
	vec4 color = texture(texColor, texCoord);
	if(color.a == 0) discard;
	colorPass = vec4((1.0 - color.r), (1.0 - color.g), (1.0 - color.b), 1.0) * spriteColor;
}


#version 330 core
layout (location = 0) out vec4 colorPass;

in vec2 texCoord;

uniform sampler2D texColor;
uniform vec4 spriteColor;

void main() {
	vec4 color = texture(texColor, texCoord);
	if(color.a == 0) discard;
	colorPass = color * spriteColor;
}

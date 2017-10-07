
#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D pass[1];

void main() {
	vec4 colorPass = texture(pass[0],texCoord);
	fragColor = colorPass;
}

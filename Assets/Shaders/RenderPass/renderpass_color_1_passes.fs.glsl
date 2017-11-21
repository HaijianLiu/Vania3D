
#version 330 core
out vec4 fragColor;

in vec2 uv;
uniform sampler2D passes[1];

void main() {
	fragColor = vec4(texture(passes[0], uv).rgb, 1);
}

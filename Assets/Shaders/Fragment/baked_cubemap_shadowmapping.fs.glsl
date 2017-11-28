
#version 330 core

in vec3 position;
uniform vec3 lightPosition;
uniform float lightRadius;

void main() {
	float d = length(position - lightPosition);
	// map to (0, 1) range by dividing by light radius
	d = d / lightRadius;
	// write this as modified depth
	gl_FragDepth = d;
}

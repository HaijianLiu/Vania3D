
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

out vec4 fragPosition; // FragPos from GS (output per emitvertex)

uniform mat4 shadowMatrices[6];

void main() {
	for (int face = 0; face < 6; face++) {
		gl_Layer = face; // built-in variable that specifies to which face we render.
		// for each triangle's vertices
		for (int i = 0; i < 3; i++) {
			fragPosition = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * fragPosition;
			EmitVertex();
		}
		EndPrimitive();
	}
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
// layout (location = 3) in vec3 atangent;
// layout (location = 4) in vec3 abtangent;
layout (location = 5) in ivec4 aBoneid;
layout (location = 6) in vec4 aWeight;


out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
// for ao
out vec3 ViewPos;
out vec3 ViewNormal;

out float weight0;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 bones[52];

void main()
{

	  mat4 BoneTransform = bones[aBoneid[0]] * aWeight[0];
    BoneTransform += bones[aBoneid[1]] * aWeight[1];
    BoneTransform += bones[aBoneid[2]] * aWeight[2];
    BoneTransform += bones[aBoneid[3]] * aWeight[3];

    TexCoords = aTexCoords;
    WorldPos = vec3(model * BoneTransform * vec4(aPos, 1.0));
    Normal = vec3(model * BoneTransform * vec4(aNormal, 0.0));
// for ao
		ViewPos = vec3(view * model * vec4(aPos, 1.0));
		// mat3 normalMatrix = transpose(inverse(mat3(view * model)));
		mat3 normalMatrix = transpose(mat3(view * model));
		ViewNormal = normalMatrix * aNormal;

    // gl_Position =  projection * view * vec4(WorldPos, 1.0);
		gl_Position =  projection * view * model * BoneTransform * vec4(aPos, 1.0);

		weight0 = aWeight[0];


}

#version 330 core
layout (location = 0) out vec4 albedoColor;
layout (location = 1) out vec4 normalColor;
layout (location = 2) out vec4 mraColor;
layout (location = 3) out vec4 worldPos;
layout (location = 4) out vec4 alpha;
// for ao
layout (location = 5) out vec4 viewPos;
layout (location = 6) out vec4 viewNormal;


// out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

in vec3 ViewPos;
in vec3 ViewNormal;

in float weight0;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D maskMap;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main() {
	// albedoColor = vec4(texture(albedoMap, TexCoords).rgb, 1.0);
	// albedoColor = vec4(texture(albedoMap, TexCoords).rgb, texture(maskMap, TexCoords).a);
	// albedoColor = vec4(vec3(weight0), texture(maskMap, TexCoords).a);
	albedoColor = vec4(pow(texture(albedoMap, TexCoords).rgb, vec3(2.2)), texture(maskMap, TexCoords).a);
	// albedoColor = pow(albedoColor, vec4(2.2));
	// normalColor = vec4(normalize(Normal), 1.0);
	normalColor = vec4(getNormalFromMap(), texture(maskMap, TexCoords).a);
	mraColor    = vec4(texture(maskMap, TexCoords).b, texture(maskMap, TexCoords).g, texture(maskMap, TexCoords).r, texture(maskMap, TexCoords).a);
	worldPos    = vec4(WorldPos, texture(maskMap, TexCoords).a);
	alpha       = vec4(1.0,1.0,1.0, texture(maskMap, TexCoords).a);
	// for ao
	viewPos     = vec4(ViewPos, texture(maskMap, TexCoords).a);
	viewNormal  = vec4(ViewNormal, texture(maskMap, TexCoords).a);
}

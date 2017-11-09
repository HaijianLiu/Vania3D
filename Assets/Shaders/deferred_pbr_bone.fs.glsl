
#version 330 core
layout (location = 0) out vec4 albedoPass;
layout (location = 1) out vec4 normalPass;
layout (location = 2) out vec4 mrcPass;
layout (location = 3) out vec4 positionPass;
layout (location = 4) out vec4 alphaPass;

in vec2 uv;
in vec3 position;
in vec3 normal;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D maskMap;

vec3 getNormalFromMap();

void main() {
	albedoPass = vec4(pow(texture(albedoMap, uv).rgb, vec3(2.2)), texture(maskMap, uv).a);
	normalPass = vec4(getNormalFromMap(), texture(maskMap, uv).a);
	mrcPass = vec4(texture(maskMap, uv).b, texture(maskMap, uv).g, texture(maskMap, uv).r, texture(maskMap, uv).a);
	positionPass = vec4(position, texture(maskMap, uv).a);
	alphaPass = vec4(1.0,1.0,1.0, texture(maskMap, uv).a);
}

vec3 getNormalFromMap() {
	vec3 tangentNormal = texture(normalMap, uv).xyz * 2.0 - 1.0;

	vec3 q1 = dFdx(position);
	vec3 q2 = dFdy(position);
	vec2 st1 = dFdx(uv);
	vec2 st2 = dFdy(uv);

	vec3 n = normalize(normal);
	vec3 t = normalize(q1 * st2.t - q2 * st1.t);
	vec3 b = -normalize(cross(n, t));
	mat3 tbn = mat3(t, b, n);

	return normalize(tbn * tangentNormal);
}

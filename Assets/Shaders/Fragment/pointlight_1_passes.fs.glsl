
#version 330 core
layout (location = 0) out vec4 pointLightPass;

in vec2 uv;
in vec3 lightColor;
in vec3 lightPosition;

uniform sampler2D albedoPass;
uniform sampler2D normalPass;
uniform sampler2D mrcPass;
uniform sampler2D positionPass;
uniform vec3 cameraPosition;

const float PI = 3.14159265359;

vec3 cookTorranceBRDF(vec3 n, vec3 v, vec3 l, float roughness, vec3 f0);

void main() {
	// passes
	vec3 albedo = texture(albedoPass, uv).rgb;
	vec3 n = texture(normalPass, uv).rgb;
	vec3 mrc = texture(mrcPass, uv).rgb;
	vec3 position = texture(positionPass, uv).rgb;
	// mrc mask
	float metallic = mrc.r;
	float roughness = mrc.g;
	float cavity = mrc.b;
	// f0
	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, vec3(metallic));
	// normals
	vec3 v = normalize(cameraPosition - position);
	vec3 l = normalize(lightPosition - position);
	vec3 r = reflect(-v, n);
	// light radiance
	float distance = length(lightPosition - position);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = lightColor * attenuation;


	// Cook-Torrance BRDF
	vec3 specular = cookTorranceBRDF(n, v, l, roughness, f0);

	// outgoing radiance
	float ndotl = max(dot(n, l), 0.0);
	vec3 diffuseF = vec3(1.0) - specular;
	diffuseF *= 1.0 - metallic;
	pointLightPass = vec4((diffuseF * albedo / PI + specular) * radiance * ndotl, 1);
	// pointLightPass = pointLightPass / (pointLightPass + vec4(1.0));
}

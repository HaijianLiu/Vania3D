
#version 330 core
out vec4 fragColor;

in vec2 uv;

uniform sampler2D fx;
uniform sampler2D lightingPass;

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

uniform sampler2D passes[4];

// ibl
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// transform
uniform vec3 cameraPosition;

// brdf
vec3 fresnelSchlickRoughness(float cosTheta, vec3 f0, float roughness) {
	return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(1.0 - cosTheta, 5.0);
}
// shadow
float shadowMapping(mat4 lightSpaceMatrix, vec3 n, vec3 position);

void main() {
	// passes
	vec3 albedo = texture(passes[0], uv).rgb;
	vec3 n = texture(passes[1], uv).rgb;
	vec3 mrc = texture(passes[2], uv).rgb;
	vec3 position = texture(passes[3], uv).rgb;
	// mrc mask
	float metallic = mrc.r;
	float roughness = mrc.g;
	float cavity = mrc.b;

	vec3 fxColor = texture(fx, uv).rgb;
	vec3 lightingColor = texture(lightingPass, uv).rgb;

	vec3 v = normalize(cameraPosition - position);
	vec3 r = reflect(-v, n);

	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, vec3(metallic));

	// ibl ambient lighting
	vec3 specularF = fresnelSchlickRoughness(max(dot(n, v), 0.0), f0, roughness);
	vec3 diffuseF = 1.0 - specularF;
	diffuseF *= 1.0 - metallic;
	// ibl diffuse
	vec3 irradiance = texture(irradianceMap, n).rgb;
	vec3 diffuse = irradiance * albedo;
	// ibl specular
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(prefilterMap, r, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf  = texture(brdfLUT, vec2(max(dot(n, v), 0.0), roughness)).rg;
	vec3 specular = prefilteredColor * (specularF * brdf.x + brdf.y);
	// ambient
	// vec3 ambient = diffuseF * diffuse + specularF * diffuse * specular;
	vec3 ambient = specularF * diffuse * specular;


	// shadow
	float shadow = shadowMapping(lightSpaceMatrix, n, position);
	lightingColor = mix(lightingColor, vec3(0), shadow);

	vec3 color = ambient + lightingColor;
	// vec3 color = lightingColor;

	// exposion & cavity & shadow
	color *= cavity;

	color += fxColor;

	// hdr tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0/2.2));

	// hard code fog
	// float fogFactor = (10 - cameraPosition.z) / 10;
	// color = mix(vec3(0), color, fogFactor);

	// fragColor = vec4(mix(vec3(0), color, alpha), 1.0);
	// fragColor = vec4(texture(passes[1], uv).rgb, 1.0);
	fragColor = vec4(color, 1.0);
	// fragColor = vec4(lightingColor, 1.0);
	// fragColor = vec4(vec3(texture(shadowMap, uv).r), 1.0);
}

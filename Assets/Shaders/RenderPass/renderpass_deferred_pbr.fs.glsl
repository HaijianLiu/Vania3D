
#version 330 core
out vec4 fragColor;

in vec2 uv;

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;


uniform sampler2D fxPass;
uniform sampler2D ambientPass;
uniform sampler2D lightingPass;


uniform sampler2D normalPass;
uniform sampler2D mrcPass;
uniform sampler2D positionPass;


// shadow
float shadowMapping(mat4 lightSpaceMatrix, vec3 n, vec3 position);

void main() {
	vec3 n = texture(normalPass, uv).rgb;
	vec3 mrc = texture(mrcPass, uv).rgb;
	vec3 position = texture(positionPass, uv).rgb;
	float cavity = mrc.b;

	vec3 fxColor = texture(fxPass, uv).rgb;
	vec3 ambientColor = texture(ambientPass, uv).rgb;
	vec3 lightingColor = texture(lightingPass, uv).rgb;

	// shadow
	float shadow = shadowMapping(lightSpaceMatrix, n, position);
	lightingColor = mix(lightingColor, vec3(0), shadow);


	// final lighting
	vec3 color = ambientColor + lightingColor;
	// exposion & cavity & shadow
	color *= cavity;
	// fx color pass
	color += fxColor;
	// hdr tonemapping & gamma correct
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));
	// final
	fragColor = vec4(color, 1.0);
}

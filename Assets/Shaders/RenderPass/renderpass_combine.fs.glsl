
#version 330 core
layout (location = 0) out vec4 combinePass;

in vec2 uv;

uniform sampler2D mrcPass;

uniform sampler2D fxPass;
uniform sampler2D ambientPass;
uniform sampler2D lightingPass;
uniform sampler2D shadowPass;
uniform sampler2D ssaoPass;
uniform sampler2D outlinePass;


void main() {
	vec3 fxColor = texture(fxPass, uv).rgb;
	vec3 ambientColor = texture(ambientPass, uv).rgb;
	vec3 lightingColor = texture(lightingPass, uv).rgb;
	float shadow = texture(shadowPass, uv).r;
	float ssao = texture(ssaoPass, uv).r;
	vec3 outline = texture(outlinePass, uv).rgb;

	float cavity = texture(mrcPass, uv).b;

	// final lighting
	vec3 color = ambientColor + mix(lightingColor, vec3(0), shadow);
	// exposion & cavity & shadow
	color = color * cavity * ssao;
	// fx color pass
	color += fxColor; // scene03
	// hdr tonemapping & gamma correct
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	// final
	combinePass = vec4(outline, 1.0);
}

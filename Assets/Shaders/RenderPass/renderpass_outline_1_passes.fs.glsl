
#version 330 core
layout (location = 0) out vec4 outlinePass;

in vec2 uv;

uniform sampler2D normalPass;
uniform sampler2D positionPass;

void main() {
	// passes
	vec3 n = texture(normalPass, uv).rgb;
	vec3 position = texture(positionPass, uv).rgb;

	float dx = 1.0 / 1920.0;
  float dy = 1.0 / 1080;

  vec3 center = texture(normalPass, uv).rgb;

  vec3 top = texture(normalPass, uv + vec2(0, dy)).rgb;
  vec3 topRight = texture(normalPass, uv + vec2(dx, dy)).rgb;
  vec3 right = texture(normalPass, uv + vec2(dx, 0)).rgb;

  vec3 t = center - top;
  vec3 r = center - right;
  vec3 tr = center - topRight;

  t = abs( t );
  r = abs( r );
  tr = abs( tr );

  float samp;
  samp = max( samp, t.x );
  samp = max( samp, t.y );
  samp = max( samp, t.z );
  samp = max( samp, r.x );
  samp = max( samp, r.y );
  samp = max( samp, r.z );
  samp = max( samp, tr.x );
  samp = max( samp, tr.y );
  samp = max( samp, tr.z );

	samp = clamp(0.0, 1.0, samp);
	samp = 1.0 - samp;

	outlinePass = vec4(vec3(samp), 1.0);
}

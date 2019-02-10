#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;

void main()
{
	vec4 proc = texture(tex,TexCoords);
	vec2 pos = (gl_FragCoord.xy/vec2(1280,720))-vec2(0.5);
	float vignette = smoothstep(0.75,0.75-0.45,length(pos));
	proc.rgb = mix(proc.rgb,proc.rgb*vignette,0.75);

	float alpha = (proc.r*0.299)+(proc.g*0.587)+(proc.b*0.114);
	vec3 sepia = vec3(alpha)*vec3(1.35,1.0,0.65);
	outColour = vec4(mix(proc.rgb,sepia,0.75),1.0);
}

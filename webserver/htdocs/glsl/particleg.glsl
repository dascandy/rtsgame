#version 330
layout(points) in;
layout(points, max_vertices=1) out;

uniform sampler3D space;

in vec3 ilocation[];
in vec3 idirection[];
in vec4 ivars[];
out vec3 location;
out vec3 direction;
out vec4 vars;

void main()
{
   if (ivars[0].x < 1) return;
   if (ivars[0].y < 1) return;
	direction = idirection[0] * 0.98 + vec3(0, ivars[0].x * 0.001, 0);
	vars = ivars[0] + vec4(-0.005*ivars[0].x, -1, length(idirection[0]) / 100, 0);
	location = ilocation[0] + idirection[0];
   gl_Position = vec4(location, 1);
	EmitVertex();
	EndPrimitive();
}

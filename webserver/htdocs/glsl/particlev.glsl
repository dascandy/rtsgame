#version 330
in vec3 in_loc;
in vec3 in_dir;
in vec4 in_vars;
out vec3 ilocation;
out vec3 idirection;
out vec4 ivars;

void main()
{
	ilocation = in_loc;
   idirection = in_dir;
	ivars = in_vars;
}

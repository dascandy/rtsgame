##V
#version 330
in vec3 in_loc;
in vec3 in_normal;
in vec2 in_tex0;
in ivec3 in_arg;

void main()
{
   gl_Position = vec4(0, 0, 0, 1);
}
##F
#version 330
uniform sampler2D heightmap;

void main (void)
{
    discard;
}
##I
in_loc
in_normal
in_tex0
in_arg

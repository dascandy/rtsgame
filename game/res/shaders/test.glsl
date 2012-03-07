##V
#version 330
in vec3 in_loc;
in vec3 in_normal;
in vec2 in_tex0;
in ivec3 in_arg;
out vec2 f_texcoord;

void main()
{
   gl_Position = vec4(in_loc, 1);
   f_texcoord = in_tex0;
}
##F
#version 330
uniform sampler2D heightmap;

in vec2 f_texcoord;

void main (void)
{
    vec4 color = texture(heightmap, f_texcoord);
    gl_FragData[0] = vec4(color.xyz, 0.0);
}
##I
in_loc
in_normal
in_tex0
in_arg

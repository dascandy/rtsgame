##V
#version 330
in vec3 in_loc;
in vec3 in_normal;
in vec2 in_tex0;
in ivec3 in_arg;
uniform mat4 mat_model;
out vec2 f_texcoord;

void main()
{
   gl_Position = mat_model * vec4(in_loc, 1);
   f_texcoord = in_tex0;
}
##F
#version 330
uniform sampler2D font;

in vec2 f_texcoord;

void main (void)
{
    vec4 color = texture(font, f_texcoord);
    if (color.a == 0) color = vec4(0.0,0.0,0.0,0.0);
    gl_FragData[0] = color;
}
##I
in_loc
in_normal
in_tex0
in_arg

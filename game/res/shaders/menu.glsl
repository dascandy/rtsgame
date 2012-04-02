##V
#version 330
in vec3 in_loc;
in vec3 in_normal;
in ivec2 in_tex0;
in ivec3 in_arg;
uniform mat4 mat_model;
uniform mat4 mat_vp;
out vec2 f_texcoord;

void main()
{
   gl_Position = mat_vp * (mat_model * vec4(in_loc, 1));
   f_texcoord = in_tex0 / 65535.0;
}
##F
#version 330
uniform sampler2D font;
uniform vec3 color;

in vec2 f_texcoord;

void main (void)
{
    vec4 texcolor = texture(font, f_texcoord) * vec4(color, 1);
    if (texcolor.a == 0) texcolor = vec4(0.0,0.0,0.0,0.0);
    gl_FragData[0] = texcolor;
//    gl_FragData[0] = vec4(f_texcoord, 0, 1);
}
##I
in_loc
in_normal
in_tex0
in_arg

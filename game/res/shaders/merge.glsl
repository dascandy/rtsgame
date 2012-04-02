##V
#version 330
in vec3 in_loc;
in vec3 in_normal;
in ivec2 in_tex0;
in ivec3 in_arg;
out vec2 f_texcoord;

void main()
{
   gl_Position = vec4(in_loc, 1);
   f_texcoord = in_tex0 / 65535;
}
##F
#version 330
uniform sampler2D menu;
uniform sampler2D gameview;
uniform float menuOpacity;

in vec2 f_texcoord;

void main (void)
{
    gl_FragData[0] = texture(menu, f_texcoord) * menuOpacity + texture(gameview, f_texcoord) * (1.0 - menuOpacity);
}
##I
in_loc
in_normal
in_tex0
in_arg

##V
#version 330
in vec3 in_loc;
in vec3 in_normal;
in ivec2 in_tex0;
in ivec3 in_arg;
uniform mat4 model_mvp;
out vec2 f_texcoord;

void main()
{
   gl_Position = model_mvp * vec4(in_loc, 1);
   f_texcoord = in_tex0 / 65535.0;
}
##F
#version 330
uniform sampler2D tex;
uniform float progress;

in vec2 f_texcoord;

void main (void)
{
    if (f_texcoord.x > progress / 100.0) discard;
    gl_FragData[0] = texture(tex, f_texcoord);
}
##I
in_loc
in_normal
in_tex0
in_arg

##V
#version 330
in vec3 in_loc;
in vec3 in_normal;
in vec2 in_tex0;
in ivec3 in_arg;
out vec2 f_texcoord;
uniform vec2 xy;
uniform vec2 wh;

void main()
{
   vec2 loc = (in_loc.xy + vec2(1,1)) / 2 * wh + xy;
   gl_Position = vec4(loc * 2 - vec2(1.0, 1.0), 0, 1);
   f_texcoord = in_tex0 * wh + xy;
}
##I
in_loc
in_normal
in_tex0
in_arg
##F
#version 330
uniform sampler2D in_height;
uniform sampler2D in_flow;
uniform vec2 onepixel;
in vec2 f_texcoord;

float getTotalHeight(vec2 coord) {
    vec4 height = texture(in_height, coord);
    return height.x + height.y;
}

vec4 getTotalNeigh(vec2 coord) {
    return vec4(getTotalHeight(coord - vec2(onepixel.x, 0)),
                getTotalHeight(coord + vec2(onepixel.x, 0)),
                getTotalHeight(coord - vec2(0, onepixel.y)),
                getTotalHeight(coord + vec2(0, onepixel.y)));
}

float getinflow(vec2 coord) {
    vec4 flow_l = texture(in_flow, coord + vec2(onepixel.x, 0));
    vec4 flow_r = texture(in_flow, coord - vec2(onepixel.x, 0));
    vec4 flow_u = texture(in_flow, coord + vec2(0, onepixel.y));
    vec4 flow_d = texture(in_flow, coord - vec2(0, onepixel.y));
    return flow_l.y + flow_r.x + flow_u.w + flow_d.z;
}

void main (void)
{
    vec4 old_inheight = texture(in_height, f_texcoord);
    float old_height = old_inheight.x + old_inheight.y;
    vec4 old_flows = texture(in_flow, f_texcoord);
    float curheight = old_height + getinflow(f_texcoord) + dot(old_flows, vec4(1,1,1,1));

    vec4 heightdiff = vec4(old_height, old_height, old_height, old_height) - getTotalNeigh(f_texcoord);
    
//    float val = getTerrainVal(f_texcoord.x * 20000, f_texcoord.y * 20000) + 1;
    gl_FragData[0] = vec4(old_inheight.x, old_inheight.y, 0, 0); // solidheight, water
    gl_FragData[1] = vec4(0, 0, 0, 0);
}


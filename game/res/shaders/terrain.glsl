##V
#version 330
in vec3 in_loc;
in vec3 in_normal;
in ivec2 in_tex0;
in ivec3 in_arg;
out vec2 f_texcoord;
uniform vec2 xy;
uniform vec2 wh;

void main()
{
   vec2 loc = (in_loc.xy + vec2(1,1)) / 2 * wh + xy;
   gl_Position = vec4(loc * 2 - vec2(1.0, 1.0), 0, 1);
   f_texcoord = in_tex0 / 65535 * wh + xy;
}
##I
in_loc
in_normal
in_tex0
in_arg
##F
#version 330
uniform int seed1, seed2, seed3, seed4, seed5, seed6, seed7, seed8;
uniform int levels;
uniform int setwater;

in vec2 f_texcoord;

float getInputValue(int x, int y, int sa, int sb) {
	return float(23985 + x*x*sa + y*y*sb + x*y*sa*sb) / 4294967296.0;
}

float interp(float t, vec4 p) {
	vec4 tv = vec4(1, t, t*t, t*t*t);
	mat4 catmullrom = mat4(0, 2, 0, 0, -1, 0, 1, 0, 2, -5, 4, -1, -1, 3, -3, 1);
	return dot(0.5 * tv, p * catmullrom);
}

float getLayerValue(float x, float y, int sa, int sb) {
	int bx = int(x) - 1;
	int by = int(y) - 1;
	float dx = x - int(x);
	float dy = y - int(y);
	vec4 v1 = vec4(getInputValue(bx, by, sa, sb),
	               getInputValue(bx+1, by, sa, sb),
	               getInputValue(bx+2, by, sa, sb),
	               getInputValue(bx+3, by, sa, sb));
	vec4 v2 = vec4(getInputValue(bx, by+1, sa, sb),
	               getInputValue(bx+1, by+1, sa, sb),
	               getInputValue(bx+2, by+1, sa, sb),
	               getInputValue(bx+3, by+1, sa, sb));
	vec4 v3 = vec4(getInputValue(bx, by+2, sa, sb),
	               getInputValue(bx+1, by+2, sa, sb),
	               getInputValue(bx+2, by+2, sa, sb),
	               getInputValue(bx+3, by+2, sa, sb));
	vec4 v4 = vec4(getInputValue(bx, by+3, sa, sb),
	               getInputValue(bx+1, by+3, sa, sb),
	               getInputValue(bx+2, by+3, sa, sb),
	               getInputValue(bx+3, by+3, sa, sb));
	vec4 v = vec4(interp(dx, v1), interp(dx, v2), interp(dx, v3), interp(dx, v4));
	return interp(dy, v);
}

float getValue(float x, float y, int sa, int sb) {
	float total = 0, factor = 1.0 / (1 << levels);
	float scale = 2000.0 / (1 << levels);
	for (int i = 0; i < levels; i++) {
		total += getLayerValue(x / scale, y / scale, sa, sb) * factor;
		factor *= 2;
		scale *= 2;
	}
	return total;
}

float getTerrainVal(float i, float j) {
	float smoothv = getValue(i, j, seed1, seed2) - 0.5;
	float whichv = getValue(i, j, seed3, seed4) * 2 - 1;
	float p1 = 1.0 - abs(getValue(i, j, seed5, seed6) - 0.5f) * 2,
		   p2 = 1.0 - abs(getValue(i, j, seed7, seed8) - 0.5f) * 2;
	float pt = (p1*p1*p1 + p2*p2*p2)/2 - 0.5;
	float ratio = cos(abs(whichv) * 3.1415926/2);
	float constval;
	if (whichv > 0) {
		constval = pt;
	} else {
		constval = -pt;
	}
	return (1 - ratio) * constval + ratio * smoothv;
}

void main (void)
{
    float val = getTerrainVal(f_texcoord.x * 20000, f_texcoord.y * 20000) + 1;
//    float val = getValue(f_texcoord.x * 20000, f_texcoord.y * 20000, seed1, seed2);
    gl_FragData[0] = vec4(val, setwater, 0, 0); // solidheight, water, water velocity (xy)
    gl_FragData[1] = vec4(0, 0, 0, 0); // sandheight, regolith-height, sediment
}
/*
rockheight1, 2, 3, 4
waterheight
sediment
water velocity(xy)
regolith-height
outflows
*/

#include "WireframeWithRaster.h"
#include "Scene.h"
#include "Model.h"
#include "Matrix.h"

static const char *regularVS = 
"#version 330\n"
"uniform mat4 mat_vp;\n"
"uniform mat4 mat_model[20];\n"
"in vec3 in_loc;\n"
"in vec2 in_tex0;\n"
"in ivec3 in_arg;\n"
"out vec2 texcoord;\n"
"\n"
"void main()\n"
"{\n"
"   texcoord = in_tex0;\n"
"   gl_Position = mat_vp * mat_model[in_arg.x] * vec4(in_loc, 1);\n"
"}\n",
*rasterFS = 
"#version 330\n"
"\n"
"in vec2 texcoord;\n"
"void main (void)\n"
"{\n"
"    ivec2 pos1 = ivec2(vec2(131072, 131072) + texcoord + (-dFdy(texcoord) + -dFdy(texcoord)) / 2);\n"
"    ivec2 pos2 = ivec2(vec2(131072, 131072) + texcoord + (-dFdy(texcoord) +  dFdy(texcoord)) / 2);\n"
"    ivec2 pos3 = ivec2(vec2(131072, 131072) + texcoord + ( dFdx(texcoord) + -dFdx(texcoord)) / 2);\n"
"    ivec2 pos4 = ivec2(vec2(131072, 131072) + texcoord + ( dFdx(texcoord) +  dFdx(texcoord)) / 2);\n"
"    ivec2 posmin = ivec2(min(min(pos1.x, pos2.x), min(pos3.x, pos4.x)),\n"
"						  min(min(pos1.y, pos2.y), min(pos3.y, pos4.y)));\n"
"    ivec2 posmax = ivec2(max(max(pos1.x, pos2.x), max(pos3.x, pos4.x)),\n"
"						  max(max(pos1.y, pos2.y), max(pos3.y, pos4.y)));\n"
"    \n"
"    if (posmin.x != posmax.x ||\n"
"        posmin.y != posmax.y)\n"
"    {\n"
"    	if (mod(posmin.y, 128) - mod(posmax.y, 128) == 127 ||\n"
"    		mod(posmin.x, 128) - mod(posmax.x, 128) == 127)\n"
"    		gl_FragData[0] = vec4(1, 1, 1, 1);\n"
"    	else if (mod(posmin.y, 16) - mod(posmax.y, 16) == 15 ||\n"
"    		mod(posmin.x, 16) - mod(posmax.x, 16) == 15)\n"
"    		gl_FragData[0] = vec4(0.6, 0.6, 0.6, 1);\n"
"    	else if (mod(posmin.y, 8) - mod(posmax.y, 8) == 7 ||\n"
"    	         mod(posmin.x, 8) - mod(posmax.x, 8) == 7)\n"
"    		gl_FragData[0] = vec4(0.3, 0.3, 0.3, 1);\n"
"    	else \n"
"    		gl_FragData[0] = vec4(0.1, 0.1, 0.1, 1);\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n",
*wireframeFS = 
"#version 330\n"
"uniform int selected;\n"
"uniform int obj_physics;\n"
"uniform int obj_mass;\n"
"\n"
"void main (void)\n"
"{\n"
"    gl_FragData[0] = vec4(obj_physics, selected, obj_mass, 1);\n"
"}",
*objIdFs = 
"#version 330\n"
"uniform int objectId;\n"
"\n"
"void main (void)\n"
"{\n"
"    ivec4 vars = ivec4(mod(objectId / 65536.0, 256),  mod(objectId / 256.0, 256), mod(objectId, 256), 255);\n"
"    gl_FragData[0] = vec4(vars) / 255.0;\n"
"}\n";

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

WireframeWithRaster::WireframeWithRaster(Renderable &scene, RenderTarget &finalTarget, Side side)
: scale(100.0)
, scene(scene)
, target(finalTarget)
, raster(ShaderProgram(regularVS, 0, rasterFS, invecs), Model::Raster(), target, view)
, objects(ShaderProgram(regularVS, 0, wireframeFS, invecs), scene, target, view)
, obj_ids(ShaderProgram(regularVS, 0, objIdFs, invecs), scene, target, view)
{
	switch(side)
	{
	default:
	case Left:
		view.rotation = Quaternion(90, 0, 0);
		break;
	case Top:
		view.rotation = Quaternion(180, 0, 270);
		break;
	case Front:
		view.rotation = Quaternion(180, 0, 0);
		break;
	}
}

void WireframeWithRaster::DoRun()
{
	view.projection = Matrix(1.33f, 1, 10000, 1/scale);
	view.location = location;
	raster.program.Set("mat_model[0]", Matrix(Vector3(0,0,0), view.rotation * Quaternion(0, 0, 90)));
	raster.Run();
	target.ClearDepth();
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	objects.Run();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
}

void WireframeWithRaster::DrawPick(std::vector<Object *> &objs)
{
	obj_ids.DrawPick(objs);
}


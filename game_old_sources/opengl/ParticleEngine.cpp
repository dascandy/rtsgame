#include "ParticleEngine.h"
#include "Scene.h"
#include <math.h>
#include "Model.h"
#include "debug.h"
#include "profile.h"
#include <stdlib.h>
#include <string.h>

static const char *particleVshader = 
"#version 330\n"
"in vec3 in_loc;\n"
"in vec3 in_dir;\n"
"in vec4 in_vars;\n"
"out vec3 ilocation;\n"
"out vec3 idirection;\n"
"out vec4 ivars;\n"
"\n"
"void main()\n"
"{\n"
"	ilocation = in_loc;\n"
"   idirection = in_dir;\n"
"	ivars = in_vars;\n"
"}\n";

static const char *particleGshader = 
"#version 330\n"
"layout(points) in;\n"
"layout(points, max_vertices=1) out;\n"
"\n"
"uniform sampler3D space;\n"
"\n"
"in vec3 ilocation[];\n"
"in vec3 idirection[];\n"
"in vec4 ivars[];\n"
"out vec3 location;\n"
"out vec3 direction;\n"
"out vec4 vars;\n"
"\n"
"void main()\n"
"{\n"
"   if (ivars[0].x < 1) return;\n" // too cold
"   if (ivars[0].y < 1) return;\n" // end of life
"	direction = idirection[0] * 0.98 + vec3(0, ivars[0].x * 0.001, 0);\n"// + texture(space, ilocation[0] / 2 + vec3(0.5,0.5,0.5)).xyz;\n"
"	vars = ivars[0] + vec4(-0.005*ivars[0].x, -1, length(idirection[0]) / 100, 0);\n"
"	location = ilocation[0] + idirection[0];\n"
"   gl_Position = vec4(location, 1);\n"
"	EmitVertex();\n"
"	EndPrimitive();\n"
"}\n";

static const char *vboVshader = 
"#version 330\n"
"in vec3 in_loc;\n"
"in vec3 in_dir;\n"
"in vec4 in_vars;\n"
"out vec3 location;\n"
"out vec4 vars;\n"
"\n"
"void main()\n"
"{\n"
"   location = in_loc;\n"
"   vars = in_vars;\n"
"}\n",
*vboGshader =
"#version 330\n"
"layout(points) in;\n"
"layout(points, max_vertices=1) out;\n"
"in vec3 location[];\n"
"in vec4 vars[];\n"
"out vec4 ovars;\n"
"\n"
"void main()\n"
"{\n"
"   gl_Layer = int(location[0].z * 32);\n"
"   gl_Position = vec4(location[0].xy, 0, 1);\n"
"   ovars = vars[0];\n"
"	EmitVertex();\n"
"	EndPrimitive();\n"
"}\n",
*vboFshader =
"#version 330\n"
"in vec4 ovars;\n"
"void main (void)\n"
"{\n"
"    gl_FragData[0] = vec4(ovars.wx, 0, 0);\n"
"}";



static const char *threedVshader = 
"#version 330\n"
"in vec3 in_loc;\n"
"out vec3 location;\n"
"out vec3 texcoord;\n"
"\n"
"void main()\n"
"{\n"
"   location = in_loc;\n"
"   texcoord = (in_loc + vec3(1,1,1)) / 2;\n"
"}\n",
*threedGshader =
"#version 330\n"
"layout(triangles) in;\n"
"layout(triangle_strip, max_vertices=96) out;\n"
"in vec3 location[];\n"
"in vec3 texcoord[];\n"
"out vec4 olocs;\n"
"\n"
"void main()\n"
"{\n"
"   for (int i = 0; i < 32; i++) {\n"
"      gl_Layer = i;\n"
"      gl_Position = vec4(location[0].xy, i/32.0, 1);\n"
"      olocs = vec4(texcoord[0],0);\n"
"	   EmitVertex();\n"
"      gl_Layer = i;\n"
"      gl_Position = vec4(location[1].xy, i/32.0, 1);\n"
"      olocs = vec4(texcoord[1],0);\n"
"	   EmitVertex();\n"
"      gl_Layer = i;\n"
"      gl_Position = vec4(location[2].xy, i/32.0, 1);\n"
"      olocs = vec4(texcoord[2],0);\n"
"	   EmitVertex();\n"
"	   EndPrimitive();\n"
"   }\n"
"}\n",
*threedFshader =
"#version 330\n"
"in vec4 olocs;\n"
"uniform sampler3D vbo;\n"
"uniform sampler3D last;\n"
"uniform sampler3D scene;\n"
"\n"
"vec3 GetDelta(vec3 delta)\n"
"{\n"
"	return delta * (texture(vbo, olocs.xyz).x - texture(vbo, olocs.xyz + delta * vec3(1.0/128.0, 1.0/128.0, 1.0/32.0)).x);\n"
"}\n"
"\n"
"void main (void)\n"
"{\n"
"	vec3 sum = vec3(0,0,0);\n"
"	sum += GetDelta(vec3(-1, -1, -1));\n"
"	sum += GetDelta(vec3( 0, -1, -1));\n"
"	sum += GetDelta(vec3( 1, -1, -1));\n"
"	sum += GetDelta(vec3(-1,  0, -1));\n"
"	sum += GetDelta(vec3( 0,  0, -1));\n"
"	sum += GetDelta(vec3( 1,  0, -1));\n"
"	sum += GetDelta(vec3(-1,  1, -1));\n"
"	sum += GetDelta(vec3( 0,  1, -1));\n"
"	sum += GetDelta(vec3( 1,  1, -1));\n"
"	sum += GetDelta(vec3(-1, -1, 0));\n"
"	sum += GetDelta(vec3( 0, -1, 0));\n"
"	sum += GetDelta(vec3( 1, -1, 0));\n"
"	sum += GetDelta(vec3(-1,  0, 0));\n"
"	sum += GetDelta(vec3( 1,  0, 0));\n"
"	sum += GetDelta(vec3(-1,  1, 0));\n"
"	sum += GetDelta(vec3( 0,  1, 0));\n"
"	sum += GetDelta(vec3( 1,  1, 0));\n"
"	sum += GetDelta(vec3(-1, -1, 1));\n"
"	sum += GetDelta(vec3( 0, -1, 1));\n"
"	sum += GetDelta(vec3( 1, -1, 1));\n"
"	sum += GetDelta(vec3(-1,  0, 1));\n"
"	sum += GetDelta(vec3( 0,  0, 1));\n"
"	sum += GetDelta(vec3( 1,  0, 1));\n"
"	sum += GetDelta(vec3(-1,  1, 1));\n"
"	sum += GetDelta(vec3( 0,  1, 1));\n"
"	sum += GetDelta(vec3( 1,  1, 1));\n"
"   sum /= 5000000;\n"
"   vec3 lastSample = texture(last, olocs.xyz).xyz;\n"
"	gl_FragData[0] = vec4(lastSample * 0.97 + sum * 0.03, 0);\n"
"}\n";




static const VertexBuffer::reference particleOffsets[] = 
{
	0, 3, GL_FLOAT,
	12, 3, GL_FLOAT,
	24, 4, GL_FLOAT,
	0, 0, 0
};

static const char *invars[] = 
{
	"in_loc",
	"in_dir",
	"in_vars",
	0
};

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

static const char *varyings[] = 
{
	"location",
	"direction",
	"vars",
	0
};

static const int format = GL_RGBA32F;

ParticleEngine::ParticleEngine(Scene &scene)
: scene(scene)
, centerLocation(70, 8, 70)
, sceneout(128, 128, 32, format)
, vboout(128, 128, 32, format)
, i1(128, 128, 32, format)
, i2(128, 128, 32, format)
, in(particleOffsets, 524288, sizeof(Particle), GL_POINTS)
, out(particleOffsets, 524288, sizeof(Particle), GL_POINTS)
, threed_vbo(128, 128, false)
, threed_render(128, 128, false)
, threed_scene(128, 128, false)
, vboRender(ShaderProgram(vboVshader, vboGshader, vboFshader, invars), in, threed_vbo, localview)
//, sceneRender(ShaderProgram(0, 0, 0, invecs), scene, threed_scene, localview)
, threedrender(ShaderProgram(threedVshader, threedGshader, threedFshader, invecs), Model::Square(), threed_render, localview)
, vboTransform(ShaderProgram(particleVshader, particleGshader, 0, invars, varyings), in, out, GL_POINTS)
{
	sceneout.Reload();
	vboout.Reload();
	i1.Reload();
	i2.Reload();

	threed_vbo.AddTarget(vboout);
	threed_vbo.blendmode = RenderTarget::Additive;
	threed_scene.AddTarget(sceneout);
	threed_render.AddTarget(i1);
	threedrender.AddTexture("vbo", vboout);
	threedrender.AddTexture("last", i2);
	threedrender.AddTexture("scene", sceneout);
	vboTransform.AddTexture("space", i1);

	// HACK: need some way to initialize some textures with zeroes
	glBindTexture(GL_TEXTURE_3D, i2.textureId);
	char *buf = (char *)malloc(128*128*32*4);
	memset(buf, 0, 128*128*32*4);
	glTexImage3D(GL_TEXTURE_3D, 0, format, 128, 128, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	glBindTexture(GL_TEXTURE_3D, 0);
	free(buf);
	TODO_W("Fix rendering to properly render depths");
}

void ParticleEngine::Update()
{
//	vboRender.Run();
//	sceneRender.Run();
//	threedrender.Run();
	vboTransform.Run();
//	i1.swap(i2);
}

void ParticleEngine::AddParticles(Particle *p, int count)
{
	out.Add(p, count);
	in.Add(p, count);
}

void ParticleEngine::SetCenterPosition(const Vector3 &)
{
	TODO("Implement particle engine center position setting");
}

void ParticleEngine::Draw(ShaderProgram &sp)
{
	out.Draw(sp);
}


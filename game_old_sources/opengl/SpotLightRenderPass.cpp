#include "SpotLightRenderPass.h"
#include "Scene.h"
#include "Model.h"
#include "debug.h"

static const char *noneVS =
"#version 330\n"
"in vec3 in_loc;\n"
"out vec2 location;\n"
"\n"
"void main()\n"
"{\n"
"   location = (in_loc.xy + vec2(1,1)) / 2;\n"
"	gl_Position = vec4(in_loc, 1);\n"
"}\n",
*pointLightFS =
"#version 330\n"
"uniform sampler2D depth;\n"
"uniform sampler2D normal;\n"
"uniform sampler2D color;\n"
"\n"
"uniform sampler2D shadowmap;\n"
"\n"
"uniform sampler2D lighttexture;\n"
"uniform vec3 lightpos;\n"
"uniform mat4 mat_view_light;\n"
"uniform float lightangle;\n"
"uniform float lightbrightness;\n"
"\n"
"in vec2 location;\n"
#include "funcs.glsl"
#include "worldpos.glsl"
"void main (void)\n"
"{\n"
"    vec3 c = texture(color, location).rgb;\n"
"    vec4 norm = texture(normal, location);\n"
"    vec3 n = norm.xyz * 2 - vec3(1, 1, 1);\n"
"    vec3 position = texture(depth, location).xyz;\n"
"    if (position == vec3(0,0,0)) discard;\n"
"\n"
"    vec3 lightdir = lightpos - position;\n"
"    vec4 position_for_light = paraboloid(mat_view_light * vec4(position, 1));\n"
"    float L = abs(position_for_light.w);\n"
"    if (position_for_light.z < cos(lightangle/2)) discard;\n"
"    position_for_light /= sin(lightangle/2);\n"
"    vec2 lightpos = ivec2(position_for_light.xy * 512) / 512.0;\n"
"    vec2 ld = (position_for_light.xy - lightpos) * 512;\n"
"    vec4 moments = texture(shadowmap, position_for_light.xy * 0.5 + vec2(0.5, 0.5));\n"
"\n"
"    float lit_factor = (L <= moments.x ? 1.0 : 0.0);\n"
"    float e_x2 = moments.y;\n"
"    float ex_2 = moments.x * moments.x;\n"
"    float variance = min(max(e_x2 - ex_2, 0.0) + 0.01, 1.0);\n"
"    float m_d = moments.x - L;\n"
"    float p = variance / (variance + m_d * m_d);\n"
"    float lightness = max(lit_factor, p);\n"
"\n"
"    float qattn = dot(lightdir, lightdir);\n"
"    float lattn = length(lightdir);\n"
"    float cosLightAngle = dot(lightdir / lattn, n);\n"
"    vec3 lightValue = lightbrightness * texture(lighttexture, position_for_light.xy * 0.5 + vec2(0.5, 0.5)).rgb * lightness * 200 / qattn;\n"
"    vec3 diffuseC = lightValue * c * cosLightAngle;\n"
"    \n"
"    vec3 eyedir = worldPosition(location, 0) - position;\n"
"    vec3 halfAngle = normalize(eyedir + lightdir);\n"
"    float specular = dot(n, halfAngle);\n"
"    vec3 specularC = lightValue * pow(specular, 512*(norm.w + 40));\n"
"    \n"
"    gl_FragData[0] = vec4(specularC + diffuseC, 1);\n"
"}",
*projVSWithTex = 
"#version 330\n"
"uniform mat4 mat_view;\n"
"uniform mat4 mat_model[20];\n"
"uniform float lightangle;\n"
"in vec3 in_loc;\n"
"in vec2 in_tex0;\n"
"in ivec3 in_arg;\n"
"out vec4 f_location;\n"
"out vec2 f_texcoord;\n"
"out float depth;\n"
"out float direction;\n"
#include "funcs.glsl"
"\n"
"void main()\n"
"{\n"
"    f_location = mat_model[in_arg.x] * vec4(in_loc, 1);\n"
"    f_texcoord = in_tex0;\n"
"    vec4 position_for_light = paraboloid(mat_view * mat_model[in_arg.x] * vec4(in_loc, 1));\n"
"    depth = position_for_light.w;\n"
"    direction = position_for_light.z;\n"
"    position_for_light /= sin(lightangle/2);\n"
"    position_for_light.z = (position_for_light.w) / (999);\n"
"    gl_Position = vec4(position_for_light.xyz, 1);\n"
"}\n",
*renderDepthWithEffect = 
"#version 330\n"
"in vec2 f_texcoord;\n"
"in vec4 f_location;\n"
"in float depth;\n"
"in float direction;\n"
"uniform sampler2D modeltex;\n"
"uniform sampler2D perlin;\n"
"uniform sampler2D random;\n"
"uniform uint time;\n"
"void main (void)\n"
"{\n"
"    if (direction < 0) discard;\n"
"    gl_FragData[0] = vec4(depth, depth * depth, 0, 0);\n"
"}",
*projVS = 
"#version 330\n"
"uniform mat4 mat_view;\n"
"uniform mat4 mat_model[20];\n"
"uniform float lightangle;\n"
"in vec3 in_loc;\n"
"in ivec3 in_arg;\n"
"out float depth;\n"
"out float direction;\n"
#include "funcs.glsl"
"\n"
"void main()\n"
"{\n"
"    vec4 position_for_light = paraboloid(mat_view * mat_model[in_arg.x] * vec4(in_loc, 1));\n"
"    depth = position_for_light.w;\n"
"    direction = position_for_light.z;\n"
"    gl_Position = vec4(position_for_light.xy / sin(lightangle / 2), position_for_light.w / 999, 1);\n"
"}\n",
*renderDepthWithoutEffect = 
"#version 330\n"
"in float depth;\n"
"in float direction;\n"
"void main (void)\n"
"{\n"
"    if (direction < 0) discard;\n"
"    gl_FragData[0] = vec4(depth, depth * depth, 0, 0);\n"
"}",
*copyIn = 
"#version 330\n"
"uniform sampler2D indata;\n"
"in vec2 location;\n"
"void main (void)\n"
"{\n"
"    gl_FragData[0] = texture2D(indata, location);\n"
"}";

static const char *particleVS = 
"#version 330\n"
"in vec3 in_loc;\n"
"in vec4 in_vars;\n"
"out vec4 location;\n"
"out vec4 vars;\n"
"\n"
"void main()\n"
"{\n"
"	location = vec4(in_loc, 1);\n"
"	vars = in_vars;\n"
"}\n",
*particleGS =
"#version 330\n"
"layout(points) in;\n"
"layout(triangle_strip, max_vertices=6) out;\n"
"\n"
"uniform mat4 mat_view;\n"
"uniform mat4 mat_iv;\n"
"uniform float lightangle;\n"
#include "funcs.glsl"
"\n"
"in vec4 location[];\n"
"in vec4 vars[];\n"
"out vec4 olocation;\n"
"out vec2 texloc;\n"
"out float direction;\n"
"\n"
"void make_vertex(float dx, float dy) \n"
"{\n"
"   vec4 objlocation = location[0] + vec4(mat3(mat_iv) * vec3( dx * vars[0].z * (vars[0].x-1), dy * vars[0].z * (vars[0].x-1), 0), 0);\n"
"   olocation = objlocation;\n"
"   vec4 transloc = paraboloid(mat_view * objlocation);\n"
"   direction = transloc.z;\n"
"   gl_Position = vec4(transloc.xy / sin(lightangle / 2), transloc.w / 999, 1);\n"
"   texloc = vec2(dx, dy);\n"
"	EmitVertex();\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"   make_vertex(-1, -1);\n"
"   make_vertex(-1,  1);\n"
"   make_vertex( 1,  1);\n"
"	EndPrimitive();\n"
"   make_vertex( 1,  1);\n"
"   make_vertex( 1, -1);\n"
"   make_vertex(-1, -1);\n"
"	EndPrimitive();\n"
"}\n",
*particleFS =
"#version 330\n"
"uniform vec3 lightpos;\n"
"in vec4 olocation;\n"
"in vec2 texloc;\n"
"in float direction;\n"
"void main (void)\n"
"{\n"
"    float len = length(texloc);\n"
"    if (len > 1) discard;\n"
"    if (direction < 0) discard;\n"
"    float depth = length(olocation.xyz - lightpos.xyz);\n"
"    gl_FragData[0] = vec4(depth, depth * depth, 0, 0);\n"
"}";

#define USE_BLUR5

static const char *blurH = 
"#version 330\n"
"uniform sampler2D indata1;\n"
"uniform sampler2D indata2;\n"
"in vec2 location;\n"
"\n"
"vec4 sample(vec2 loc) {\n"
"    vec4 in1 = texture(indata1, loc);\n"
"    vec4 in2 = texture(indata2, loc);\n"
"    return (in1.x > 0 && in1.x < in2.x) ? in1 : in2;\n"
"}\n"
"\n"
"void main (void)\n"
"{\n"
#if defined(USE_BLUR9)
"    gl_FragData[0] = 0.004 * sample(location + vec2(-4.0 / 256, 0)) + \n"
"                     0.031 * sample(location + vec2(-3.0 / 256, 0)) + \n"
"                     0.109 * sample(location + vec2(-2.0 / 256, 0)) + \n"
"                     0.219 * sample(location + vec2(-1.0 / 256, 0)) + \n"
"                     0.274 * sample(location + vec2( 0.0 / 256, 0)) + \n"
"                     0.219 * sample(location + vec2( 1.0 / 256, 0)) + \n"
"                     0.109 * sample(location + vec2( 2.0 / 256, 0)) + \n"
"                     0.031 * sample(location + vec2( 3.0 / 256, 0)) + \n"
"                     0.004 * sample(location + vec2( 4.0 / 256, 0));\n"
#elif defined(USE_BLUR5)
"    gl_FragData[0] = 0.063 * sample(location + vec2(-2.0 / 256, 0)) + \n"
"                     0.245 * sample(location + vec2(-1.0 / 256, 0)) + \n"
"                     0.384 * sample(location + vec2( 0.0 / 256, 0)) + \n"
"                     0.245 * sample(location + vec2( 1.0 / 256, 0)) + \n"
"                     0.063 * sample(location + vec2( 2.0 / 256, 0));\n"
#else
"    gl_FragData[0] = sample(location);\n"
#endif
"}\n",
*blurV = 
"#version 330\n"
"uniform sampler2D indata;\n"
"in vec2 location;\n"
"\n"
"void main (void)\n"
"{\n"
#if defined(USE_BLUR9)
"    gl_FragData[0] = 0.004 * texture(indata, location + vec2(0, -4.0 / 256)) + \n"
"                     0.031 * texture(indata, location + vec2(0, -3.0 / 256)) + \n"
"                     0.109 * texture(indata, location + vec2(0, -2.0 / 256)) + \n"
"                     0.219 * texture(indata, location + vec2(0, -1.0 / 256)) + \n"
"                     0.274 * texture(indata, location + vec2(0,  0.0 / 256)) + \n"
"                     0.219 * texture(indata, location + vec2(0,  1.0 / 256)) + \n"
"                     0.109 * texture(indata, location + vec2(0,  2.0 / 256)) + \n"
"                     0.031 * texture(indata, location + vec2(0,  3.0 / 256)) + \n"
"                     0.004 * texture(indata, location + vec2(0,  4.0 / 256));\n"
#elif defined(USE_BLUR5)
"    gl_FragData[0] = 0.063 * texture(indata, location + vec2(0, -2.0 / 256)) + \n"
"                     0.245 * texture(indata, location + vec2(0, -1.0 / 256)) + \n"
"                     0.384 * texture(indata, location + vec2(0,  0.0 / 256)) + \n"
"                     0.245 * texture(indata, location + vec2(0,  1.0 / 256)) + \n"
"                     0.063 * texture(indata, location + vec2(0,  2.0 / 256));\n"
#else
"    gl_FragData[0] = texture(indata, location);\n"
#endif
"}\n";

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

static const char *invars[] = 
{
	"in_loc",
	"in_dir",
	"in_vars",
	0
};

static const int smapsize = 256;

SpotLightRenderPass::SpotLightRenderPass(Scene &scene, Texture &depth, Texture &normals, Texture &color, Texture *perlin, Texture *random, SpotLight &light, RenderView &view, RenderTarget &output, bool shadowCasting, bool dynamicObject)
: depth(depth)
, normals(normals)
, color(color)
, perlin(perlin)
, random(random)
, view(view)
, output(output)
, light(light)
, onlyStatic(scene)
, onlyDynamic(scene)
, onlyParticles(scene)
, render(ShaderProgram(noneVS, 0, pointLightFS, invecs), Model::Square(), output, view)
, renderStaticT(smapsize, smapsize, true)
, renderDynamicT(smapsize, smapsize, true)
, renderBlur1T(smapsize, smapsize, false)
, renderBlur2T(smapsize, smapsize, false)
, renderStatic(ShaderProgram(projVS, 0, renderDepthWithoutEffect, invecs), onlyStatic, dynamicObject ? renderDynamicT : renderStaticT, light.view)
, renderDynamic(ShaderProgram(projVS, 0, renderDepthWithoutEffect, invecs), onlyDynamic, renderDynamicT, light.view)
, renderParticles(ShaderProgram(particleVS, particleGS, particleFS, invars), onlyParticles, renderDynamicT, light.view)
, renderBlur1(ShaderProgram(noneVS, 0, blurH, invecs), Model::Square(), renderBlur1T, noneView)
, renderBlur2(ShaderProgram(noneVS, 0, blurV, invecs), Model::Square(), renderBlur2T, noneView)
, smap_static(smapsize, smapsize, GL_RGBA32F)
, smap_dynamic(smapsize, smapsize, GL_RGBA32F)
, smap_blur1(smapsize, smapsize, GL_RGBA32F)
, smap_blur2(smapsize, smapsize, GL_RGBA32F)
, dynamicObject(dynamicObject)
, shadowCasting(shadowCasting)
, staticDone(false)
{
	renderStaticT.AddTarget(smap_static);

	renderDynamicT.AddTarget(smap_dynamic);

	renderBlur1.AddTexture("indata1", smap_dynamic);
	renderBlur1.AddTexture("indata2", smap_static);
	renderBlur1T.AddTarget(smap_blur1);

	renderBlur2.AddTexture("indata", smap_blur1);
	renderBlur2T.AddTarget(smap_blur2);

	render.AddTexture("depth", depth);
	render.AddTexture("normal", normals);
	render.AddTexture("color", color);
	render.AddTexture("lighttexture", *light.lighttexture);
	render.AddTexture("shadowmap", smap_blur2);
}

void SpotLightRenderPass::DoRun()
{
	switch(step)
	{
	case 1:
		renderDynamicT.Clear();
		renderBlur1T.Clear();
		renderBlur2T.Clear();
		if (!shadowCasting) return;
		if (dynamicObject) return;
		if (staticDone) return;
		renderStaticT.Clear();
		renderStatic.program.Set("lightangle", light.angle / 57.17f);
		renderStatic.Run();
		staticDone = true;
		break;
	case 2:
		if (!shadowCasting) return;
		if (dynamicObject)
		{
			renderStatic.program.Set("lightangle", light.angle / 57.17f);
			renderStatic.Run();
		}
		TODO_W("There's still a bug hiding in this winding oops");
		renderDynamic.program.Set("lightangle", light.angle / 57.17f);
		renderDynamic.Run();
		renderParticles.program.Set("lightangle", light.angle / 57.17f);
		renderParticles.program.Set("lightpos", light.view.location);
		renderParticles.Run();
		break;
	case 3:
		renderBlur1.Run();
		break;
	case 4:
		renderBlur2.Run();
		break;
	case 5:
		light.view.Set(render.program, "_light");
		render.program.Set("lightpos", light.view.location);
		render.program.Set("lightangle", light.angle / 57.17f);
		render.program.Set("lightbrightness", light.brightness);
		render.Run();
		break;
	}
}


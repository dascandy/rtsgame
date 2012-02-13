#include "DeferredRenderEngine.h"
#include "SpotLightRenderPass.h"
#include "PointLightRenderPass.h"
#include "Scene.h"
#include "Model.h"
#include <vector>
#include "SpotLight.h"
#include "PointLight.h"
#include "debug.h"
#include "ParticleEngine.h"
#include "TextureDeserializer.h"
#include "GameState.h"
#include "profile.h"

static const char *initialVS = 
"#version 330\n"
"uniform mat4 mat_model[20];\n"
"uniform mat4 mat_vp;\n"
"in vec3 in_loc;\n"
"in vec3 in_normal;\n"
"in vec2 in_tex0;\n"
"in ivec3 in_arg;\n"
"out vec4 g_location;\n"
"out vec3 g_normal;\n"
"out vec2 g_texcoord;\n"
"out vec4 g_loc;\n"
"\n"
"void main()\n"
"{\n"
"   vec4 loc = mat_vp * mat_model[in_arg.x] * vec4(in_loc, 1);\n"
"   g_location = mat_model[in_arg.x] * vec4(in_loc, 1);\n"
"   g_location /= g_location.w;\n"
"   g_loc = loc;\n"
"   g_normal = mat3(mat_model[in_arg.x]) * in_normal;\n"
"   g_texcoord = in_tex0;\n"
"}\n",
*initialGS =
"#version 330\n"
"layout(triangles) in;\n"
"layout(triangle_strip, max_vertices=3) out;\n"
"in vec4 g_location[];\n"
"in vec4 g_loc[];\n"
"in vec3 g_normal[];\n"
"in vec2 g_texcoord[];\n"
"out vec4 f_location;\n"
"out vec3 f_normal;\n"
"out vec3 f_tangent;\n"
"out vec3 f_bitangent;\n"
"out vec2 f_texcoord;\n"
"\n"
"void main()\n"
"{\n"
"    vec3 edge1 = g_location[1].xyz - g_location[0].xyz;\n"
"    vec3 edge2 = g_location[2].xyz - g_location[0].xyz;\n"
"    vec2 edge1uv = g_texcoord[1] - g_texcoord[0];\n"
"    vec2 edge2uv = g_texcoord[2] - g_texcoord[0];\n"
"    vec3 i_tangent = edge2 * edge1uv.y - edge1 * edge2uv.y;\n"
"    vec3 t_tangent = vec3(1, 0, 0);\n"
"    if (length(i_tangent) > 0.01)\n"
"        t_tangent = normalize(i_tangent);\n"
"\n"
"    f_location = g_location[0];\n"
"    f_normal = g_normal[0];\n"
"    f_texcoord = g_texcoord[0];\n"
"    gl_Position = g_loc[0];\n"
"    f_bitangent = cross(t_tangent, g_normal[0]);\n"
"    f_tangent = cross(g_normal[0], f_bitangent);\n"
"    EmitVertex();\n"
"\n"
"    f_location = g_location[1];\n"
"    f_normal = g_normal[1];\n"
"    f_texcoord = g_texcoord[1];\n"
"    gl_Position = g_loc[1];\n"
"    f_bitangent = cross(t_tangent, g_normal[1]);\n"
"    f_tangent = cross(g_normal[1], f_bitangent);\n"
"    EmitVertex();\n"
"\n"
"    f_location = g_location[2];\n"
"    f_normal = g_normal[2];\n"
"    f_texcoord = g_texcoord[2];\n"
"    gl_Position = g_loc[2];\n"
"    f_bitangent = cross(t_tangent, g_normal[2]);\n"
"    f_tangent = cross(g_normal[2], f_bitangent);\n"
"    EmitVertex();\n"
"\n"
"    EndPrimitive();\n"
"}\n",
*initialFS = 
"#version 330\n"
"uniform sampler2D modeltex;\n"
"uniform sampler2D speculartex;\n"
"uniform sampler2D normaltex;\n"
"uniform sampler2D emissivetex;\n"
"uniform sampler2D perlin;\n"
"uniform sampler2D random;\n"
"uniform uint time;\n"
"in vec4 f_location;\n"
"in vec3 f_normal;\n"
"in vec3 f_tangent;\n"
"in vec3 f_bitangent;\n"
"in vec2 f_texcoord;\n"
"void main (void)\n"
"{\n"
"    vec4 color = texture(modeltex, f_texcoord);\n"
"    vec3 norm = texture(normaltex, f_texcoord).xyz * 2 - vec3(1, 1, 1);\n"
"    vec3 emissive = texture(emissivetex, f_texcoord).rgb;\n"
"    mat3 m = mat3(f_tangent, f_bitangent, f_normal);\n"
"    vec3 r_norm = m * norm;\n"
"\n"
"    if (color.a == 0) discard;\n"
"    gl_FragData[0] = vec4(color.xyz, 0.0);\n"
"    gl_FragData[1] = vec4(f_location.xyz, 0);\n"
"    gl_FragData[2] = vec4(r_norm * 0.5 + vec3(0.5, 0.5, 0.5), texture(speculartex, f_texcoord).x);\n"
"    gl_FragData[3] = vec4(emissive, 1);\n"
"}";

static const char *ambientVS =
"#version 330\n"
"in vec3 in_loc;\n"
"in vec3 in_normal;\n"
"in vec2 in_tex0;\n"
"out vec2 location;\n"
"\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(in_loc, 1);\n"
"   location = in_tex0;\n"
"}\n",
*ambientFS = 
"#version 330\n"
"uniform sampler2D color;\n"
"uniform sampler2D depth;\n"
"uniform sampler2D normal;\n"
"uniform sampler2D other;\n"
"uniform sampler2D random;\n"
"\n"
"const vec3 ambientcolor = vec3(0.9, 0.9, 0.9);\n"
"in vec2 location;\n"
#include "funcs.glsl"
"\n"
"float sample(vec2 coords, vec3 basePosition, vec3 baseNormal)\n"
"{\n"
"    vec3 position = texture(depth, coords).xyz;\n"
"    if (position == vec3(0,0,0)) return 0.0;\n"
"\n"
"	vec3 displacement = basePosition - position;\n"
"	vec3 direction = displacement/length(displacement);\n"
"\n"
"	float occlusionAngle = dot(direction, baseNormal);\n"
"	return (occlusionAngle < 0.001) ? 0.0 : occlusionAngle;\n"
"}\n"
"\n"
"float fourSamples(vec2 baseCoords, vec2 baseOffset, vec3 basePosition, vec3 baseNormal)\n"
"{\n"
"	float result=0;\n"
"	result+=sample(baseCoords+vec2( baseOffset.x,  baseOffset.y), basePosition, baseNormal);\n"
"	result+=sample(baseCoords+vec2( baseOffset.y, -baseOffset.x), basePosition, baseNormal);\n"
"	result+=sample(baseCoords+vec2(-baseOffset.x, -baseOffset.y), basePosition, baseNormal);\n"
"	result+=sample(baseCoords+vec2(-baseOffset.y,  baseOffset.x), basePosition, baseNormal);\n"
"	return result;\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"    vec3 position = texture(depth, location).xyz;\n"
"    if (position == vec3(0,0,0)) discard;\n"
"    vec3 colorV = texture(color, location).rgb;\n"
"    vec3 normal = normalize(texture(normal, location).xyz);\n"
"    vec3 emissive = texture(other, location).rgb;\n"
"\n"
"    float n = texture(random, location.xy).r;\n"
"    vec2 randomDirection = vec2(cos(1.5707963 * n), sin(1.5707963 * n));\n"
"    vec2 axisOffset = randomDirection*0.01;\n"
"    vec2 angleOffset = vec2(axisOffset.x-axisOffset.y, axisOffset.x+axisOffset.y)*(sqrt(2.0)/2);\n"
"\n"
"    float occlusion = 0;\n"
"    occlusion+=fourSamples(location, angleOffset * 0.25, position, normal);\n"
"    occlusion+=fourSamples(location, axisOffset * 0.5, position, normal);\n"
"    occlusion+=fourSamples(location, angleOffset * 0.75, position, normal);\n"
"    occlusion+=fourSamples(location, axisOffset * 1.0, position, normal);\n"
"\n"
"    gl_FragData[0] = vec4((ambientcolor * colorV) * (1-clamp(1-occlusion/(occlusion+1), 0, 1)) + emissive, 1);\n"
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

DeferredRenderEngine::DeferredRenderEngine(Scene &scene, RenderTarget &finalTarget, RenderView &finalView)
: scene(scene)
, target(finalTarget)
, view(finalView)
, color(finalTarget.width, finalTarget.height, GL_RGBA32F)
, depth(finalTarget.width, finalTarget.height, GL_RGBA32F)
, normal(finalTarget.width, finalTarget.height, GL_RGBA32F)
, other(finalTarget.width, finalTarget.height, GL_RGBA32F)
, perlin(TextureDeserializer("perlin").deserialize())
, random(TextureDeserializer("random").deserialize())
, initialR(finalTarget.width, finalTarget.height, true)
, initial(ShaderProgram(initialVS, initialGS, initialFS, invecs), scene, initialR, view)
, ambient(ShaderProgram(ambientVS, 0, ambientFS, invecs), Model::Square(), target, nullView)
{
	initialR.AddTarget(color);
	initialR.AddTarget(depth);
	initialR.AddTarget(normal);
	initialR.AddTarget(other);
	initial.AddTexture("perlin", *perlin);
	initial.AddTexture("random", *random);

	ambient.AddTexture("color", color);
	ambient.AddTexture("depth", depth);
	ambient.AddTexture("normal", normal);
	ambient.AddTexture("other", other);
	ambient.AddTexture("random", *random);
}

void DeferredRenderEngine::DoRun()
{
	static std::map<SpotLight *, SpotLightRenderPass *> srenders;
	static std::map<PointLight *, PointLightRenderPass *> prenders;
	initialR.Clear();
	initial.Run();
	ambient.Run();
	target.blendmode = target.Additive;
	std::vector<PointLight *> pointlights;
	std::vector<SpotLight *> spotlights;
	scene.GetLights(GameState::Instance().gamePosition, pointlights, spotlights);
	glFrontFace(GL_CW);
	for (std::vector<SpotLight *>::iterator it = spotlights.begin(); it != spotlights.end(); ++it)
	{
		if (!srenders[*it])
			srenders[*it] = new SpotLightRenderPass(scene, depth, normal, color, perlin, random, **it, view, target, true, false);
		srenders[*it]->step = 1;
		srenders[*it]->Run();
	}
	for (std::vector<SpotLight *>::iterator it = spotlights.begin(); it != spotlights.end(); ++it)
	{
		srenders[*it]->step = 2;
		srenders[*it]->Run();
	}
	glFrontFace(GL_CCW);
	for (std::vector<SpotLight *>::iterator it = spotlights.begin(); it != spotlights.end(); ++it)
	{
		srenders[*it]->step = 3;
		srenders[*it]->Run();
	}
	for (std::vector<SpotLight *>::iterator it = spotlights.begin(); it != spotlights.end(); ++it)
	{
		srenders[*it]->step = 4;
		srenders[*it]->Run();
	}
	for (std::vector<PointLight *>::iterator it = pointlights.begin(); it != pointlights.end(); ++it)
	{
		if (!prenders[*it])
			prenders[*it] = new PointLightRenderPass(depth, normal, color, **it, view, target);
		prenders[*it]->Run();
	}
	for (std::vector<SpotLight *>::iterator it = spotlights.begin(); it != spotlights.end(); ++it)
	{
		srenders[*it]->step = 5;
		srenders[*it]->Run();
	}
	target.blendmode = target.Overwrite;
}


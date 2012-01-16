#include "PointLightRenderPass.h"
#include "Scene.h"
#include "Model.h"
#include "debug.h"
#include "PointLight.h"

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
"uniform vec3 lightcolor;\n"
"uniform vec3 lightpos;\n"
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
"    float qattn = dot(lightdir, lightdir);\n"
"    float lattn = length(lightdir);\n"
"    vec3 lightValue = lightcolor * lightbrightness / qattn;\n"
"    float cosLightAngle = dot(lightdir / lattn, n);\n"
"    vec3 diffuseC = lightValue * c * cosLightAngle;\n"
"\n"
"    vec3 eyedir = worldPosition(location, 0) - position;\n"
"    vec3 halfAngle = normalize(eyedir + lightdir);\n"
"    float specular = dot(n, halfAngle);\n"
"    vec3 specularC = lightValue * pow(specular, 512*(norm.w + 40));\n"
"\n"
"    gl_FragData[0] = vec4(specularC + diffuseC, 1);\n"
"}";

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

PointLightRenderPass::PointLightRenderPass(Texture &depth, Texture &normals, Texture &color, PointLight &light, RenderView &view, RenderTarget &output)
: depth(depth)
, normals(normals)
, color(color)
, light(light)
, render(ShaderProgram(noneVS, 0, pointLightFS, invecs), Model::Square(), output, view)
{
	render.AddTexture("depth", depth);
	render.AddTexture("normal", normals);
	render.AddTexture("color", color);
}

void PointLightRenderPass::DoRun()
{
	render.program.Set("lightcolor", light.color);
	render.program.Set("lightpos", light.view.location);
	render.program.Set("lightbrightness", light.brightness);
	render.Run();
}


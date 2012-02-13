#include "HDRRenderEngine.h"
#include "Scene.h"
#include "Model.h"

static const char *tonemapVS =
"#version 330\n"
"in vec3 in_loc;\n"
"out vec2 location;\n"
"\n"
"void main()\n"
"{\n"
"   location = (in_loc.xy + vec2(1,1)) / 2;\n"
"	gl_Position = vec4(in_loc, 1);\n"
"}\n",
*tonemapLinear =
"#version 330\n"
"uniform sampler2D hdrbuf;\n"
"in vec2 location;\n"
"void main (void)\n"
"{\n"
"    gl_FragData[0] = texture2D(hdrbuf, location);\n"
//"    gl_FragData[0] = vec4(location.xy, 0, 1);\n"
"}",
*tonemapReinhard =
"#version 330\n"
"uniform sampler2D hdrbuf;\n"
"in vec4 location;\n"
"void main()\n"
"{\n"
"    const float exposure = 15.0;\n"
"    vec3 color = texture2D(hdrbuf, location.xy).rgb;\n"
"    vec2 texsize = textureSize(hdrbuf, 0);\n"
"    vec3 luminanceSample = textureLod(hdrbuf, vec2(0.5,0.5), log2(max(texsize.x, texsize.y))).xyz;\n"
"    float avgLuminance = dot(luminanceSample, vec3(0.297, 0.599, 0.114));\n"
"\n"
"  const mat3 RGB2XYZ = mat3( 0.5141364, 0.3238786,  0.16036376,\n"
"                             0.265068,  0.67023428, 0.06409157,\n"
"                             0.0241188, 0.1228178,  0.84442666 );\n"	                    
"  vec3 XYZ = RGB2XYZ * color;\n"
"  \n"
"  vec3 Yxy;\n"
"  Yxy.r = XYZ.g;\n"
"  Yxy.g = XYZ.r / (XYZ.r + XYZ.g + XYZ.b );\n"
"  Yxy.b = XYZ.g / (XYZ.r + XYZ.g + XYZ.b );\n"
"\n"
"  float Lp = Yxy.r * exposure / avgLuminance;                       \n"
"  Yxy.r = (Lp)/(1.0f + Lp);\n"
"\n"
"  XYZ.r = Yxy.r * Yxy.g / Yxy. b;\n"
"  XYZ.g = Yxy.r;\n"
"  XYZ.b = Yxy.r * (1 - Yxy.g - Yxy.b) / Yxy.b;\n"
"\n"
"  const mat3 XYZ2RGB  = mat3(  2.5651, -1.1665, -0.3986, \n"
"                              -1.0217,  1.9777,  0.0439, \n"
"                               0.0753, -0.2543,  1.1892);\n"
"  gl_FragData[0] = vec4(XYZ2RGB * XYZ, 1.0);\n"
"}\n";

static const char *GetToneMapper(HDRRenderEngine::ToneMapType type)
{
	switch(type)
	{
	default:
	case HDRRenderEngine::Linear:
		return tonemapLinear;
	case HDRRenderEngine::Reinhard:
		return tonemapReinhard;
	}
}

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

HDRRenderEngine::HDRRenderEngine(Texture &input, RenderTarget &output, ToneMapType type)
: input(input)
, output(output)
/*
, bloomDS((input.width() + 3) / 4, (input.height() + 3) / 4)
, bloomHB((input.width() + 3) / 4, (input.height() + 3) / 4)
, bloomVB((input.width() + 3) / 4, (input.height() + 3) / 4)
, bloomDSR(ShaderProgram(vs, 0, fs, invecs), Model::Square(), targetDS, view)
, bloomHBR(ShaderProgram(vs, 0, fs, invecs), Model::Square(), targetHB, view)
, bloomVBR(ShaderProgram(vs, 0, fs, invecs), Model::Square(), targetVB, view)
*/
, outputRender(ShaderProgram(tonemapVS, 0, GetToneMapper(type), invecs), Model::Square(), output, view)
{
	/*
	bloomDSR.AddTexture("hdrbuf", input);
	targetDS.AddTarget(bloomDS);
	bloomHBR.AddTexture("dsbuf", bloomDS);
	targetHB.AddTarget(bloomHB);
	bloomVBR.AddTexture("hbbuf", bloomHB);
	targetVB.AddTarget(bloomVB);
	outputRender.AddTexture("bloombuf", bloomVB);
	*/
	outputRender.AddTexture("hdrbuf", input);
}

void HDRRenderEngine::DoRun()
{
//	input.SetMipmapped(true);
/*	bloomHBR.Run();
	bloomVBR.Run();
*/	outputRender.Run();
//	input.SetMipmapped(false);
}


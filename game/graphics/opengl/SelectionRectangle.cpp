#include "SelectionRectangle.h"
#include "Scene.h"
#include "Model.h"
#include <utility>

static const char *selectionVS =
"#version 330\n"
"in vec3 in_loc;\n"
"out vec2 location;\n"
"\n"
"void main()\n"
"{\n"
"   location = (in_loc.xy + vec2(1,1)) / 2;\n"
"	gl_Position = vec4(in_loc, 1);\n"
"}\n",
*selectionFS =
"#version 330\n"
"uniform sampler2D hdrbuf;\n"
"uniform float top, left, bottom, right;\n"
"\n"
"in vec2 location;\n"
"void main (void)\n"
"{\n"
"    if (location.x >= left && \n"
"        location.y >= top &&\n"
"        location.x <= right &&\n"
"        location.y <= bottom)\n"
"        gl_FragData[0] = vec4(1.0, 1.0, 1.0, 0.1);\n"
"    else\n"
"        discard;\n"
"}";

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

SelectionRectangle::SelectionRectangle(RenderTarget &output)
: output(output)
, selectionRender(ShaderProgram(selectionVS, 0, selectionFS, invecs), Model::Square(), this->output, view)
{
	this->output.blendmode = RenderTarget::Alphablend;
}

void SelectionRectangle::DoRun()
{
	selectionRender.program.Set("left", std::min(x, x2));
	selectionRender.program.Set("right", std::max(x, x2));
	selectionRender.program.Set("top", std::min(y, y2));
	selectionRender.program.Set("bottom", std::max(y, y2));
	selectionRender.Run();
}


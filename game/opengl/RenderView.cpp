#include "RenderView.h"
#include "ShaderProgram.h"
#include <GL/glew.h>

void RenderView::Set(ShaderProgram &prog, const std::string &postfix)
{
/*	Matrix view = Matrix(location, rotation).Invert();
	Matrix viewproj = projection * view;
	prog.Set(("mat_proj" + postfix).c_str(), projection);
	prog.Set(("mat_view" + postfix).c_str(), view);
	prog.Set(("mat_vp" + postfix).c_str(), viewproj);
	prog.Set(("mat_iv" + postfix).c_str(), view, true);
	prog.Set(("mat_iproj" + postfix).c_str(), projection, true);
	prog.Set(("mat_ivp" + postfix).c_str(), viewproj, true);
*/
}


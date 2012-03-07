#include "ResourceManager.h"
#include "ShaderProgram.h"
#include <stdio.h>
#include <stdlib.h>


class ShaderProgramHandler : public ResourceTypeHandler<ShaderProgram> {
public:
	ShaderProgramHandler();
	ShaderProgram *load(Blob &b);
	const char *getExt() { return "glsl"; }
};

static ShaderProgramHandler _instShader;

ShaderProgramHandler::ShaderProgramHandler() 
{
	ResourceManager::Instance().RegisterResourceTypeHandler<ShaderProgram>(*this);
}

static const char **getArrayPtr(char *str) {
	std::vector<const char *> arr;
	while (str && *str) {
		const char *item = str;
		char *newp = strchr(str, '\n');
		str = newp + 1;
		while (*newp == '\r' ||
			*newp == '\n' ||
			*newp == '\t' ||
			*newp == ' ') {
			*newp-- = 0;
		}
		if (strlen(item))
			arr.push_back(item);
	}
	const char **xarr = new const char *[arr.size() + 1];
	for (size_t i = 0; i < arr.size(); i++) {
		xarr[i] = arr[i];
	}
	xarr[arr.size()] = 0;
	return xarr;
}

ShaderProgram *ShaderProgramHandler::load(Blob &b) {
	char *vs = 0, *fs = 0, *gs = 0, *invars = 0, *varyings = 0;
	char **cur = &vs;
	char *data = new char[b.size+1];
	memcpy(data, b.data, b.size);
	data[b.size] = 0;
	char *op = data;
	for (;;) {
		char *np = strstr(op, "##");
		if (!np) break;
		*np = 0;
		np += 2;
		*cur = op;
		switch (*np) {
			case 'V': case 'v': cur = &vs; break;
			case 'G': case 'g': cur = &gs; break;
			case 'F': case 'f': cur = &fs; break;
			case 'I': case 'i': cur = &invars; break;
			case 'Y': case 'y': cur = &varyings; break;
			default: printf("Unknown shader type %c\n", *np); break;
		}
		op = strstr(np, "\n") + 1;
	}
	*cur = op;

	ShaderProgram *sp = new ShaderProgram(vs, gs, fs, getArrayPtr(invars), getArrayPtr(varyings));
	delete [] data;
	return sp;
}


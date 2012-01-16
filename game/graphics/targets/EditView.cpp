#include "EditView.h"
#include "GameState.h"
#include "Scene.h"
#include "Model.h"
#include "Matrix.h"
#include "Object.h"
#include <SDL.h>
#include <utility>
#include <math.h>
#include "debug.h"
#include "swap.h"
#include "ObjectTypeDatabase.h"
#include "ObjectType.h"
#include "profile.h"

#ifndef NO_EDITING
static const char *defaultVs = 
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
*rasterFs = 
"#version 330\n"
"\n"
"in vec2 texcoord;\n"
"void main (void)\n"
"{\n"
"    ivec2 pos1 = ivec2(texcoord + (-dFdy(texcoord) + -dFdy(texcoord)) / 2);\n"
"    ivec2 pos2 = ivec2(texcoord + (-dFdy(texcoord) +  dFdy(texcoord)) / 2);\n"
"    ivec2 pos3 = ivec2(texcoord + ( dFdx(texcoord) + -dFdx(texcoord)) / 2);\n"
"    ivec2 pos4 = ivec2(texcoord + ( dFdx(texcoord) +  dFdx(texcoord)) / 2);\n"
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
"    		gl_FragData[0] = vec4(0, 1, 0, 1);\n"
"    	else if (mod(posmin.y, 16) - mod(posmax.y, 16) == 15 ||\n"
"    		mod(posmin.x, 16) - mod(posmax.x, 16) == 15)\n"
"    		gl_FragData[0] = vec4(1, 1, 1, 1);\n"
"    	else if (mod(posmin.y, 8) - mod(posmax.y, 8) == 7 ||\n"
"    	         mod(posmin.x, 8) - mod(posmax.x, 8) == 7)\n"
"    		gl_FragData[0] = vec4(1, 1, 0, 1);\n"
"    	else \n"
"    		gl_FragData[0] = vec4(0.3, 0.3, 0.3, 1);\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n",
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

EditView::EditView(RenderTarget &target)
: mainTarget(target)
, tl((int)(target.width * 0.45 - 4), (int)(target.height * 0.5 - 4), true)
, tr((int)(target.width * 0.45 - 4), (int)(target.height * 0.5 - 4), true)
, bl((int)(target.width * 0.45 - 4), (int)(target.height * 0.5 - 4), true)
, br((int)(target.width * 0.45 - 4), (int)(target.height * 0.5 - 4), true)
, right((int)(target.width * 0.1), target.height, false)
, left(Scene::Instance(), tr, WireframeWithRaster::Left)
, top(Scene::Instance(), tl, WireframeWithRaster::Top)
, front(Scene::Instance(), bl, WireframeWithRaster::Front)
, rasterToBr(ShaderProgram(defaultVs, 0, rasterFs, invecs), Model::Raster(), br, cam.view)
, dre_tex(ShaderProgram(defaultVs, 0, objIdFs, invecs), Scene::Instance(), br, cam.view)
, dre(Scene::Instance(), br, cam.view)
, select(target)
, viewmode(false)
, mousex(0)
, mousey(0)
, scale(10.0f)
, rasterLocation(60, 5, 60)
, catNameIdx(0)
, objNameIdx(0)
{
	cam.view.location = Vector3(50,5,50);
	for (std::map<std::string, std::map<std::string, ObjectType *> >::iterator it = ObjectTypeDatabase::Instance().categories.begin();
		it != ObjectTypeDatabase::Instance().categories.end(); ++it)
	{
		catNames.push_back(it->first);
	}
	for (std::map<std::string, ObjectType *>::iterator it = ObjectTypeDatabase::Instance().categories[catNames[catNameIdx]].begin();
		it != ObjectTypeDatabase::Instance().categories[catNames[catNameIdx]].end(); ++it)
	{
		objNames.push_back(it->first);
	}

	bl.vx = tl.vx = 2;
	br.vx = tr.vx = (int)(target.width * 0.45) + 2;
	br.vy = bl.vy = 2;
	tr.vy = tl.vy = (int)(target.height * 0.5) + 2;
	right.vx = (int)(target.width * 0.9);
	select.enabled = false;
}

void EditView::Run()
{
	left.scale = top.scale = front.scale = scale;
	left.location = top.location = front.location = rasterLocation;
	cam.Update();
	mainTarget.Clear();
//	rasterToBr.program.Set("mat_model[0]", Matrix());
//	rasterToBr.Run();
	left.Run();
	top.Run();
	front.Run();
	select.Run();
	dre.Run();
}

Object *EditView::Pick(int x, int y)
{
	std::vector<Object *> objs;
	objs.push_back(NULL);
	mainTarget.Clear();
	dre_tex.DrawPick(objs);
	left.DrawPick(objs);
	top.DrawPick(objs);
	front.DrawPick(objs);
	unsigned int id;
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &id);

	if ((id >> 8) > objs.size())
	{
		DebugBreak();
		return NULL;
	}
	return objs[id >> 8];
}

std::set<Object *> EditView::PickArea(int x, int y, int x2, int y2)
{
	std::vector<Object *> objs;
	std::set<Object *> returns;
	objs.push_back(NULL);
	mainTarget.Clear();
	dre_tex.DrawPick(objs);
	left.DrawPick(objs);
	top.DrawPick(objs);
	front.DrawPick(objs);

	if (x > x2) swap(x, x2);
	if (y > y2) swap(y, y2);

	mainTarget.Activate();
	unsigned int *id = new unsigned int[(x2-x)*(y2-y)];
	memset(id, 0, (x2-x)*(y2-y));
	glReadPixels(x, y, x2-x, y2 - y, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, id);

	for (int i = 0; i < (x2-x) * (y2-y); i++)
	{
		if ((id[i] >> 8) > objs.size()) 	
		{
			DebugBreak();
			break;
		}
		Object *obj = objs[id[i] >> 8];
		if (obj &&
			returns.find(obj) == returns.end())
			returns.insert(obj);
	}

	delete [] id;

	return returns;
}


static bool alt, ctrl, shift;
static bool lalt = false, ralt = false, lctrl = false, rctrl = false, lshift = false, rshift = false;
static int mouseButtonDown = 0;
static bool dragging = false;
static int mousedragstart_x, mousedrag_x;
static int mousedragstart_y, mousedrag_y;
static std::set<Object *> selected;
static int storedx, storedy;

void EditView::MoveSelection(int dx, int dy)
{
	storedx += dx;
	storedy += dy;
	dx = storedx / 10;
	dy = storedy / 10;
	storedx -= dx * 10;
	storedy -= dy * 10;
	TODO("Fix moving objects");
/*
	for (std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		Vector3 newLocation = (*it)->getTranslation() + deltaXMovement.normalize() * (0.5f * dx) + deltaYMovement.normalize() * (0.5f * dy);
		Scene::Instance().MoveObject(*it, newLocation);
	}
	*/
}

void EditView::RotateSelection(int da)
{
	storedx += da;
	da = storedx / 10;
	storedx -= 10 * da;
//	float a = da * 0.3926991f;
	TODO("Fix rotating objects");
/*
	Quaternion deltaRotation(cos(a), sin(a) * rotAxis.x, sin(a) * rotAxis.y, sin(a) * rotAxis.z);
	for (std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		(*it)->setRotation((*it)->getRotation() * deltaRotation);
	}
	*/
}

void EditView::AlignSelection()
{
	for (std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
//		Scene::Instance().AddObject(new Object(**it));
	}
}

void EditView::DuplicateSelection()
{
	for (std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		Scene::Instance().AddObject(new Object(**it));
	}
}

void EditView::MouseMove(int x, int y)
{
	if (mouseButtonDown == SDL_BUTTON_LEFT)
	{
		if (!dragging)
		{
			int dx2 = (mousedrag_x - x) * (mousedrag_x - x);
			int dy2 = (mousedrag_y - y) * (mousedrag_y - y);
			if (dx2 + dy2 >= 5*5)
			{
				dragging = true;
			}
		}

		if (dragging)
		{
			if (alt)
			{
				MoveSelection(x - mousedrag_x, mousedrag_y - y);
			}
			else if (shift)
			{
				RotateSelection(x - mousedrag_x);
			}
			else
			{
				select.enabled = true;
				select.x = (float)std::min(x, mousedragstart_x) / mainTarget.width;
				select.x2 = (float)std::max(x, mousedragstart_x) / mainTarget.width;
				select.y = (float)std::min(y, mousedragstart_y) / mainTarget.height;
				select.y2 = (float)std::max(y, mousedragstart_y) / mainTarget.height;
			}
			mousedrag_x = x;
			mousedrag_y = y;
		}
	}

	mousex = x;
	mousey = y;
}

void EditView::MouseMoveX(int delta)
{
	if (mouseButtonDown == SDL_BUTTON_RIGHT)
	{
		if (viewmode) cam.MouseMoveX(delta);
		rasterLocation += deltaXMovement * (float)delta;
	}
}

void EditView::MouseMoveY(int delta)
{
	if (mouseButtonDown == SDL_BUTTON_RIGHT)
	{
		if (viewmode) cam.MouseMoveY(delta);
		rasterLocation += deltaYMovement * (float)delta;
	}
}

void EditView::MouseScrollUp()
{
	scale *= 1.1f;
}

void EditView::MouseScrollDown()
{
	scale /= 1.1f;
}

void EditView::MouseButtonUp(int button)
{
	if (mouseButtonDown != button)
		return;

	if (button == SDL_BUTTON_RIGHT)
	{
		viewmode = false;
		deltaXMovement = Vector3();
		deltaYMovement = Vector3();
	}
	else if (dragging)
	{
		if (alt || shift)
		{
			// handled during the dragging
		}
		else 
		{
			std::set<Object *> objs;
			int minx = std::min(mousex, mousedragstart_x);
			int maxx = std::max(mousex, mousedragstart_x);
			int miny = std::min(mousey, mousedragstart_y);
			int maxy = std::max(mousey, mousedragstart_y);
			objs = PickArea(minx, miny, maxx, maxy);

			if (ctrl)
			{
				for (std::set<Object *>::iterator it = objs.begin(); it != objs.end(); ++it)
				{
					if (!(*it)->isSelected())
					{
						(*it)->setSelected(true);
						selected.insert(*it);
					}
				}
			}
			else
			{
				bool allSelected = true;
				for (std::set<Object *>::iterator it = objs.begin(); it != objs.end(); ++it)
				{
					if (!(*it)->isSelected())
					{
						allSelected = false;
					}
				}
				if (allSelected)
				{
					for (std::set<Object *>::iterator it = objs.begin(); it != objs.end(); ++it)
					{
						(*it)->setSelected(false);
					}
					selected.clear();
				}
				else
				{
					for (std::set<Object *>::iterator it = objs.begin(); it != objs.end(); ++it)
					{
						if (!(*it)->isSelected())
						{
							(*it)->setSelected(true);
							selected.insert(*it);
						}
					}
				}
			}
		}
	}
	else
	{
		Object *obj = Pick(mousex, mousey);
		if (obj)
		{
			if (ctrl)
			{
				if (obj->isSelected())
				{
					selected.erase(obj);
					obj->setSelected(false);
				}
				else
				{
					selected.insert(obj);
					obj->setSelected(true);
				}
			}
			else
			{
				for(std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
				{
					(*it)->setSelected(false);
				}
				selected.clear();

				selected.insert(obj);
				obj->setSelected(true);
			}
		}
		else
		{
			if (!ctrl)
			{
				for(std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
				{
					(*it)->setSelected(false);
				}
				selected.clear();
			}
		}
	}

	mouseButtonDown = 0;
	select.enabled = false;
}

void EditView::MouseButtonDown(int button)
{
	if (mouseButtonDown) 
		return;

	mouseButtonDown = button;
	float baseUnit = (4 * scale / mainTarget.height);

	if (mousex >= mainTarget.width * 0.45 &&
		mousex < mainTarget.width * 0.9 &&
		mousey >= mainTarget.height * 0.5)
	{
		deltaXMovement = Vector3(0, 0, baseUnit);
		rotAxis = Vector3(1, 0, 0);
		deltaYMovement = Vector3(0, -baseUnit, 0);
	}
	else if (mousex < mainTarget.width * 0.45 &&
		mousey < mainTarget.height * 0.5)
	{
		deltaXMovement = Vector3(-baseUnit, 0, 0);
		rotAxis = Vector3(0, 0, 1);
		deltaYMovement = Vector3(0, -baseUnit, 0);
	}
	else if (mousex < mainTarget.width * 0.45 &&
			 mousey >= mainTarget.height * 0.5)
	{
		deltaXMovement = Vector3(-baseUnit, 0, 0);
		rotAxis = Vector3(0, 1, 0);
		deltaYMovement = Vector3(0, 0, -baseUnit);
	}

	if (mousex > mainTarget.width * 0.9)
	{
		// forward to right hand edit window
	}
	else
	{
		if (button == SDL_BUTTON_LEFT)
		{
			dragging = false;
			mousedragstart_x = mousedrag_x = mousex;
			mousedragstart_y = mousedrag_y = mousey;
			alt = lalt || ralt;
			ctrl = lctrl || rctrl;
			shift = lshift || rshift;
			storedx = 0;
			storedy = 0;
		}
		else
		{
			if (mousex >= mainTarget.width * 0.45 &&
					 mousey < mainTarget.height * 0.5)
			{
				viewmode = true;
			}
		}
	}
}

void EditView::KeyDown(int key)
{
	switch (key)
	{
	case SDL_SCANCODE_LALT: lalt = true; break;
	case SDL_SCANCODE_RALT: ralt = true; break;
	case SDL_SCANCODE_LCTRL: lctrl = true; break;
	case SDL_SCANCODE_RCTRL: rctrl = true; break;
	case SDL_SCANCODE_LSHIFT: lshift = true; break;
	case SDL_SCANCODE_RSHIFT: rshift = true; break;
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_S:
		cam.KeyDown(key);
		break;
	case SDL_SCANCODE_A:
		if ((lctrl || rctrl) && (lalt || ralt))
			AlignSelection();
		else
			cam.KeyDown(key);
		break;
	case SDL_SCANCODE_D:
		if ((lctrl || rctrl) && (lalt || ralt))
			DuplicateSelection();
		else
			cam.KeyDown(key);
		break;
	case SDL_SCANCODE_1: 
	case SDL_SCANCODE_2: 
	case SDL_SCANCODE_3: 
	case SDL_SCANCODE_4: 
	case SDL_SCANCODE_5: 
	case SDL_SCANCODE_6: 
	case SDL_SCANCODE_7: 
	case SDL_SCANCODE_8:
	case SDL_SCANCODE_9:
		for(std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
		{
			TODO("Fix effects so that they start from the object-relative (bone-relative) and object-space location of impact");
			/*
			Object::DissolveType type = (Object::DissolveType)(key - SDL_SCANCODE_1 + 3);
			Vector3 loc = (*it)->getTranslation();
			if (type == Object::Dissolve || type == Object::Liquefy) loc += Vector3(0, 3, 0);
			if (type == Object::Disintegrate || type == Object::Electrocute) loc += Vector3(-2, -2, -2);
			if (type == Object::TeleportIn || type == Object::TeleportOut || type == Object::TeleportThrough) loc += Vector3(0, -3, 0);
			(*it)->StartEffect(SDL_GetTicks(), loc, type, false);
			*/
		}
		break;
	case SDL_SCANCODE_N:
		{
			TODO("Fix new object - where do you invent new object locations?");
//			ObjectType *ot = ObjectTypeDatabase::Instance().categories[catNames[catNameIdx]][objNames[objNameIdx]];
//			Object *ob = ot->Create(rasterLocation, Quaternion());
//			Scene::Instance().AddObject(ob);
//			selected.clear();
//			selected.insert(ob);
		}
		break;
	case SDL_SCANCODE_LEFTBRACKET:
		catNameIdx--;
		if (catNameIdx < 0) catNameIdx = catNames.size() - 1;
		objNames.clear();
		for (std::map<std::string, ObjectType *>::iterator it = ObjectTypeDatabase::Instance().categories[catNames[catNameIdx]].begin();
			it != ObjectTypeDatabase::Instance().categories[catNames[catNameIdx]].end(); ++it)
		{
			objNames.push_back(it->first);
		}
		if (objNameIdx >= objNames.size()) objNameIdx = 0;
		printf("selected category %s object type %s\n", catNames[catNameIdx].c_str(), objNames[objNameIdx].c_str());
		break;
	case SDL_SCANCODE_RIGHTBRACKET:
		catNameIdx++;
		if (catNameIdx >= catNames.size()) catNameIdx = 0;
		objNames.clear();
		for (std::map<std::string, ObjectType *>::iterator it = ObjectTypeDatabase::Instance().categories[catNames[catNameIdx]].begin();
			it != ObjectTypeDatabase::Instance().categories[catNames[catNameIdx]].end(); ++it)
		{
			objNames.push_back(it->first);
		}
		if (objNameIdx >= objNames.size()) objNameIdx = 0;
		printf("selected category %s object type %s\n", catNames[catNameIdx].c_str(), objNames[objNameIdx].c_str());
		break;
	case SDL_SCANCODE_SEMICOLON:
		objNameIdx--;
		if (objNameIdx < 0) objNameIdx = objNames.size() - 1;
		printf("selected category %s object type %s\n", catNames[catNameIdx].c_str(), objNames[objNameIdx].c_str());
		break;
	case SDL_SCANCODE_APOSTROPHE:
		objNameIdx++;
		if (objNameIdx >= objNames.size()) objNameIdx = 0;
		printf("selected category %s object type %s\n", catNames[catNameIdx].c_str(), objNames[objNameIdx].c_str());
		break;
	case SDL_SCANCODE_DELETE:
		for(std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
		{
			Scene::Instance().RemoveObject(*it);
		}
		break;
	case SDL_SCANCODE_R:
		TODO("Fix the fix-alignment command");
/*		for(std::set<Object *>::iterator it = selected.begin(); it != selected.end(); ++it)
		{
			(*it)->setRotation(Quaternion());
		}
*/
		break;
	}
}

void EditView::KeyUp(int key)
{
	switch (key)
	{
	case SDL_SCANCODE_LALT: lalt = false; break;
	case SDL_SCANCODE_RALT: ralt = false; break;
	case SDL_SCANCODE_LCTRL: lctrl = false; break;
	case SDL_SCANCODE_RCTRL: rctrl = false; break;
	case SDL_SCANCODE_LSHIFT: lshift = false; break;
	case SDL_SCANCODE_RSHIFT: rshift = false; break;
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_D:
		cam.KeyUp(key);
		break;
	}
}

#endif


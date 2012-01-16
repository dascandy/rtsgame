#include "ResourceFile.h"
#include "stream.h"
#include "debug.h"

stream ResourceFile::open(std::string filename, filetype type)
{
	std::string fullfilename = Instance().fileName;
	switch(type)
	{
	case Ms3dModel:
		fullfilename += "models\\" + filename + ".ms3d";
		break;
	case Music:
		fullfilename += "music\\" + filename + ".ogg";
		break;
	case SoundFragment:
		fullfilename += "sound\\" + filename + ".wav";
		break;
	case SfxSoundFragment:
		fullfilename += "sound\\" + filename + ".sfx";
		break;
	case DatFile:
		fullfilename += "datfiles\\" + filename + ".dat";
		break;
	case PhysicsModel:
		fullfilename += "physics\\" + filename + ".col";
		break;
	case Movie:
		fullfilename += "movie\\" + filename + ".avi";
		break;
	case Scene:
		fullfilename += "levels\\" + filename + ".lvq";
		break;
	case SceneText:
		fullfilename += "levels\\" + filename + ".lvl";
		break;
	case DXPixelShader:
		fullfilename += "directx\\" + filename + ".dps";
		break;
	case DXVertexShader:
		fullfilename += "directx\\" + filename + ".dvs";
		break;
	case DXGeometryShader:
		fullfilename += "directx\\" + filename + ".dgs";
		break;
	case HdrTexture:
		fullfilename += "textures\\" + filename + ".hdr";
		break;
	case PngTexture:
		fullfilename += "textures\\" + filename + ".png";
		break;
	case BmpTexture:
		fullfilename += "textures\\" + filename + ".bmp";
		break;
	default:
		LOG_WARN("Cannot open file %s", fullfilename.c_str());
		throw std::exception();
	}
	return stream(fullfilename);
}

stream ResourceFile::openW(std::string filename, filetype type)
{
	std::string fullfilename = Instance().fileName;
	switch(type)
	{
	case Ms3dModel:
		fullfilename += "models\\" + filename + ".ms3d";
		break;
	case DatFile:
		fullfilename += "datfiles\\" + filename + ".dat";
		break;
	case PhysicsModel:
		fullfilename += "physics\\" + filename + ".col";
		break;
	case Scene:
		fullfilename += "levels\\" + filename + ".lvq";
		break;
	case SceneText:
		fullfilename += "levels\\" + filename + ".lvl";
		break;
	case HdrTexture:
		fullfilename += "textures\\" + filename + ".hdr";
		break;
	case PngTexture:
		fullfilename += "textures\\" + filename + ".png";
		break;
	case BmpTexture:
		fullfilename += "textures\\" + filename + ".bmp";
		break;
	default:
		LOG_WARN("Cannot open file %s for writing", fullfilename.c_str());
		throw std::exception();
	}
	return stream(fullfilename, true);
}

ResourceFile &ResourceFile::Instance(const std::string &fileName)
{
	static ResourceFile file(fileName);
	return file;
}

ResourceFile::ResourceFile(const std::string &fileName)
: fileName(fileName)
{
}

ResourceFile::~ResourceFile()
{
}


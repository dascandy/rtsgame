#ifndef RESOURCEFILE_H
#define RESOURCEFILE_H

#include <string>

class stream;

class ResourceFile
{
public:
	enum filetype
	{
		Ms3dModel,       // Milkshape3D model
		Music,           // OGG Vorbis music / sound effect
		SoundFragment,   // Sound fragment in WAV format at 44100 16b mono
		SfxSoundFragment,// Sound fragment in SFX format at 11025 8b mono
		DatFile,	     // Data file describing object definitions & stuff
		PhysicsModel,    // Physics model for a given 3d model
		Movie,           // FMV movie pre-recorded for the game
		Scene,			 // Scene file
		SceneText,		 // uncompiled Scene file (testing only)
		DXPixelShader,   // pixel shader for DirectX renderer
		DXVertexShader,  // vertex shader for DirectX renderer
		DXGeometryShader,// geometry shader for DirectX renderer
		HdrTexture,      // texture (16F HDR file)
		PngTexture,      // texture (32bpp PNG file)
		BmpTexture,      // texture (24+8bpp BMP file)
	};
	static stream open(std::string filename, filetype type);
	static stream openW(std::string filename, filetype type);
	static ResourceFile &Instance(const std::string &fileName = "");
private:
	int read(char *buffer, int offset, int count);
	ResourceFile(const std::string &fileName);
	~ResourceFile();
	ResourceFile &operator=(const ResourceFile &);
	ResourceFile(const ResourceFile &);
	std::string fileName;
};

#endif


#include "VfwAvi.h"
#include <exception>
#include "stdio.h"
#include "debug.h"

class Vfw {
public:
	std::string current;
    HIC                 hicv;

	Vfw()
	: current("")
	, hicv(0)
	{
		AVIFileInit();
	}
	~Vfw() 
	{
		if (hicv)
			ICDecompressEnd(hicv);

	    AVIFileExit();
	}
	void SwitchTo(unsigned int fcc, LPBITMAPINFOHEADER informat, LPBITMAPINFOHEADER outformat)
	{
		if (hicv)
			ICDecompressEnd(hicv);

		hicv = ICLocate(ICTYPE_VIDEO, fcc, informat, outformat, ICMODE_DECOMPRESS);

		if (!hicv) 
			throw std::exception();

		if (ICDecompressQuery(hicv, informat, outformat) != ICERR_OK)
			throw std::exception();

		if (ICDecompressBegin(hicv, informat, outformat) != ICERR_OK)
			throw std::exception();
	}
	bool SwitchNDecompress(std::string name, unsigned int fcc, LPBITMAPINFOHEADER informat, void *avidata, LPBITMAPINFOHEADER outformat, void *tex)
	{
		if (name != current)
		{
			SwitchTo(fcc, informat, outformat);
			current = name;
		}

		return (ICDecompress(hicv, 0, informat, avidata, outformat, tex) == ICERR_OK);
	}
};

static Vfw vfw;

VfwAvi::VfwAvi(std::string name) 
: name(name)
, fps(0)
, movieTime(0)
, currentFrame(0)
, maxFrame(0)
, texframe(0)
, avidata(0)
, stream(0)
, outformat(0)
, informat(0)
{
	std::string filename = "C:\\crusader\\game\\res\\movie\\" + name + ".avi";

	if (AVIStreamOpenFromFile(&stream, filename.c_str(), streamtypeVIDEO, 0, OF_READ, NULL)) 
		throw std::exception();

	AVISTREAMINFO avi_info;
	if (AVIStreamInfo(stream, &avi_info, sizeof(avi_info)))
		throw std::exception();

	LONG          size;
	if (AVIStreamFormatSize(stream, 0, &size)) 
		throw std::exception();

	informat  = (LPBITMAPINFOHEADER)malloc(size);
	memset(informat, 0, size);

	outformat = (LPBITMAPINFOHEADER)malloc(size);
	memset(outformat, 0, size);

	if (AVIStreamReadFormat(stream, 0, informat, &size))
		throw std::exception();

	avidata   = new unsigned char[informat->biHeight * informat->biWidth * 4];
	memset(avidata,  0, informat->biHeight * informat->biWidth * 4);

	texframe  = new unsigned char[informat->biHeight * informat->biWidth * 4];
	memset(texframe, 0, informat->biHeight * informat->biWidth * 4);

	outformat->biSize         = size;
	outformat->biWidth        = informat->biWidth;
	outformat->biHeight       = informat->biHeight;
	outformat->biPlanes       = 1;
	outformat->biBitCount     = 32;

	maxFrame = AVIStreamLength(stream);
	fps = (float)avi_info.dwRate / avi_info.dwScale;
	fcc = avi_info.fccHandler;
}

VfwAvi::~VfwAvi(void)
{
	if (informat)
		free(informat);
	if (outformat)
		free(outformat);

	if (stream)
		AVIStreamRelease(stream);

	delete [] avidata;
	delete [] texframe;
}

int VfwAvi::GetWidth(void) 
{
	return informat->biWidth;
}

int VfwAvi::GetHeight(void) 
{
    return informat->biHeight;
}

void *VfwAvi::Frame(msec_t delta) 
{
	movieTime += delta;
	int nextFrame = (int)(movieTime * fps / 1000);
	if (nextFrame >= maxFrame) 
		return NULL;

	for (; currentFrame < nextFrame; ++currentFrame)
	{
		if (AVIStreamRead(stream, currentFrame, 1, avidata, informat->biHeight * informat->biWidth * informat->biBitCount / 8, NULL, NULL)) 
			return NULL;

		if (!vfw.SwitchNDecompress(name, fcc, informat, avidata, outformat, texframe))
			return NULL;
	}

	return texframe;
}

void VfwAvi::mix(signed short *, int)
{
	TODO_W("audio");
}


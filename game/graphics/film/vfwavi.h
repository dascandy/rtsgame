#ifndef VFWAVI_H
#define VFWAVI_H

#include "Movie.h"
#include <windows.h>
#include <vfw.h>
#include "os.h"
#include <string>

#pragma comment(lib, "vfw32.lib")

class VfwAvi : public Movie 
{
public:
	VfwAvi(std::string filename);
    ~VfwAvi();

    int GetWidth();
    int GetHeight();
	void *Frame(msec_t delta);
	void mix(signed short *target, int sampleCount);
private:
	void Start(std::string filename);
	std::string name;

	unsigned int fcc;
	float fps;
    msec_t movieTime;
	int currentFrame;
	int maxFrame;
    unsigned char * texframe;
    unsigned char * avidata;

    PAVISTREAM          stream;
    LPBITMAPINFOHEADER  outformat;
    LPBITMAPINFOHEADER  informat;
};

#endif


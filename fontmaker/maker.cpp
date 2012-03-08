#include <stdio.h>
#include <string.h>
#include "Font.h"

struct bmphdr {
	char p1[18];
	unsigned int w __attribute__((packed)), h __attribute__((packed));
	char p2[8];
	unsigned int bufsize __attribute__((packed));
	char p3[16];
};

class Image {
public:
	unsigned int *buffer;
	unsigned short w, h;
	Image(const char *filename) {
		if (sizeof(bmphdr) != 0x36) printf("BMPHEADER not right");
		FILE *f = fopen(filename, "rb");
		bmphdr hdr;
		fread(&hdr, sizeof(hdr), 1, f);
		w = hdr.w;
		h = hdr.h;
		buffer = new unsigned int[hdr.bufsize/4];
		printf("%d * %d bufsize %d\n", w, h, hdr.bufsize);
		for (int i = hdr.h-1; i >= 0; i--) {
			fread(buffer + i * w, w*4, 1, f);
		}
		fclose(f);
	}
	unsigned int operator()(int x, int y) {
		if (x < 0 || y < 0 || x >= w || y >= h) return 0;
		return buffer[y * w + x] & 0xFFFFFF;
	}
};

int determine_fonth(Image &v, int chars) {
	int i;
	for (i = v.h-1; i > 0; i--) {
		bool foundSomething = false;
		for (int j = 0; j < v.w; j++) {
			if (v(j, i) != 0xFFFFFF) {
				foundSomething = true;
			}
		}
		if (foundSomething) break;
	}
	//i = (int)(i * 1.005f);
	printf("Determined font height to be %d (%d is last non-white pixel, %d chars)\n", i / chars, i, chars);
	return i / chars;
}

int main(int argc, char **argv) {
	Image h(argv[1]);
	Image v(argv[2]);
	const char charstoget[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`~!@#$%^&*()-_=+[{]}\\|;:'\",<.>/? 0123456789A";
	Font f;
	f.h = 132;//determine_fonth(v, strlen(charstoget));
	int curx = 0, cury = 0;
	char lastchar = 0;
	for (int i = 0; i < strlen(charstoget); i++) {
		printf("cur = %d %d\n", curx, cury);
		char tomatch = charstoget[i];
		int thischarleft = 0;
		for (int j = 0; j < v.w; j++) {
			bool foundAny = false;
			for (int k = 0; k < f.h; k++) {
				if (v(j, k + cury) != 0xFFFFFF) foundAny = true;
			}
			thischarleft = j;
			if (foundAny) break;
		}
		for (;; curx++) {
			bool foundAny = false;
			for (int k = 0; k < f.h; k++) {
				if (h(curx, k) != 0xFFFFFF) foundAny = true;
			}
			if (!foundAny) { curx++; break; }
		}
		for (;; curx++) {
			bool foundAny = false;
			for (int k = 0; k < f.h; k++) {
				if (h(curx, k) != 0xFFFFFF) foundAny = true;
			}
			if (foundAny) { curx++; break; }
		}
		if (i != strlen(charstoget) - 1) {
			f[tomatch].x = curx - thischarleft;
			printf("%d %d\n", curx, thischarleft);
			printf("found %c x %d\n", tomatch, f[tomatch].x);
		}
		if (lastchar) {
			f[lastchar].w = curx - thischarleft - f[lastchar].x;
			printf("found %c w %d\n", lastchar, f[lastchar].w);
		}
		lastchar = tomatch;
		cury += f.h;
	}

	f.write("font.fnt");
}





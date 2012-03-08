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

int main(int argc, char **argv) {
	Image h(argv[1]);
	Image v(argv[2]);
	const char charstoget[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`~!@#$%^&*()-_=+[{]}\\|;:'\",<.>/? 0123456789A";
	Font f;
	f.h = 115;
	int curx = 0, cury = 0;
	char lastchar = 0;
	for (int i = 0; i < strlen(charstoget); i++) {
		char tomatch = charstoget[i];
		if (i != strlen(charstoget) - 1) {
			f[tomatch].x = curx;
		}
		if (lastchar) {
			f[lastchar].w = 60;
		}
		lastchar = tomatch;
		cury += f.h;
		curx += 60;
		if (i % 20 == 0) curx++;
	}

	f.write("font.fnt");
}





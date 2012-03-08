#ifndef FONT_H
#define FONT_H

#include <map>

class Font {
public:
	struct entry {
		int x, y, w;
	};
	std::map<char, entry> entries;
	int h;
	entry &operator[](char index) { return entries[index]; }
	void write(const char *filename) {
		FILE *f = fopen(filename, "wb");
		for (std::map<char, entry>::iterator it = entries.begin(); it != entries.end(); ++it) {
			fprintf(f, "% 3d % 5d % 5d % 5d % 5d\n", it->first, it->second.x, 0, it->second.w, h);
		}
		fclose(f);
	}
};

#endif



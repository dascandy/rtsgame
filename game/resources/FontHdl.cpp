#include "Font.h"
#include "ResourceManager.h"
#include <map>

class FontWriter : public ResourceStorer<Font> {
public:
	FontWriter();
	Blob save(Font *);
	const char *getExt() { return "fnt"; }
};

class FontReader : public ResourceTypeHandler<Font> {
public:
	FontReader();
	Font *load(Blob &b, const char *name);
	const char *getExt() { return "fnt"; }
};

static FontReader _readerFnt;
static FontWriter _writerFnt;

FontReader::FontReader() 
{
	ResourceManager::Instance().RegisterResourceTypeHandler<Font>(*this);
}

static char *getLine(char *&linePtr) {
	if (linePtr == 0) return 0;
	char *line = linePtr;
	while (*linePtr != '\n' && *linePtr != 0) linePtr++;
	if (*linePtr == 0) {
		linePtr = 0;
		return line;
	}
	if (linePtr[-1] == '\r') linePtr[-1] = 0;
	*linePtr = 0;
	linePtr++;
	return line;
}

Font *FontReader::load(Blob &b, const char *name) {
	// use a copy as we can't modify the original blob since it might be used by another reader
	Blob copy(b.size);
	memcpy(copy.data, b.data, b.size);
	char *buf = copy.data;
	const char *texture = getLine(buf);
	Font *f = new Font(name, ResourceManager::Instance().getResource<Texture>(texture), texture);
	const char *vals;
	while ((vals = getLine(buf)) != 0) {
		int charno, x, y, w, h;
		sscanf(vals, " %d %d %d %d %d", &charno, &x, &y, &w, &h);
		f->entries.insert(std::make_pair((char)charno, Font::entry(x, y, w, h)));
	}
	return f;
}

FontWriter::FontWriter() 
{
	ResourceManager::Instance().RegisterResourceStorer<Font>(*this);
}

Blob FontWriter::save(Font *b) {
	char *buffer = new char[40*b->entries.size() + 100];
	char *writeptr = buffer;
	sprintf(writeptr, "%s\n", b->texturename);
	for (std::map<char, Font::entry>::iterator it = b->entries.begin(); it != b->entries.end(); ++it) {
		writeptr += strlen(writeptr);
		sprintf(writeptr, "% 5d % 5d % 5d % 5d % 5d\n", it->first, it->second.x, it->second.y, it->second.w, it->second.h);
	}
	return Blob(strlen(buffer), buffer);
}


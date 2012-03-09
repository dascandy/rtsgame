#include "Font.h"
#include "ResourceManager.h"

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

Font *FontReader::load(Blob &b, const char *) {
	return 0;
}

FontWriter::FontWriter() 
{
	ResourceManager::Instance().RegisterResourceStorer<Font>(*this);
}

Blob FontWriter::save(Font *b) {
	return Blob();
}


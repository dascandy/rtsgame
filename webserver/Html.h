#ifndef HTML_H
#define HTML_H

#include <string>
#include <vector>

class Div;

class HtmlPage {
private:
	std::vector<Div *> divs;
	std::vector<std::string> scripts;
public:
	void addDiv(Div *div);
	std::string toString();
};

class Div {
public:
	virtual std::string toString() = 0;
};

class ImageDiv : public Div {
private:
	int x, y, w, h;
	std::string imageUrl;
public:
	ImageDiv(int x, int y, int w, int h, std::string imageUrl);
	std::string toString();
};

class DocumentDiv : public Div {
public:
	int x, y, w, h;
	std::string fileUrl;
public:
	DocumentDiv(int x, int y, int w, int h, std::string fileUrl);
	std::string toString();
};

#endif



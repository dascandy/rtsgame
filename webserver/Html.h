#ifndef HTML_H
#define HTML_H

#include <string>
#include <vector>
#include "Dll.h"

class Div;

class WEBSERVERSPEC HtmlPage {
private:
	std::vector<Div *> divs;
	std::vector<std::string> scripts;
public:
	void addDiv(Div *div);
	std::string toString();
};

class WEBSERVERSPEC Div {
protected:
	int x, y, w, h;
	Div(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
public:
	virtual std::string toString() = 0;
};

class WEBSERVERSPEC ImageDiv : public Div {
private:
	std::string imageUrl;
public:
	ImageDiv(int x, int y, int w, int h, std::string imageUrl);
	std::string toString();
};

class WEBSERVERSPEC DocumentDiv : public Div {
private:
	std::string fileUrl;
public:
	DocumentDiv(int x, int y, int w, int h, std::string fileUrl);
	std::string toString();
};

class WEBSERVERSPEC DirlistDiv : public Div {
public:
	DirlistDiv(int x, int y, int w, int h);
	std::string toString();
};

#endif



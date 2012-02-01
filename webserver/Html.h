#ifndef HTML_H
#define HTML_H

#include <string>
#include <vector>

class Div;

class HtmlPage {
private:
	std::vector<Div *> divs;
public:
	void addDiv(Div *div);
	std::string toString();
};

class Div {
private:
	int x, y, w, h;
	std::string imageUrl;
public:
	Div(int x, int y, int w, int h, std::string imageUrl);
	std::string toString();
};

#endif



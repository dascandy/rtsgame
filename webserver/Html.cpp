#include "Html.h"
#include <stdio.h>
void HtmlPage::addDiv(Div *div) {
	divs.push_back(div);
}

std::string HtmlPage::toString() {
	std::string page = "<html><head><title></title></head><body>";
	for (std::vector<Div *>::iterator it = divs.begin(); it != divs.end(); ++it) {
		page += (*it)->toString();
	}
	page += "</body></html>";
	return page;
}

Div::Div(int x, int y, int w, int h, std::string imageUrl)
: x(x)
, y(y)
, w(w)
, h(h)
, imageUrl(imageUrl)
{}

std::string Div::toString() {
	char buffer[2048];
	sprintf(buffer, "<div style=\"position: absolute; left: %dpx; top: %dpx; width: %dpx; height: %dpx; border: 0px; margin: 0px; padding: 0px\">"
		"<a href=\"%s\"><img width=\"%d\" height=\"%d\" src=\"%s?type=thumb\"></a></div>", x, y, w, h, imageUrl.c_str(), w, h, imageUrl.c_str());
	return buffer;
}



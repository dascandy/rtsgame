#include "Html.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

ImageDiv::ImageDiv(int x, int y, int w, int h, std::string imageUrl)
: Div(x, y, w, h)
, imageUrl(imageUrl)
{}

std::string ImageDiv::toString() {
	char buffer[2048];
	sprintf(buffer, "<div style=\"position: absolute; left: %dpx; top: %dpx; width: %dpx; height: %dpx; border: 0px; margin: 0px; padding: 0px\">"
		"<a href=\"%s\"><img width=\"%d\" height=\"%d\" src=\"%s?type=thumb\"></a></div>", x, y, w, h, imageUrl.c_str(), w, h, imageUrl.c_str());
	return buffer;
}

DocumentDiv::DocumentDiv(int x, int y, int w, int h, std::string fileUrl)
: Div(x, y, w, h)
, fileUrl(fileUrl)
{}

std::string DocumentDiv::toString() {
	char fileBuffer[65536];
	FILE *f = fopen(("htdocs" + fileUrl).c_str(), "rb");
	int bytes = fread(fileBuffer, 1, 65536, f);
	fclose(f);
	fileBuffer[bytes] = 0;
	char buffer[65536];
	sprintf(buffer, "<script language=\"javascript\" type=\"text/javascript\" src=\"/editarea/edit_area/edit_area_full.js\"></script>"
"<script language=\"javascript\" type=\"text/javascript\">"
"editAreaLoader.init({ id : \"textarea_1\", syntax: \"glsl\", start_highlight: true });"
"</script><div style=\"position: absolute; left: %dpx; top: %dpx; width: %dpx; height: %dpx; border: 0px; margin: 0px; padding: 0px\">"
		"<form method=\"POST\" action=\"%s\"><textarea id=\"textarea_1\" name=\"content\" cols=\"150\" rows=\"30\">%s</textarea><input type=\"submit\" value=\"Save changes\" /></form></div>", x, y, w, h, fileUrl.c_str(), fileBuffer);
	return buffer;
}

std::string DirlistDiv::toString() {
	return "";
}


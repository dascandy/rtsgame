#include "WebFont.h"
#include "Webserver.h"
#include "Font.h"
#include "ResourceManager.h"
#include <stdio.h>

class FontWebClient : public Webserver::Callback {
public:
    HttpReply handle(HttpRequest &hr) {
		if (hr.url.size() < 10) {
			std::string accum = "<html><head><title>Fonts</title></head><body>";

			char buffer[1024];
			for (std::map<std::string, Font *>::iterator it = Font::fonts.begin(); it != Font::fonts.end(); ++it) {
				sprintf(buffer, "<a href=\"/fonts/%s\">%s.glsl</a><br />", it->first.c_str(), it->first.c_str());
				accum += buffer;
			}

			accum += "</body></html>";
			return HttpReply(200, "Ok", "text/html", accum.c_str());
		} else {
			std::string name = hr.url.substr(9);
			if (Font::fonts.find(name) == Font::fonts.end())
				return HttpReply::defaultReply(404);

			Blob data = ResourceManager::Instance().storeResource<Font>(Font::fonts[name]);
			std::string datastr(data.data, data.size);
			char *buffer = new char[data.size + 4096];
			sprintf(buffer, "<html><head><title>Editing %s</title></head><body><form name=\"input\" action=\"/fonts/%s\" method=\"post\"><textarea cols=\"120\" rows=\"50\">%s</textarea><input type=\"submit\" value=\"Save\" /></form></body></html>", name.c_str(), name.c_str(), datastr.c_str());

			Blob webpage(strlen(buffer), buffer);
			return HttpReply(200, "Ok", "text/html", webpage);
		}
    }
};

static FontWebClient &getPwc() { static FontWebClient pwc; return pwc; }

void web_init_font() {
        Webserver::Instance().registerUrl("/font", &getPwc());
}


#include "WebShader.h"
#include "Webserver.h"
#include "ShaderProgram.h"
#include "ResourceManager.h"
#include <stdio.h>

class ShaderWebClient : public Webserver::Callback {
public:
    HttpReply handle(HttpRequest &hr) {
		if (hr.url.size() < 10) {
			std::string accum = "<html><head><title>Shaders</title></head><body>";

			char buffer[1024];
			for (std::map<std::string, ShaderProgram *>::iterator it = ShaderProgram::shaders.begin(); it != ShaderProgram::shaders.end(); ++it) {
				sprintf(buffer, "<a href=\"/shaders/%s\">%s.glsl</a><br />", it->second->name.c_str(), it->second->name.c_str());
				accum += buffer;
			}

			accum += "</body></html>";
			return HttpReply(200, "Ok", "text/html", accum.c_str());
		} else {
			std::string name = hr.url.substr(9);
			if (ShaderProgram::shaders.find(name) == ShaderProgram::shaders.end())
				return HttpReply::defaultReply(404);

			Blob data = ResourceManager::Instance().storeResource<ShaderProgram>(ShaderProgram::shaders[name]);
			std::string datastr(data.data, data.size);
			char *buffer = new char[data.size + 4096];
			sprintf(buffer, "<html><head><title>Editing %s</title></head><body><form name=\"input\" action=\"/shaders/%s\" method=\"post\"><textarea cols=\"120\" rows=\"50\">%s</textarea><input type=\"submit\" value=\"Save\" /></form></body></html>", name.c_str(), name.c_str(), datastr.c_str());

			Blob webpage(strlen(buffer), buffer);
			return HttpReply(200, "Ok", "text/html", webpage);
		}
    }
};

static ShaderWebClient &getPwc() { static ShaderWebClient pwc; return pwc; }

void web_init_shader() {
        Webserver::Instance().registerUrl("/shader", &getPwc());
}


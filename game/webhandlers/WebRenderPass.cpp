#include "WebRenderPass.h"
#include "Webserver.h"
#include "RenderPass.h"
#include "ResourceManager.h"
#include <stdio.h>

class RenderPassWebClient : public Webserver::Callback {
	std::vector<std::string> names;
public:
	void add(const char *name) {
		names.push_back(name);
	}
    HttpReply handle(HttpRequest &hr) {
		std::string accum = "<html><head><title>RenderPasses</title></head><body>";

		char buffer[1024];
		for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it) {
			sprintf(buffer, "<a href=\"/renderpass/%s\">%s</a><br />", it->c_str(), it->c_str());
			accum += buffer;
		}

		accum += "</body></html>";
		return HttpReply(200, "Ok", "text/html", accum.c_str());
    }
};

static RenderPassWebClient &getPwc() { static RenderPassWebClient pwc; return pwc; }

void web_init_renderpass() {
    Webserver::Instance().registerUrl("/renderpass", &getPwc());
}

void web_add_renderpass(const char *name) {
	getPwc().add(name);
}


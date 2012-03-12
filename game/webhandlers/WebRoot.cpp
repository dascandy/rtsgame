#include "WebRoot.h"
#include "Webserver.h"
#include "ResourceManager.h"
#include <stdio.h>

class RootWebClient : public Webserver::Callback {
public:
    HttpReply handle(HttpRequest &hr) {
		std::string accum = "<html><head><title>Roots</title></head><body>";
		accum += "<a href=\"/shaders/\">Shaders</a><br />";
		accum += "<a href=\"/renderpass/\">Render passes</a><br />";
		accum += "<a href=\"/fonts/\">Fonts</a><br />";
		accum += "<a href=\"/textures/\">Textures</a><br />";
		accum += "<a href=\"/profile/\">Profiling</a><br />";
		accum += "</body></html>";
		return HttpReply(200, "Ok", "text/html", accum.c_str());
    }
};

static RootWebClient &getPwc() { static RootWebClient pwc; return pwc; }

void web_init_root() {
        Webserver::Instance().registerUrl("", &getPwc());
}


#include "Textures.h"
#include "Webserver.h"
#include "Texture.h"
#include "ResourceManager.h"
#include <stdio.h>

std::string getTexturesWebsite() {
	std::string accum = "<html><head><title>Textures</title></head><body>";

	for (std::map<int, Texture *>::iterator it = Texture::textures.begin(); it != Texture::textures.end(); ++it) {
		int highestTexSize = it->second->width();
		if (it->second->height() > highestTexSize) highestTexSize = it->second->height();
		highestTexSize /= 100;
		char buffer[1024];
		sprintf(buffer, "<a href=\"/textures/%d\"><img width=\"%d\" height=\"%d\" src=\"/textures/%d?thumb\"/></a>", it->second->textureId, it->second->width() / highestTexSize, it->second->height() / highestTexSize, it->second->textureId);
		accum += buffer;
	}

	return accum + "</body></html>";
}

class TextureWebClient : public Webserver::Callback {
public:
    HttpReply handle(HttpRequest &hr) {
		if (hr.url.size() < 10) {
			return HttpReply(200, "Ok", "text/html", getTexturesWebsite().c_str());
		} else {
			int num = atoi(hr.url.substr(10).c_str());
			if (Texture::textures.find(num) == Texture::textures.end())
				return HttpReply::defaultReply(404);

			Blob data = ResourceManager::Instance().storeResource<Texture>(Texture::textures[num]);
			return HttpReply(200, "Ok", "image/png", data);
		}
    }
};

static TextureWebClient &getTwc() { static TextureWebClient twc; return twc; }

void web_init_textures() {
    Webserver::Instance().registerUrl("/textures", &getTwc());
}


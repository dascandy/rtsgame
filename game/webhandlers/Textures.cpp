#include "Textures.h"
#include "Webserver.h"
#include "DevIL.h"
#include "Texture.h"
#include <stdio.h>

std::string getTexturesWebsite() {
	std::string accum = "<html><head><title>Textures</title></head><body>";

	for (std::map<int, Texture *>::iterator it = Texture::textures.begin(); it != Texture::textures.end(); ++it) {
		char buffer[1024];
		sprintf(buffer, "<a href=\"/textures/%d\"><img src=\"/textures/%d?thumb\"/></a>", it->second->textureId, it->second->textureId);
		accum += buffer;
	}

	return accum + "</body></html>";
}

class TextureWebClient : public Webserver::Callback {
public:
        void handle(HttpRequest &hr) {
		if (hr.url.size() < 10) {
			
		} else {
			/*
			void *buffer;
			size_t buffersize;
			getCompressedTexture(id, buffer, buffersize);
			HttpReply reply(defaultReply(200));
			std::string pageContent = page.toString();
			reply.contentType = "text/html";
			reply.content = pageContent.c_str();
			reply.contentLength = pageContent.size();
			reply.writeTo(hr.fd);
			*/
		}
        }
};

static TextureWebClient &getTwc() { static TextureWebClient twc; return twc; }

void web_init_textures() {
        Webserver::Instance().registerUrl("/textures", &getTwc());
}



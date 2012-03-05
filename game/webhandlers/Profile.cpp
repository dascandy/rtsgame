#include "Profile.h"
#include "Webserver.h"
#include <stdio.h>

class ProfileWebClient : public Webserver::Callback {
public:
        HttpReply handle(HttpRequest &hr) {
		if (hr.url == "/profile/overview") {
			
		} else if (hr.url.substr(0, 14) == "/profile/graph") {
			
		}
		return HttpReply::defaultReply(501);
        }
};

static ProfileWebClient &getPwc() { static ProfileWebClient pwc; return pwc; }

void web_init_profile() {
        Webserver::Instance().registerUrl("/profile", &getPwc());
}



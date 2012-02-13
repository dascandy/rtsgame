#include "Profile.h"
#include "Webserver.h"
#include <stdio.h>

class ProfileWebClient : public Webserver::Callback {
public:
        void handle(HttpRequest &hr) {
		if (hr.url == "/profile/overview") {
			
		} else if (hr.url.substr(0, 14) == "/profile/graph") {
			
		}
        }
};

static ProfileWebClient &getPwc() { static ProfileWebClient pwc; return pwc; }

void web_init_profile() {
        Webserver::Instance().registerUrl("/profile", &getPwc());
}



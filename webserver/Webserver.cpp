#include "Webserver.h"
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

static const char *textFor(int statuscode) {
	switch(statuscode) {
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 103: return "Checkpoint";
		case 122: return "Request-URI too long";
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";
		case 226: return "IM Used";
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Switch Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Resume Incomplete";
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Request Entity Too Large";
		case 414: return "Request-URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Requested Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 420: return "Enhance Your Calm";
		case 422: return "Unprocessable Entity";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 425: return "Unordered Collection";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 444: return "No Response";
		case 449: return "Retry With";
		case 450: return "Blocked by Windows Parental Controls";
		case 499: return "Client Closed Request";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 509: return "Bandwidth Limit Exceeded";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";
		case 598: return "Network read timeout error";
		case 599: return "Network connect timeout error";
		default: return "Unknown status code";
	}
}

HttpReply defaultReply(int statuscode) {
	switch(statuscode) {
		case 204: return HttpReply(204, textFor(204), 0, 0);
		case 404: return HttpReply(404, textFor(404), "text/html", "<html><head><title>File not found</title></head><body><h1>File not found!</h1></body></html>");
		case 500: return HttpReply(500, textFor(500), "text/plain", "The server encountered an internal server error.");
		default: return HttpReply(statuscode, textFor(statuscode), "application/octet-stream", 0);
	}
}

struct extension {
	std::string extension, mimeType;
} exts[] = {
   "exe", "application/executable",
   "txt", "text/plain",
   "html", "text/html",
   "htm", "text/html",
   "css", "text/css",
   "js", "text/javascript",
   "png", "image/png",
   "jpg", "image/jpeg",
   "jpeg", "image/jpeg",
   "gif", "image/gif",
   "", ""
};

std::string getMimetype(std::string ext) {
	for (extension *f = exts; f->extension.size(); f++) {
		if (f->extension == ext) return f->mimeType;
	}
	return "application/octet-stream";
}

HttpRequest::HttpRequest() {}

static std::string readLineFrom(int fd) {
	char buffer[1024];
	int idx = 0;
	do {
		read(fd, &buffer[idx++], 1);
	} while (idx < 2 || buffer[idx-2] != '\r' && buffer[idx-1] != '\n');
	return std::string(buffer, 0, idx-2);
}

void HttpRequest::readFrom(int fd) {
	std::string requestLine = readLineFrom(fd);
	int firstSpace = requestLine.find(" ", 0);
	int secondSpace = requestLine.find(" ", firstSpace+1);
	method = requestLine.substr(0, firstSpace);
	url = requestLine.substr(firstSpace+1, secondSpace - firstSpace - 1);
	httpversion = requestLine.substr(secondSpace+1);
	std::string argLine = readLineFrom(fd);
	while (argLine.size()) {
		std::string name, arg;
		int separator = argLine.find(": ", 0);
		name = argLine.substr(0, separator);
		arg = argLine.substr(separator+2);
		attributes.insert(std::make_pair(name, arg));
	
		argLine = readLineFrom(fd);
	}

	if (url.find_first_of('?') != std::string::npos) {
		std::string args = url.substr(url.find_first_of('?')+1);
		url = url.substr(0, url.find_first_of('?'));
		while (args.size() > 0) {
			int nextAmp = args.find_first_of('&');
			std::string nextArg;
			if (nextAmp == std::string::npos) {
				nextArg = args;
				args = "";
			} else {
				nextArg = args.substr(0, nextAmp);
				args = args.substr(nextAmp+1);
			}
			std::string val = "true";
			int eq = nextArg.find_first_of('=');
			if (eq != std::string::npos) {
				val = nextArg.substr(eq+1);
				nextArg = nextArg.substr(0, eq);
			}
			arguments.insert(std::make_pair(nextArg, val));
		}
	}

	if (url[url.size()-1] == '/') url = url.substr(0, url.size()-1);
}

void HttpRequest::handle() {
	readFrom(fd);
	Webserver::Instance().handle(*this);
	// TODO: allow reuse here with keepalive
	close(fd);
}

HttpReply::HttpReply(int code, std::string text, const char *contentType, const char *content) 
: statusCode(code)
, statusText(text)
, contentType(contentType)
, content(content)
, contentLength(-1)
{
	if (content) {
		contentLength = strlen(content);
	}
}

void HttpReply::writeTo(int fd) {
	char buffer[2048];
	char linebuffer[255];
	sprintf(buffer, "HTTP/1.0 %d %s\r\n", statusCode, statusText.c_str());
	if (contentType) {
		sprintf(linebuffer, "Content-Type: %s\r\n", contentType);
		strcat(buffer, linebuffer);
	}
	if (contentLength >= 0) {
		sprintf(linebuffer, "Content-Length: %d\r\n", contentLength);
		strcat(buffer, linebuffer);
	}
	strcat(buffer, "\r\n");
	write(fd, buffer, strlen(buffer));
	if (content) {
		write(fd, content, contentLength);
	}
}

Webserver::Webserver(int port) 
{
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) { printf("Cannot create socket"); exit(-1); }
	stopped = false;
	struct sockaddr_in inaddr;
	inaddr.sin_port = htons(port);
	inaddr.sin_addr.s_addr = INADDR_ANY;
	inaddr.sin_family = AF_INET;
	int rv = bind(listenfd, (struct sockaddr *)&inaddr, sizeof(inaddr));
	if (rv < 0) { printf("Cannot bind socket: %d\n", errno); exit(-1); }
	rv = listen(listenfd, 4);
	if (listenfd < 0) { printf("Cannot listen on socket: %d\n", errno); exit(-1); }
	atexit(&stop);
	t.start(this, &Webserver::run);
}

void Webserver::stop() {
	close(Webserver::Instance().listenfd);
	Webserver::Instance().t.stop();
}

void Webserver::run() {
	while (!stopped) {
		struct sockaddr_in addr;
		unsigned int addrlen;
		int newfd = accept(listenfd, (struct sockaddr *)&addr, &addrlen);
		if (newfd < 0) break;
		HttpRequest *request = new HttpRequest();
		request->fd = newfd;
		request->t = new Thread();
		request->t->start(request, &HttpRequest::handle);
	}
}

bool matches(std::string a, std::string b) {
	printf("%s ... %s  %d\n", a.c_str(), b.c_str(), (int)b.find(a));
	if (b.find(a) == 0) return true;
	return false;
}

void Webserver::handle(HttpRequest &hr) {
	std::vector<std::pair<std::string, Webserver::Callback *> >::iterator it = urls.begin();
	for (; it != urls.end(); ++it) {
		if (matches(it->first, hr.url)) {
			try {
				it->second->handle(hr);
			} catch (...) {
				defaultReply(500).writeTo(hr.fd);
			}
			return;
		}
	}
	handleDefault(hr);
}

void Webserver::handleDefault(HttpRequest &hr) {
	std::string filename = "htdocs" + hr.url;
	struct stat data;
	int statr = stat(filename.c_str(), &data);

	if (S_ISREG(data.st_mode)) {
		std::string ext = hr.url.substr(hr.url.find_last_of('.')+1);
		std::string mimeType = getMimetype(ext);
		HttpReply reply(200, "Ok", mimeType.c_str(), 0);
		reply.contentLength = data.st_size;
		reply.writeTo(hr.fd);
		char buffer[1024];
		int rfd = open(filename.c_str(), O_RDONLY);
		int bytesRead = 0;
		do {
			bytesRead = read(rfd, buffer, sizeof(buffer));
			if (bytesRead > 0) write(hr.fd, buffer, bytesRead);
		} while (bytesRead > 0);
		close(rfd);
	} else if (S_ISDIR(data.st_mode)) {
		std::string acquiredData = "<html><head><title>Dir list of " + hr.url + "</title></head><body><table>";
		DIR *dir = opendir(filename.c_str());
		struct dirent *ent;
		while ((ent = readdir(dir))) {
			char buffer[2048];
			std::string fname = hr.url + "/" + ent->d_name;
			if (hr.url.size() == 1) fname = std::string("/") + ent->d_name;
			statr = lstat(("htdocs" + fname).c_str(), &data);
			const char *img = "/unknown.png";
			switch(ent->d_type) {
				case DT_DIR: img = "/folder.jpg"; break;
				case DT_REG: img = "/file.gif"; break;
				case DT_LNK: img = "/link.png"; break;
			}
			sprintf(buffer, "<tr><td><img width=\"20\" height=\"20\" src=\"%s\"></td><td>%d</td><td><a href=\"%s\">%s</a></td></tr>\n", img, (int)data.st_size, fname.c_str(), fname.c_str());
			acquiredData += buffer;
		}
		closedir(dir);
		acquiredData += "</table></body></html>";
		HttpReply reply(200, "Ok", "text/html", acquiredData.c_str());
		reply.writeTo(hr.fd);
	} else {
		defaultReply(404).writeTo(hr.fd);
	}
}

void Webserver::registerUrl(std::string match, Webserver::Callback *callback) {
	urls.push_back(std::make_pair(match, callback));
}



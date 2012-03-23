#include "HttpReply.h"
#include "ClientSocket.h"
#include <string.h>
#include <stdio.h>

const char *HttpReply::textFor(int statuscode) {
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

HttpReply HttpReply::defaultReply(int statuscode) {
	switch(statuscode) {
		case 204: return HttpReply(204, textFor(204), 0, "");
		case 404: return HttpReply(404, textFor(404), "text/html", "<html><head><title>Directory not found</title></head><body><h1>Directory not found!</h1></body></html>");
		case 500: return HttpReply(500, textFor(500), "text/plain", "The server encountered an internal server error.");
		default: return HttpReply(statuscode, textFor(statuscode), "application/octet-stream", "");
	}
}

static struct extension {
	const char *ext; 
	const char *mimeType;
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

const char *HttpReply::getMimetype(std::string ext) {
	for (extension *f = exts; f->ext[0]; f++) {
		if (strcmp(f->ext, ext.c_str()) == 0) return f->mimeType;
	}
	return "application/octet-stream";
}

HttpReply::HttpReply(int code, std::string text, const char *contentType, Blob content) 
: statusCode(code)
, statusText(text)
, content(content)
{
	attributes["Content-Type"] = contentType;
	attributes["Host"] = "localhost";
	attributes["Server"] = "libWebserver 0.1";
}

HttpReply::HttpReply(ClientSocket *cs) {
	readFrom(cs);
}

void HttpReply::readFrom(ClientSocket *cs) {
	
}

void HttpReply::writeTo(ClientSocket *cs) {
	char buffer[2048];
	char *bufptr = buffer;
	sprintf(bufptr, "HTTP/1.1 %d %s\r\n", statusCode, statusText.c_str());
	bufptr += strlen(bufptr);
	for (std::map<std::string, std::string>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
		sprintf(bufptr, "%s: %s\r\n", it->first.c_str(), it->second.c_str());
		bufptr += strlen(bufptr);
	}
	if (content.size >= 0) {
		sprintf(bufptr, "Content-Length: %d\r\n", (unsigned int)content.size);
		bufptr += strlen(bufptr);
	}
	strcat(bufptr, "\r\n");
	cs->write(buffer, strlen(buffer));
	if (content.size) {
		cs->write(content.data, content.size);
	}
}

HttpReply &HttpReply::setContent(Blob content) {
	this->content = content;
	return *this;
}


#include "ClientSocket.h"

#ifdef _MSC_VER
#include <winsock.h>
#else
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#endif
#include <string.h>


HALSPEC ClientSocket::ClientSocket(SOCK_TYPE fd) 
: fd(fd) {
}

HALSPEC ClientSocket::ClientSocket(std::string host, unsigned short port) {
	fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr_in;
	addr_in.sin_port = htons(port);
	addr_in.sin_family = AF_INET;

	hostent *ent = gethostbyname(host.c_str());
	memcpy((void *)&addr_in.sin_addr.s_addr, ent->h_addr_list[0], 4);

	unsigned int addrlen = sizeof(addr_in);
	connect(fd, (struct sockaddr *)&addr_in, addrlen);
}

HALSPEC int ClientSocket::read(char *buffer, int max) {
#ifdef _MSC_VER
	return ::recv(fd, buffer, max, 0);
#else
	return ::read(fd, buffer, max);
#endif
}

HALSPEC int ClientSocket::write(const char *buffer, int count) {
#ifdef _MSC_VER
	return ::send(fd, buffer, count, 0);
#else
	return ::write(fd, buffer, count);
#endif
}

HALSPEC ClientSocket::~ClientSocket() {
#ifdef _MSC_VER
	closesocket(fd);
#else
	close(fd);
#endif
}


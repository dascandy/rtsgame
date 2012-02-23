#include "ServerSocket.h"
#include <exception>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "Sockets.h"

#ifdef _MSC_VER
#include <winsock.h>
#else
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

class HALSPEC SocketException : public std::exception {
	std::string value;
public:
	SocketException(std::string value) : value(value) {}
	~SocketException() throw() {}
	const char *what() throw() { return value.c_str(); }
};

HALSPEC ServerSocket::ServerSocket(unsigned short port) {
	Sockets::Init();
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) { printf("Cannot create socket"); exit(-1); }
	struct sockaddr_in inaddr;
	inaddr.sin_port = htons(port);
	inaddr.sin_addr.s_addr = INADDR_ANY;
	inaddr.sin_family = AF_INET;
	int rv = bind(sock, (struct sockaddr *)&inaddr, sizeof(inaddr));
	if (rv < 0) { printf("Cannot bind socket: %d\n", errno); exit(-1); }
	rv = listen(sock, 4);
	if (sock < 0) { printf("Cannot listen on socket: %d\n", errno); exit(-1); }
}

HALSPEC ServerSocket::~ServerSocket() {
#ifdef _MSC_VER
	closesocket(sock);
#else
	close(sock);
#endif
}

HALSPEC ClientSocket *ServerSocket::Accept(unsigned short &portno, std::string &ipaddr) {
	struct sockaddr_in addr;
#ifdef _MSC_VER
	int addrlen;
#else
	unsigned int addrlen;
#endif
	SOCK_TYPE newfd = accept(sock, (struct sockaddr *)&addr, &addrlen);
	if (newfd <= 0) return NULL;

	portno = ntohs(addr.sin_port);
	char ipaddr_[16];
	char *raw = (char *)&addr.sin_addr.s_addr;
	sprintf(ipaddr_, "%d.%d.%d.%d", raw[0], raw[1], raw[2], raw[3]);
	ipaddr = ipaddr_;
	return new ClientSocket(newfd);
}


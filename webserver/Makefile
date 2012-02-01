
LD=g++

all: server

server: main.o Webserver.o Html.o
	$(LD) -o $@ $^ -lpthread


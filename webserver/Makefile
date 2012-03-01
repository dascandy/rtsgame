
LD=g++
CPP=g++

all: webserver.so

webserver.so: DirectoryHandler.o Html.o HttpReply.o HttpRequest.o Webserver.o Client.o
	$(LD) -shared -o $@ $^

%.o: %.cpp
	$(CPP) -I../HAL/ -fPIC -DPIC -o $@ -c $<




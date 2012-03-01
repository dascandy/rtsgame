
LD=g++
CPP=g++

all: proxywebserver

proxywebserver: main.o libHAL.so libwebserver.so
	$(LD) -o $@ $^ -L./ -lpthread -lHAL -lwebserver

%.o: %.cpp
	$(CPP) -I../HAL/ -I../webserver/ -fPIC -DPIC -o $@ -c $< -g

libHAL.so:
	cp ../HAL/HAL.so libHAL.so

libwebserver.so:
	cp ../webserver/webserver.so libwebserver.so


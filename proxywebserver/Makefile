
LD=g++
CPP=g++

all: proxywebserver

webserver.so: main.o
	$(LD) -shared -o $@ $^

%.o: %.cpp
	$(CPP) -I../HAL/ -fPIC -DPIC -o $@ -c $<




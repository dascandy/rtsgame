
LD=g++
CPP=g++

all: webserver.so

webserver.so: main.o Webserver.o Html.o
	$(LD) -shared -o $@ $^

%.o: %.cpp
	$(CPP) -fPIC -DPIC -o $@ -c $<



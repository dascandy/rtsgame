
LD=g++
CPP=g++

all: HAL.so

HAL.so: Blob.o ClientSocket.o Directory.o Entry.o File.o ServerSocket.o Sockets.o
	$(LD) -shared -o $@ $^

%.o: %.cpp
	$(CPP) -fPIC -DPIC -o $@ -c $<





MKDIR=mkdir -p
LIBS=GLEW SDL PNG
GLEWDIR=$(shell ls -d external/glew-*)
GLEWSOURCES=$(shell find $(GLEWDIR)/ -name \*.c -type f; find $(GLEWDIR)/ -name \*.h -type f; find $(GLEWDIR)/ -name \*.cpp -type f)
SDLDIR=$(shell ls -d external/SDL-*)
SDLSOURCES=$(shell find $(SDLDIR)/ -name \*.c -type f; find $(SDLDIR)/ -name \*.h -type f; find $(SDLDIR)/ -name \*.cpp -type f)
PNGDIR=$(shell ls -d external/libpng-*)
PNGSOURCES=$(shell find $(PNGDIR)/ -name \*.c -type f; find $(PNGDIR)/ -name \*.h -type f; find $(PNGDIR)/ -name \*.cpp -type f)

LIB_DEP=$(patsubst %,lib/lib%.so,$(LIBS))

all: bin/game

clean:
	$(RM) $(LIB_DEP)

bin/game: $(LIB_DEP)
	

lib/libGLEW.so: $(GLEWSOURCES)
	external/build_glew.sh

lib/libSDL.so: $(SDLSOURCES)
	external/build_sdl.sh

lib/libPNG.so: $(PNGSOURCES)
	external/build_png.sh



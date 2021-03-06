
MKDIR=mkdir -p
EXTLIBS=glew sdl png bullet ogg vorbis xz devil glm
GLEWDIR=$(shell ls -d external/glew-*)
GLEWSOURCES=$(shell find $(GLEWDIR)/ -name \*.c -type f; find $(GLEWDIR)/ -name \*.h -type f; find $(GLEWDIR)/ -name \*.cpp -type f)
SDLDIR=$(shell ls -d external/SDL-*)
SDLSOURCES=$(shell find $(SDLDIR)/ -name \*.c -type f; find $(SDLDIR)/ -name \*.h -type f; find $(SDLDIR)/ -name \*.cpp -type f)
PNGDIR=$(shell ls -d external/libpng-*)
PNGSOURCES=$(shell find $(PNGDIR)/ -name \*.c -type f; find $(PNGDIR)/ -name \*.h -type f; find $(PNGDIR)/ -name \*.cpp -type f)
OGGDIR=$(shell ls -d external/libogg-*)
OGGSOURCES=$(shell find $(OGGDIR)/ -name \*.c -type f; find $(OGGDIR)/ -name \*.h -type f; find $(OGGDIR)/ -name \*.cpp -type f)
VORBISDIR=$(shell ls -d external/libvorbis-*)
VORBISSOURCES=$(shell find $(VORBISDIR)/ -name \*.c -type f; find $(VORBISDIR)/ -name \*.h -type f; find $(VORBISDIR)/ -name \*.cpp -type f)
BULLETDIR=$(shell ls -d external/bullet-*)
BULLETSOURCES=$(shell find $(BULLETDIR)/ -name \*.c -type f; find $(BULLETDIR)/ -name \*.h -type f; find $(BULLETDIR)/ -name \*.cpp -type f)
XZDIR=$(shell ls -d external/xz-*)
XZSOURCES=$(shell find $(XZDIR)/ -name \*.c -type f; find $(XZDIR)/ -name \*.h -type f; find $(XZDIR)/ -name \*.cpp -type f)
DEVILDIR=$(shell ls -d external/devil-*)
DEVILSOURCES=$(shell find $(DEVILDIR)/ -name \*.c -type f; find $(DEVILDIR)/ -name \*.h -type f; find $(DEVILDIR)/ -name \*.cpp -type f)
GLMDIR=$(shell ls -d external/glm-*)
GLMSOURCES=$(shell find $(GLMDIR)/ -name \*.c -type f; find $(GLMDIR)/ -name \*.h -type f; find $(GLMDIR)/ -name \*.cpp -type f)
webserverDIR=webserver/
webserverSOURCES=$(shell find $(webserverDIR)/ -name \*.cpp -type f; find $(webserverDIR)/ -name \*.h -type f)
proxywebserverDIR=proxywebserver/
proxywebserverSOURCES=$(shell find $(proxywebserverDIR)/ -name \*.cpp -type f; find $(proxywebserverDIR)/ -name \*.h -type f)
HALDIR=HAL/
HALSOURCES=$(shell find $(HALDIR)/ -name \*.cpp -type f; find $(HALDIR)/ -name \*.h -type f)
LIB_DEP=$(patsubst %,lib/lib%.so,$(LIBS))

all: bin/game

clean:
	$(RM) $(LIB_DEP)

bin/game: $(patsubst %,external/%_built,$(EXTLIBS)) build/lib/libwebserver.so build/lib/libHAL.so build/bin/proxywebserver
	@$(MAKE) -C game/
	@cp game/game build/bin/game

external/glew_built: $(GLEWSOURCES)
	@echo COMPILE GLEW
	@external/build_glew.sh >external/glew.log 2>&1

external/png_built: $(PNGSOURCES)
	@echo COMPILE PNG
	@external/build_png.sh >external/png.log 2>&1

external/bullet_built: $(BULLETSOURCES)
	@echo COMPILE BULLET
	@external/build_bullet.sh >external/bullet.log 2>&1

external/ogg_built: $(OGGSOURCES)
	@echo COMPILE OGG
	@external/build_ogg.sh >external/ogg.log 2>&1

external/vorbis_built: $(VORBISSOURCES) external/ogg_built
	@echo COMPILE VORBIS
	@external/build_vorbis.sh >external/vorbis.log 2>&1

external/sdl_built: $(SDLSOURCES)
	@echo COMPILE SDL
	@external/build_sdl.sh >external/sdl.log 2>&1

external/xz_built: $(XZSOURCES)
	@echo COMPILE XZ
	@external/build_xz.sh >external/xz.log 2>&1

external/devil_built: $(DEVILSOURCES)
	@echo COMPILE DEVIL
	@external/build_devil.sh >external/devil.log 2>&1

external/glm_built: $(GLMSOURCES)
	@echo COMPILE GLM
	@external/build_glm.sh >external/glm.log 2>&1

build/lib/libwebserver.so: $(webserverSOURCES)
	@echo COMPILE WEBSERVER
	@$(MAKE) -C webserver/ >webserver.log 2>&1
	@cp webserver/webserver.so build/lib/libwebserver.so

build/bin/proxywebserver: $(proxywebserverSOURCES)
	@echo COMPILE PROXYWEBSERVER
	@$(MAKE) -C proxywebserver/ >proxywebserver.log 2>&1
	@cp proxywebserver/proxywebserver build/bin/proxywebserver

build/lib/libHAL.so: $(HALSOURCES)
	@echo COMPILE HAL
	@$(MAKE) -C HAL/ >HAL.log 2>&1
	@cp HAL/HAL.so build/lib/libHAL.so



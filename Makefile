
MKDIR=mkdir -p
LIBS=GLEW
GLEWDIR=$(shell ls -d external/glew-*)
GLEWSOURCES=$(shell find $(GLEWDIR)/ -name \*.c -type f; find $(GLEWDIR)/ -name \*.h -type f)

LIB_DEP=$(patsubst %,lib/lib%.so,$(LIBS))

all: bin/game

clean:
	$(RM) $(LIB_DEP)

bin/game: $(LIB_DEP)
	

lib/libGLEW.so: $(GLEWDIR)/lib/libGLEW.so
	$(MKDIR) lib
	cp $(GLEWDIR)/lib/libGLEW.so lib/libGLEW.so

$(GLEWDIR)/lib/libGLEW.so: $(GLEWSOURCES)
	$(MAKE) -C $(GLEWDIR)/




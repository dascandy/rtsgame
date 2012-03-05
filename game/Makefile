
CCFLAGS=-MMD -MP $(INCLUDES)
CC=g++
LD=g++
TARGET=$(shell basename `pwd`)
OBJECTS=$(patsubst %.cpp,%.o,$(shell find ./ | grep cpp$))
DEPFILES=$(patsubst %.o,%.d,$(OBJECTS))
UNMATCHED=$(filter-out $(OBJECTS) $(DEPFILES),$(shell find ./ | grep \\.d$) $(shell find ./ | grep \\.o$))
INCLUDES=\
	-I../build/include \
	-I../external/glew-1.7.0/include \
	-I../external/libpng-1.5.8 \
	-I../external/SDL-2011-01-01/include \
	-I../external/libvorbis-1.32/include \
	-I../external/libogg-1.30/include \
	-I../external/bullet-2.79/src \
	-I../HAL \
	-I../webserver \
	-Isfx \
	-Iphysics \
	-Isound \
	-Ibehaviour \
	-Ilights \
	-Icontrol \
	-Igraphics/soil \
	-Iopengl \
	-Ipng \
	-Imath \
	-Ianimation \
	-Imisc \
	-Iexceptions \
	-I. \
	-Iresources \
	-Iscene \
	-Igraphics/shaderfuncs \
	-Iview \
	-Idevil \

-include $(TARGET).conf

all: $(TARGET) $(TARGET).a
$(TARGET).a: $(OBJECTS) Makefile
	@echo AR $@
	@$(AR) -rcs $@ $(filter %.o,$^)

$(TARGET): $(OBJECTS) Makefile
	@echo LD $@ $(LDFLAGS)
	@$(LD) -o $@ $(filter %.o,$^) $(LDFLAGS)

%.o: %.cpp Makefile
	@echo CC $@
	@$(CC) $(CCFLAGS) -c -o $@ $<

clean:
	@$(RM) $(TARGET) $(TARGET).a $(OBJECTS) $(DEPFILES) $(UNMATCHED)

-include $(DEPFILES)



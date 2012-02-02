
CCFLAGS=-MMD -MP $(INCLUDES)
CC=g++
LD=g++
TARGET=$(shell basename `pwd`)
OBJECTS=$(patsubst %.cpp,%.o,$(shell find ./ | grep cpp$))
DEPFILES=$(patsubst %.o,%.d,$(OBJECTS))
UNMATCHED=$(filter-out $(OBJECTS) $(DEPFILES),$(shell find ./ | grep \\.d$) $(shell find ./ | grep \\.o$))
INCLUDES=\
	-I../external/glew-1.7.0/include \
	-I../external/libpng-1.5.8 \
	-I../external/SDL-2011-01-01/include \
	-I../external/libvorbis/include \
	-I../external/libogg/include \
	-I../webserver \
	-Isfx \
	-Iphysics \
	-Isound \
	-Ibehaviour \
	-Ilights \
	-Icontrol \
	-Igraphics/targets \
	-Igraphics/soil \
	-Igraphics/editui \
	-Igraphics/film \
	-Igraphics/opengl \
	-Ipng \
	-Imath \
	-Ianimation \
	-Imisc \
	-Iexceptions \
	-I. \
	-Iresources \
	-Iscene \
	-Igraphics/shaderfuncs \
	-I../external/bullet/src \

-include $(TARGET).conf

$(TARGET): $(OBJECTS) Makefile
	@echo LD $@ $(LDFLAGS)
	@$(LD) $(LDFLAGS) -o $@ $(filter %.o,$^)

%.o: %.cpp Makefile
	@echo CC $@
	@$(CC) $(CCFLAGS) -c -o $@ $<

clean:
	@$(RM) $(TARGET) $(OBJECTS) $(DEPFILES) $(UNMATCHED)

-include $(DEPFILES)



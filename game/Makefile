
CCFLAGS=-MMD -MP
CC=g++
LD=g++
TARGET=$(shell basename `pwd`)
OBJECTS=$(patsubst %.cpp,%.o,$(shell find ./ | grep cpp$))
DEPFILES=$(patsubst %.o,%.d,$(OBJECTS))
UNMATCHED=$(filter-out $(OBJECTS) $(DEPFILES),$(shell find ./ | grep \\.d$) $(shell find ./ | grep \\.o$))
-include $(TARGET).conf

$(TARGET): $(OBJECTS) Makefile
	@echo LD $@
	@$(LD) $(LDFLAGS) -o $@ $(filter %.o,$^)

%.o: %.cpp Makefile
	@echo CC $@
	@$(CC) $(CCFLAGS) -c -o $@ $<

clean:
	@$(RM) $(TARGET) $(OBJECTS) $(DEPFILES) $(UNMATCHED)

-include $(DEPFILES)


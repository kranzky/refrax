# vim: noet
CPP	:= g++ -Wall -O3 -Iinclude -I/usr/local/include/SDL
LINK	:= -lSDL -lSDL_mixer -lpthread -lSDLmain -framework Cocoa

TARGETS	:= bin/refrax
SOURCES	:= $(filter-out $(TARGETS:bin/%=source/%.cpp),$(wildcard source/*.cpp))
HEADERS := $(wildcard include/*.h)
OBJECTS := $(SOURCES:source/%.cpp=object/%.o)

all: ${OBJECTS} ${TARGETS}

clean:
	@echo "Cleaning up"
	-@rm -f ${OBJECTS} $(TARGETS:%=object/%.o) ${TARGETS}

object/%.o: source/%.cpp ${HEADERS}
	@echo "Compiling $<"
	@${CPP} -c $< -o $@

bin/%: object/%.o ${OBJECTS}
	@echo "Linking $@"
	${CPP} -o $@ ${OBJECTS} $< ${LINK}

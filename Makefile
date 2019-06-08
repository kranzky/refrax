# vim: noet
CPP	:= em++ -Wall -Iinclude --use-preload-plugins
DEF := -s WASM=1

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
	${CPP} -c $< ${DEF} -o $@

bin/%: object/%.o ${OBJECTS}
	@echo "Linking $@"
	${CPP} ${OBJECTS} $< ${DEF} -o $@.html --preload-file resource

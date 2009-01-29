ifeq "$(shell [ -e Makeconfig ] && echo Makeconfig)" "Makeconfig"
include Makeconfig
endif
ifndef EXEEXT
EXEEXT = 
endif
ifndef LIBEXT
LIBEXT = .so
endif
CXXFLAGS += -g -O2
LFLAGS += -L. "-Wl,-R$(shell pwd)"
files = $(wildcard *.cpp)
files := $(files:main.cpp=)

pluginfiles = plugins/test.cpp
pluginfiles += plugins/network.cpp
pluginfiles += plugins/cli.cpp
pluginfiles += plugins/log.cpp
ifdef USE_SDL
pluginfiles += plugins/sdl_display.cpp
endif

all: EvolutionBots$(EXEEXT) $(pluginfiles:.cpp=.so) docs

debug:
	CXXFLAGS="-Ddebug" make --no-print-directory

EvolutionBots$(EXEEXT): out/main.o libevolve$(LIBEXT)
	$(CXX) $(CXXFLAGS) $(LFLAGS) out/main.o -levolve -ldl -o EvolutionBots$(EXEEXT)

libevolve$(LIBEXT): $(files:%.cpp=out/%.o)
	$(CXX) -shared $(files:%.cpp=out/%.o) -o libevolve$(LIBEXT)

out/%.o: %.cpp
	@if [ ! -e out ]; then mkdir out; fi
	$(CXX) $(CXXFLAGS) -c $< -o $@

plugins/%.so: plugins/%.cpp
	cd plugins && make $(shell echo "$@" | sed -e "s/^plugins\///;")

docs: Doxyfile *.h
	doxygen > /dev/null

docs-debug: Doxyfile *.h
	doxygen

test:
	cd tests && make

clean:
	rm -rf docs out EvolutionBots$(EXEEXT) libevolve$(LIBEXT)
	cd tests && make clean
	cd plugins && make clean

distclean: clean
	rm -f Makeconfig

.PHONY: test docs clean distclean

include Makeconfig
CXXFLAGS += -g
files = $(wildcard *.cpp)

pluginfiles = plugins/test.cpp
pluginfiles += plugins/network.cpp
pluginfiles += plugins/cli.cpp
ifdef USE_SDL
pluginfiles += plugins/sdl_display.cpp
endif

all: EvolutionBots $(pluginfiles:.cpp=.so) docs

EvolutionBots: $(files:%.cpp=out/%.o)
	$(CXX) $(CXXFLAGS) $(files:%.cpp=out/%.o) -ldl -o EvolutionBots

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
	rm -rf docs out EvolutionBots
	cd tests && make clean
	cd plugins && make clean

.PHONY: test docs clean

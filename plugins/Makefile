include ../Makeconfig
files = $(wildcard *.cpp)

all: $(files:.cpp=.so)

sdl_display.so: sdl_display.cpp ../out/pointers.o
	$(CXX) $(CXXFLAGS) -shared sdl_display.cpp $(SDLFLAGS) -pthread ../out/pointers.o ../out/creature.o -o sdl_display.so

cli.so: cli.cpp ../out/pointers.o
	$(CXX) $(CXXFLAGS) -shared cli.cpp -pthread ../out/pointers.o ../out/creature.o -o cli.so

%.so: %.cpp ../out/pointers.o ../out/eventmanager.o
	$(CXX) $(CXXFLAGS) -shared $< ../out/pointers.o ../out/eventmanager.o -o $@

../out/%.o: ../%.cpp
	cd .. && make "$$(echo "$@" | sed -e "s/^..\///;")"

clean:
	rm -f $(files:.cpp=.so)

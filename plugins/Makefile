ifeq "$(shell [ -e ../Makeconfig ] && echo Makeconfig)" "Makeconfig"
include ../Makeconfig
endif
ifndef EXEEXT
EXEEXT = 
endif
ifndef LIBEXT
LIBEXT = .so
endif
files = $(wildcard *.cpp)
ifndef SDLFLAGS
SDLFLAGS = -lSDL
endif

all: $(files:.cpp=$(LIBEXT))

sdl_display$(LIBEXT): sdl_display.cpp ../libevolve$(LIBEXT)
	$(CXX) $(CXXFLAGS) -shared sdl_display.cpp $(SDLFLAGS) -pthread -L.. -levolve -o sdl_display$(LIBEXT)

%$(LIBEXT): %.cpp ../libevolve$(LIBEXT)
	$(CXX) $(CXXFLAGS) -shared $< -L.. -levolve $(shell [ -e "$(@:%$(LIBEXT)=%.flags)" ] && cat "$(@:%$(LIBEXT)=%.flags)") -o $@


../libevolve$(LIBEXT):
	cd .. && make libevolve$(LIBEXT)

clean:
	rm -f $(files:.cpp=.so)

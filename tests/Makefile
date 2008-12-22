CXXFLAGS = -g

all: notecompiling COLcompilertest notetesting tests

COLcompilertest: COLcompilertest.cpp ../out/creature.o
	$(CXX) $(CXXFLAGS) COLcompilertest.cpp ../out/creature.o -o $@

tests:
	@echo "Test-compiling the COL code \"F0:0D\\n2:7f\""
	./COLcompilertest

notecompiling:
	@echo "Compiling test applications..."

notetesting:
	@echo "Applications compiled, running tests:"

../out/creature.o: ../creature.cpp
	cd .. && make out/creature.o

clean:
	rm -f COLcompilertest

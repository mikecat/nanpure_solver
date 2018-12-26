CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic
LDFLAGS=-s

.PHONY: all
all: NanpureSolverDriver NanpureGeneratorDriver

NanpureSolverDriver: NanpureBoard.o NanpureSolver.o NanpureSolverDriver.o
	$(CXX) $(LDFLAGS) -o $@ $^

NanpureGeneratorDriver: NanpureBoard.o NanpureSolver.o NanpureGenerator.o NanpureGeneratorDriver.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -f NanpureSolverDriver NanpureGeneratorDriver *.o

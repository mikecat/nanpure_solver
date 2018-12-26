CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic -std=c++11
LDFLAGS=-s

.PHONY: all
all: NanpureSolverDriver NanpureGeneratorDriver NanpureGeneratorXorshiftDriver

NanpureSolverDriver: NanpureBoard.o NanpureSolver.o NanpureSolverDriver.o
	$(CXX) $(LDFLAGS) -o $@ $^

NanpureGeneratorDriver: NanpureBoard.o NanpureSolver.o NanpureGenerator.o NanpureGeneratorDriver.o
	$(CXX) $(LDFLAGS) -o $@ $^

NanpureGeneratorXorshiftDriver: NanpureBoard.o NanpureSolver.o NanpureGenerator.o NanpureGeneratorXorshiftDriver.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -f NanpureSolverDriver NanpureGeneratorDriver NanpureGeneratorXorshiftDriver *.o

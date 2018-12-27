CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic -std=c++11 -DUSE_ZLIB
LDFLAGS=-s

.PHONY: all
all: NanpureSolverDriver NanpureGeneratorDriver NanpureGeneratorXorshiftDriver

NanpureSolverDriver: NanpureBoard.o NanpureSolver.o NanpureSolverDriver.o
	$(CXX) $(LDFLAGS) -o $@ $^

NanpureGeneratorDriver: NanpureBoard.o NanpureSolver.o NanpureGenerator.o NanpureGeneratorDriver.o
	$(CXX) $(LDFLAGS) -o $@ $^

NanpureGeneratorXorshiftDriver: NanpureBoard.o NanpureSolver.o NanpureGenerator.o NanpureGeneratorXorshiftDriver.o
	$(CXX) $(LDFLAGS) -o $@ $^ -lz

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -f NanpureSolverDriver NanpureGeneratorDriver NanpureGeneratorXorshiftDriver *.o

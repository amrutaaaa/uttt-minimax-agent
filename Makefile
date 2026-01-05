.PHONY: clean

CXXFLAGS=-Wall

all: run.out

run.out: src/main.cpp src/board.cpp
	g++ $(CXXFLAGS) src/main.cpp -o $@

clean:
	rm -rf bin/ *.out
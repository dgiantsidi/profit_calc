export CXX = g++

target: all main run clean

all: run

main: clean
	$(CXX) -Wall -O3 -std=c++2a -std=gnu++2a $@.cpp -o $@ -lfmt

run: main
	./$^

clean:
	rm -f main

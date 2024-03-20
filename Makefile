export CXX = g++

target: all main

all: 

main: main.cpp
	$(CXX) -Wall $@.cpp -o $@ -lfmt

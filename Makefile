FILES=src/*.cpp
OUTFILE=bin/orng
CFLAGS=-std=c++17 -o $(OUTFILE)
LFLAGS=-ledit

all:
	g++ $(CFLAGS) $(FILES) $(LFLAGS)
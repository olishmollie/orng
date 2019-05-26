FILES=*.cpp
CFLAGS=-std=c++17
LFLAGS=-ledit

all:
	g++ $(CFLAGS) $(FILES) $(LFLAGS)
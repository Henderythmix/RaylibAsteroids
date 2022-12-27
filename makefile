LIBS=-O3 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -std=c++2a

default: main

engine: src/DEngine/*.cpp
	g++ src/DEngine/*.cpp -c $(LIBS)

main: src/main.cpp
	g++ src/main.cpp -o bin/game.exe $(LIBS) -Wvexing-parse -g -fpermissive

run:
	./bin/game.exe
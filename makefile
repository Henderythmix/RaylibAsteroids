WinParams=-mwindows -Wvexing-parse -lopengl32 -lgdi32 -lwinmm
LIBS=-O3 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -std=c++2a -g $(WinParams)

default: build

engine: src/DEngine/*.cpp
	g++ src/DEngine/*.cpp -c $(LIBS)

title: src/title.cpp
	g++ src/title.cpp -c $(LIBS)

game: src/game.cpp
	g++ src/game.cpp -c $(LIBS)

main:: src/main.cpp
	g++ src/main.cpp -c $(LIBS)

build: main engine title game
	g++ ./*.o -o bin/game.exe $(LIBS) -fpermissive

clean:
	rm ./*.o

run:
	./bin/game.exe
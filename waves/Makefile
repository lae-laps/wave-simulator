LIBS=-lsfml-graphics -lsfml-window -lsfml-system
FLAGS=-Wall

all:

	@echo " * building "

	g++ src/main.cpp -c -o build/main.o
	g++ -o main build/main.o $(LIBS)

warn:

	@echo " * warnings on "
	
	g++ $(FLAGS) src/main.cpp -c -o build/main.o
	g++ $(FLAGS) -o main build/main.o $(LIBS)

clean:
	rm main
	rm -rf build/*

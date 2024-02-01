install:
	sudo apt-get install libzmq3-dev

graphic: graphic.bat
	cmd.exe /c "start /min graphic.bat" &

build: liste_chaine.c file.c piece.c jeu.c main.c
	gcc -Wall -Wextra -fsanitize=address -lzmq -o main liste_chaine.c file.c piece.c abr.c jeu.c resolution.c main.c

run: main
	./main

all: build run

clean:
	rm -f main

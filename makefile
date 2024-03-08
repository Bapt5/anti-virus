install:
	sudo apt-get install libzmq3-dev

graphic: graphic.bat
	cmd.exe /c "start /min graphic.bat" &

build_warning: egalite.c liste_chaine.c file.c piece.c abr.c jeu.c resolution.c main.c
	gcc -Wall -Wextra -fsanitize=address -lzmq -o main.out egalite.c liste_chaine.c file.c piece.c abr.c jeu.c resolution.c main.c

build: egalite.c liste_chaine.c file.c piece.c abr.c jeu.c resolution.c main.c
	gcc -w -fsanitize=address -lzmq -o main.out egalite.c liste_chaine.c file.c piece.c abr.c jeu.c resolution.c main.c

run: main.out 
	./main.out $(MFLAGS)

all: build run

clean:
	rm -f main

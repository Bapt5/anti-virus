install:
	sudo apt-get install libzmq3-dev

graphic: graphic.cmd
	cmd.exe /c "start /min graphic.cmd" &

build_warning: source/egalite.c source/liste_chaine.c source/file.c source/piece.c source/abr.c source/tas.c source/jeu.c source/recherche.c source/resolution.c source/creation.c source/similarite.c source/ia.c source/main.c
	gcc -Wall -Wextra -fsanitize=address -lzmq -o main.out source/egalite.c source/liste_chaine.c source/file.c source/piece.c source/abr.c source/tas.c source/jeu.c source/recherche.c source/resolution.c source/creation.c source/similarite.c source/ia.c source/main.c

build: source/egalite.c source/liste_chaine.c source/file.c source/piece.c source/abr.c source/tas.c source/jeu.c source/recherche.c source/resolution.c source/creation.c source/similarite.c source/ia.c source/main.c
	gcc -w -o main.out source/egalite.c source/liste_chaine.c source/file.c source/piece.c source/abr.c source/tas.c source/jeu.c source/recherche.c source/resolution.c source/creation.c source/similarite.c source/ia.c source/main.c -lzmq

run: main.out 
	./main.out

all: build run

clean:
	rm -f main

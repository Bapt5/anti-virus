install:
	sudo apt-get install libzmq3-dev

graphic: graphic.bat
	cmd.exe /c "start /min graphic.bat" &

build_warning: source/egalite.c source/liste_chaine.c source/file.c source/piece.c source/abr.c source/tas_min.c jeu.c source/recherche.c source/resolution.c source/main.c
	gcc -Wall -Wextra -fsanitize=address -lzmq -o main.out source/egalite.c source/liste_chaine.c source/file.c source/piece.c source/abr.c source/tas_min.c source/jeu.c source/recherche.c source/resolution.c source/main.c

build: source/egalite.c source/liste_chaine.c source/file.c source/piece.c source/abr.c source/tas_min.c source/jeu.c source/recherche.c source/resolution.c source/main.c
	gcc -w -fsanitize=address -lzmq -o main.out source/egalite.c source/liste_chaine.c source/file.c source/piece.c source/abr.c source/tas_min.c source/jeu.c source/recherche.c source/resolution.c source/main.c

run: main.out 
	./main.out

all: build run

clean:
	rm -f main

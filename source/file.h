#ifndef _FILE_
#define _FILE_

#include "librairies.h"
#include "liste_chaine.h"

struct file_s {
	maillon* premier;
	maillon* dernier;
};
typedef struct file_s file;

file creer_file(void);
bool est_vide_file(file);
void enfiler(void*, file*);
void* defiler(file*);
void free_file(file*, void (*)(void*));

#endif

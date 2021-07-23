#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "huffmanModulos.h" 

int main(int argc, char* argv[]){
	if (argc != 4){
		comoUsar();
		return 1;
	}
	if (strcmp(argv[1], "-c")==0){
		comprimeArquivo(argv[2], argv[3]);
	} else{
		descomprimeArquivo(argv[2], argv[3]);
	}
	return 0;
}
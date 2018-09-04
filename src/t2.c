#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "controle.h"
#include "arquivos.h"

struct reg{

	char *path_entrada;
	char *seg_entrada;
	char *entrada;
	char *saida;
};



void main(int argc, const char *argv[]){

	reg *arquivos_path = malloc(sizeof(reg));
	char comando[50];

	//args
	arquivos_path = argumentos(arquivos_path, argc, argv);

	//le arquivo
	ler(arquivos_path,argc,argv);

}
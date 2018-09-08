#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "controle.h"
#include "arquivos.h"
#include "formas.h"
#include "set_var.h"
#include "arqs_svg.h"
#include "city_consultas.h"
#include "op.h"

struct reg{

	char *path_entrada;
	char *seg_entrada;
	char *ec_entrada;
	char *pm_entrada;
	char *entrada;
	char *saida;
};

struct forma{

	int id;
	char tipo;
	float tam[2];
	char cor[20];
	char borda[20];
	float pos[2];
	float centro[2];
	int sobrep;
	
};

struct cor{

	char *fillQ;
	char *bordaQ;
	char *fillH;
	char *bordaH;
	char *fillT;
	char *bordaT;
	char *fillS;
	char *bordaS;

};

struct eqp{
    char *id;
	char tipo;
	float pos[2];
	float tam[2];
    char *fill;
	char *stroke;
    struct eqp *esq;
	struct eqp *dir;
};

struct EC{
	char *CNPJ;
	char tipo;
	char *CEP;
	char face;
	int num;
	char *nome;
	struct EC *dir;
	struct EC *esq;
};

struct ECtipo{
	char *tipo;
	char *descript;
	struct EC *lista;
	struct ECtipo *dir;
	struct ECtipo *esq;
};

struct structure{

	struct eqp *tree;
	struct eqp *quadras;
	struct eqp *torres;
	struct ECtipo *type;
	struct EC *treeEC;
};


reg *path_treat(reg *arquivos_path, int aux){

	int tam,i;

	if(aux==1){
		//barra fim da entrada
		tam = strlen(arquivos_path->path_entrada);
		if(arquivos_path->path_entrada[tam]!='/'){
			arquivos_path->path_entrada[tam] = '/';
		}
	}else if(aux == 2){
		//barra fim da saida
		tam = strlen(arquivos_path->saida);
		if(arquivos_path->saida[tam]!='/'){
			arquivos_path->saida[tam] = '/';
		}
	}
	return arquivos_path;

}

void lerEC(structure *arvores, char *path, char *name){
	FILE *arq;
	char *entrada;
	char comando[100];

	ECtipo *est_comerciais = NULL;

	entrada = calloc(strlen(path)+strlen(name)+2,sizeof(char));
	strcpy(entrada,path);
	strcat(entrada,name);

	arq = fopen(entrada,"r");
	if(arq == NULL){
         printf("erro na abertura do arquivo .ec de entrada\n");
    }else{
    	while(1){
    		fgets(comando,sizeof(comando),arq);
    		if(feof(arq)){
    			break;
    		}
    		controlEC(arvores,est_comerciais,comando);
    	}
    }	fclose(arq);

}

void ler_2(reg *arquivos_path,cor *cores,structure *arvores){

	FILE *arq;
	char *entrada, comando[100], *saida;
	forma *formas;

	formas = criar(1);

	entrada = calloc(strlen(arquivos_path->path_entrada)+strlen(arquivos_path->seg_entrada)+2,sizeof(char));
	strcpy(entrada,arquivos_path->path_entrada);
	strcat(entrada,arquivos_path->seg_entrada);

	arq = fopen(entrada,"r");
	if(arq == NULL){
         printf("erro na abertura do arquivo .qry de entrada\n");
    }else{
    	while(1){
    		fgets(comando,sizeof(comando),arq);
    		if(feof(arq)){
    			break;
    		}
    		control2(arquivos_path,comando,cores,formas,arvores);
    	}
    }	fclose(arq);
    saida = gera_saida(arquivos_path,2);
    mapa(arvores->tree,cores,saida);

}

void ler(reg *arquivos_path,int argc, const char *argv[]){

	FILE *arq;
	forma *formas;
	cor *cores;
	char comando[100], *entrada;
	structure *arvores = malloc(sizeof(structure));
	
	arvores->tree = malloc(sizeof(eqp));
	arvores->tree->id = NULL;
	arvores->quadras = malloc(sizeof(eqp));
	arvores->quadras->id = NULL;
	arvores->torres = malloc(sizeof(eqp));
	arvores->torres->id = NULL;

	cores = malloc(sizeof(cor));

	entrada = malloc((strlen(arquivos_path->path_entrada)+strlen(arquivos_path->entrada)+1)*sizeof(char));
	entrada = strcpy(entrada,arquivos_path->path_entrada);
	strcat(entrada,arquivos_path->entrada);

	arq = fopen(entrada,"r");
		if(arq == NULL){
         	printf("erro na abertura do arquivo de entrada\n");
     	}else{
     		formas = criar(-1);
        	while(fgets(comando,sizeof(comando),arq) != NULL){
        		if(strcmp(comando,"#")==0){
        			control(arquivos_path,comando,formas,cores,arvores);
        			break;
        		}
        		control(arquivos_path,comando,formas,cores,arvores);
        	}
        	fclose(arq);
     	}

	//le outros arquivos
    if(arquivos_path->seg_entrada!=NULL){
    	ler_2(arquivos_path,cores,arvores);
    }

	if(arquivos_path->ec_entrada!=NULL){
		lerEC(arvores,arquivos_path->entrada,arquivos_path->ec_entrada);
	}
	if(arquivos_path->pm_entrada!=NULL){
		//lerPM();
	}
}


void consultas(reg *arquivos_path, char *comando, forma *formas, char *parm){

	FILE *txt;
	int i,j,w;
	char *saida;

	saida = defsaida(arquivos_path);

	int tam = strlen(saida);
	for(i=tam;saida[i]!='.';i--);
	saida[i+1] = 't';
	saida[i+2] = 'x';
	saida[i+3] = 't';

	i = transform(&comando[2]);
	w = posic(comando,2);
	j = transform(&comando[w+1]);

	i = getpos(formas,i);
	j = getpos(formas,j);

	if(strcmp(parm,"-1")==0){
		txt = fopen(saida,"a");
		if(txt==NULL){
			printf("erro na abertura do arquivo txt\n");
		}
		fprintf(txt, "\n%s", comando);
		if(formas[i].sobrep == j){
			fprintf(txt,"%s\n","SIM");
		}else{
			fprintf(txt,"%s\n","NÃO");
		}
		fclose(txt);
	}else{
		txt = fopen(saida,"a");
		fprintf(txt, "\n%s", comando);
		fprintf(txt, "%s\n", parm);
		fclose(txt);
	}

}

char *geraSVG(reg *arquivos_path,forma *formas,char *comando){

	int i, princ;
	char *sufixo, *saida;

	//tratamento comando
	princ = transform(&comando[2]);
	i = posic(comando,2);
	sufixo = &comando[i+1];

	for(i=0;sufixo[i]!='\n';i++);
	sufixo[i] = '\0';

	saida = calloc(strlen(arquivos_path->saida)+strlen(sufixo)+strlen(arquivos_path->entrada)+1,sizeof(char));

	strcpy(saida,arquivos_path->saida);
	strcat(saida,arquivos_path->entrada);

	for(i=1;saida[i]!='\0';i++);

	saida[i-1] = '\0';
	saida[i-2] = '\0';
	saida[i-3] = '\0';
	saida[i-4] = '-';

	strcat(saida,sufixo);
	strcat(saida,".svg");

	return saida;

}




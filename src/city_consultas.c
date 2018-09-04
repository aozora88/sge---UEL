#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "formas.h"
#include "set_var.h"
#include "op.h"
#include "city_consultas.h"
#include "arqs_svg.h"
#include "kd_tree.h"

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

struct quadra{

	char *cep;
	float pos[2];
	float tam[2];
	quadra *next;
	quadra *before;
	char *fill;
	char *stroke;

};

struct reg{

	char *path_entrada;
	char *seg_entrada;
	char *ec_entrada;
	char *pm_entrada;
	char *entrada;
	char *saida;
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
	//struct eqp *pai;
};

/*
struct structure{

	struct eqp *tree;
	struct eqp *quadras;
	struct eqp *equips;
	struct eqp *torres;

};
*/

char *get_cor(eqp *est,cor *cores, int c){
		char *color[2];

		if(est->fill == NULL){
			if(est->tipo=='h'){
				color[0] = cores->fillH;
				color[1] = cores->bordaH;
			}else if(est->tipo=='s'){
				color[0] = cores->fillS;
				color[1] = cores->bordaS;
			}else{
				color[0] = cores->fillT;
				color[1] = cores->bordaT;
			}
		}else{
			color[0] = est->fill;
			color[1] = est->stroke;
		}

		return color[0];
}

void delimita(forma *formas, reg *end, char *comando,int i){
	char *saida, *out, *cd;

	cd = gera_comando(comando,i);
	formas = add(cd,formas);

	out = gera_saida(end,2);

	print_delimita(out,formas);
}


void rangeSearch(eqp **tree,eqp **root,forma *area,cor *cores,char kind,int key, int i, char *saida){
	int flag;
	int flag2 = 0;
	int auxi = 0;
	eqp *aux;

	if(kind==(*tree)->tipo || kind=='n'){
		flag = loop(area,(*tree)->pos[0],(*tree)->pos[1]);
		if((*tree)->tipo=='q'){
			flag2 = loop(area,(*tree)->pos[0]+(*tree)->tam[0],(*tree)->pos[1]+(*tree)->tam[1]);	
		}
	}

	if(flag==0 || flag2==0){
		if((*tree)->pos[key] > area->pos[key]+area->tam[key]){
			if((*tree)->esq!=NULL){
				rangeSearch(&(*tree)->esq,root,area,cores,kind,!key,i,saida);
				return;
			}
		}else if((*tree)->pos[key] < area->pos[key]){
			if((*tree)->dir!=NULL){
				rangeSearch(&(*tree)->dir,root,area,cores,kind,!key,i,saida);
				return;
			}
		}
	}

	if(flag==1){
		reporta(*tree,cores,saida);
		if(i==1){
			removeNo(root,tree);
		}
	}

	if((*tree)->esq!=NULL){
		rangeSearch(&(*tree)->esq,root,area,cores,kind,!key,i,saida);
	}

	if((*tree)->dir!=NULL){
		rangeSearch(&(*tree)->dir,root,area,cores,kind,!key,i,saida);
	}

}


int loop(forma *formas,float x, float y){
	char *cq, *boo;

	cq = gera_comando_i(x,y);
	boo = interno(cq,formas);
	
	if(boo[0]=='S'){
		return 1;
	}else{
		return 0;
	}
}

void reporta(eqp *lista, cor *cores, char *saida){
	FILE *text;
	char *figure, *corF, *bordaS;

	text = fopen(saida,"a");

		if(lista->tipo=='q'){
			figure = calloc(7,sizeof(char));
			strcpy(figure,"quadra");
			corF = cores->fillQ;
			bordaS = cores->bordaQ;
		}else if(lista->tipo=='h'){
			figure = calloc(9,sizeof(char));
			strcpy(figure,"hidrante");
			corF = cores->fillH;
			bordaS = cores->bordaH;
		}else if(lista->tipo=='s'){
			figure = calloc(9,sizeof(char));
			strcpy(figure,"semaforo");
			corF = cores->fillS;
			bordaS = cores->bordaS;		
		}else if(lista->tipo=='t'){
			figure = calloc(5,sizeof(char));
			strcpy(figure,"torre");
			corF = cores->fillT;
			bordaS = cores->bordaT;
		}

		if(lista->fill!=NULL){
			corF = lista->fill;
			bordaS = lista->stroke;
		}

		//printf("%p\n",lista);
		//printf("%p %s %c %f %f %f %f %p %p \n",&lista,lista->id, lista->tipo, lista->pos[0], lista->pos[1], lista->tam[0], lista->tam[1], lista->esq, lista->dir);
		fprintf(text,"%s: cep/id = %s posx = %f posy = %f width = %f hight = %f cor = %s borda = %s\n ",figure,lista->id,lista->pos[0],lista->pos[1],lista->tam[0],lista->tam[1],corF,bordaS);


	fclose(text);
}

void mapa(eqp *tree,cor *cores,char *saida){
	char *aux;
	FILE *svg;
	char i = 0;

	svg = fopen(saida,"r");
	if(svg==NULL){
		i = 1;
	}else{
		fclose(svg);
	}

	svg = fopen(saida,"a");
	if(svg==NULL){
		printf("falha ao criar svg\n");
	}
	if(i==1){
		fprintf(svg,"<svg xmlns=\"http://www.w3.org/2000/svg\">");
	}
		
	printTreeSVG(tree,cores,svg);
		
	fprintf(svg,"\n</svg>");
	fclose(svg);

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arqs_svg.h"
#include "op.h"
#include "set_var.h"
#include "kd_tree.h"

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

void desenha_a(forma *formas, char *saida, char *comando){
	FILE *svg;
	int id, i;
	float dist;

	id = transform(&comando[2]);

	svg = fopen(saida,"a");
	for(i=0;formas[i].tipo!='f';i++){
		if(formas[i].tipo != 'n' && i!=id){
			fprintf(svg, "\n<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" style=\"stroke:%s;stroke-width:2\"/>", formas[id].centro[0], formas[id].centro[1], formas[i].centro[0], formas[i].centro[1], formas[id].borda);
			dist = calcula_dist(formas,id,i);
			fprintf(svg, "\n<text x=\"%f\" y=\"%f\" style=\"fill:black;font-size:10.0px;font-family:arial\">%.3f</text>", formas[i].centro[0]+3, formas[i].centro[1], dist);	
		}
	}
	fprintf(svg, "\n</svg>");
	fclose(svg);

}

void desenha_sobrep(forma *formas, char *saida){

	FILE *svg;
	float *f1;
	float *f2;
	float *sizes;
	int i;
	int c;

	c = -1;

	svg = fopen(saida,"a");

	for(i=0;formas[i].tipo!='f';i++){
		if(formas[i].sobrep!=-1 && i!=c){
			f1 = intervalos(formas,i);
			f2 = intervalos(formas,formas[i].sobrep);
			sizes = tam_sobrep(f1,f2);
			fprintf(svg,"\n<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:none;stroke-width:3;stroke:purple;stroke-dasharray:3;opacity:1.0\"/>",sizes[0], sizes[1], sizes[2], sizes[3]);
			fprintf(svg, "\n<text x=\"%f\" y=\"%f\" style=\"fill:purple;font-size:7.0px;font-family:sans-serif\">SOBREPÕE</text>", sizes[0], sizes[1]-4);
			c = formas[i].sobrep;
		}
	}
	//fprintf(svg,"\n</svg>");
	fclose(svg);

}

void print_delimita(char *out,forma *formas){
	FILE *p;
	int i,c = 0;

	p = fopen(out,"r");
	if(p==NULL){
		c = 1;
	}else{
		fclose(p);
	}

	p = fopen(out,"a");
	if(p==NULL){
			printf("falha ao criar segundo svg\n");
	}else{

	if(c==1){
		fprintf(p,"<svg xmlns=\"http://www.w3.org/2000/svg\">");
	}
 		if(formas[0].tipo == 'c'){
 			fprintf(p,"\n<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" stroke-dasharray=\"3\" opacity=\"1.0\"/>",formas[0].pos[0],formas[0].pos[1], formas[0].tam[0], formas[0].borda, formas[0].cor);
 		}else if(formas[0].tipo == 'r'){
 			fprintf(p,"\n<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:%s;stroke-width:1;stroke:%s;stroke-dasharray:3;opacity:1.0\"/>",formas[0].pos[0],formas[0].pos[1], formas[0].tam[0], formas[0].tam[1], formas[0].cor, formas[0].borda);

 		}
 	
 	fclose(p);
 }

}


void fecha_svg(char *saida,forma *formas){

	int i;
	FILE *svg;

	svg = fopen(saida,"w");
	if(svg==NULL){
		printf("falha ao criar arquivo svg\n");
	}

	fprintf(svg,"<svg xmlns=\"http://www.w3.org/2000/svg\">");

	for(i=0;formas[i].tipo!='f';i++){
 		if(formas[i].tipo == 'c'){
 			fprintf(svg,"\n<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" opacity=\"0.7\"/>",formas[i].pos[0],formas[i].pos[1], formas[i].tam[0], formas[i].borda, formas[i].cor);
 		}else if(formas[i].tipo == 'r'){
 			fprintf(svg,"\n<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:%s;stroke-width:1;stroke:%s;opacity:1.0\"/>",formas[i].pos[0],formas[i].pos[1], formas[i].tam[0], formas[i].tam[1], formas[i].cor, formas[i].borda);

 		}
 	}
 	fclose(svg);

}




char *defsaida(reg *arquivos_path){

	int i;
	char *saida;

	i = strlen(arquivos_path->saida)+strlen(arquivos_path->entrada);
	saida = calloc(i,sizeof(char));
	strcpy(saida,arquivos_path->saida);
	strcat(saida,arquivos_path->entrada);

	saida[i-1] = 'g';
	saida[i-2] = 'v';
	saida[i-3] = 's';

	return saida;
}

void muda_cor(eqp **urbanEQP,char *comando){
	eqp *auxe;
	char *info;
	int id,c;

	info = takeword(&comando[3]);

	
	auxe = procuraID(*urbanEQP,info);
	if(auxe==NULL){
		printf("equipamento nao encontrado pelo comando cc\n");
	}else{
		id = posic(comando,3);
		c = posic(comando,id+1);
		auxe->stroke = malloc((c-id)*sizeof(char));
		strcpy(auxe->stroke , takeword(&comando[id+1]));
		info = &comando[c+1];
		auxe->fill = malloc(strlen(info)*sizeof(char));
		strcpy(auxe->fill , info);
		strcpy(auxe->fill , tiran(auxe->fill));	
	}

}

void printTreeSVG(eqp *tree, cor *cores, FILE *saida){

	////////////////////////////////////////////////
	if(tree->tipo=='h'){
		if(tree->fill==NULL){
			fprintf(saida,"\n<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" opacity=\"0.7\"/>",tree->pos[0],tree->pos[1],tree->tam[0],cores->fillH,cores->bordaH);
		}else{
			fprintf(saida,"\n<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" opacity=\"0.7\"/>",tree->pos[0],tree->pos[1],tree->tam[0],tree->fill,tree->stroke);			
		}
	}else if(tree->tipo=='s'){
		if(tree->fill==NULL){
			fprintf(saida,"\n<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" opacity=\"0.7\"/>",tree->pos[0],tree->pos[1],tree->tam[0],cores->fillS,cores->bordaS);
		}else{
			fprintf(saida,"\n<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" opacity=\"0.7\"/>",tree->pos[0],tree->pos[1],tree->tam[0],tree->fill,tree->stroke);			
		}
	}else if(tree->tipo=='t'){
		if(tree->fill==NULL){
			fprintf(saida,"\n<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" opacity=\"0.7\"/>",tree->pos[0],tree->pos[1],tree->tam[0],cores->fillT,cores->bordaT);
		}else{
			fprintf(saida,"\n<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" opacity=\"0.7\"/>",tree->pos[0],tree->pos[1],tree->tam[0],tree->fill,tree->stroke);			
		}
	}else{
		if(tree->fill==NULL){
			fprintf(saida,"\n<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:%s;stroke-width:1;stroke:%s;opacity:0.7\"/>",tree->pos[0],tree->pos[1],tree->tam[0],tree->tam[1],cores->fillQ,cores->bordaQ);
		}else{
			fprintf(saida,"\n<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:%s;stroke-width:1;stroke:%s;opacity:0.7\"/>",tree->pos[0],tree->pos[1],tree->tam[0],tree->tam[1],tree->fill,tree->stroke);
		}
	}
	////////////////////////////////////////////////

	if(tree->esq != NULL){
		printTreeSVG(tree->esq,cores,saida);
	}

	if(tree->dir != NULL){
		printTreeSVG(tree->dir,cores,saida);
	}

	return;
}

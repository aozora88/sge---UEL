#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "set_var.h"
#include "op.h"

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

int getpos(forma *formas,int id){
	int i;

	for(i=0;formas[i].tipo!='f';i++){
		if(formas[i].id==id && formas[i].tipo!='n'){
			break;
		}
	}

	return i;

}

int defID(forma *formas){

	int i;

	for(i=0;formas[i].tipo!='f';i++){
		if(formas[i].tipo=='n'){
			break;
		}
	}

	return i;

}

float *tam_sobrep(float *f1, float *f2){
	float *sizes;

	sizes = calloc(4,sizeof(float));


	if(f1[0] >= f2[0]){ 
		sizes[0] = f2[0]-3;
	}else{
		sizes[0] = f1[0]-3;
	}

	if(f1[2] >= f2[2]){ 
		sizes[1] = f2[2]-3;
	}else{
		sizes[1] = f1[2]-3;
	}

	if(f1[1] >= f2[1]){
		sizes[2] = f1[1] - sizes[0]+3;
	}else{
		sizes[2] = f2[1] - sizes[0]+3;
	}

	if(f1[3] >= f2[3]){
		sizes[3] = f1[3] - sizes[1]+3;
	}else{
		sizes[3] = f2[3] - sizes[1]+3;
	}


	return sizes;
}

float *intervalos(forma *formas, int i){

	float *P;

	P = calloc(4,sizeof(float));

	if(formas[i].tipo=='c'){
		P[0] = formas[i].pos[0] - formas[i].tam[0];
		P[1] = formas[i].pos[0] + formas[i].tam[0];
		P[2] = formas[i].pos[1] - formas[i].tam[0];
		P[3] = formas[i].pos[1] + formas[i].tam[0];
	}else{
		P[0] = formas[i].pos[0];
		P[1] = formas[i].pos[0] + formas[i].tam[0];
		P[2] = formas[i].pos[1];
		P[3] = formas[i].pos[1] + formas[i].tam[1];
	}

	return P;
}


forma *calc_sob(char *comando, forma *formas){


	int i,j, C, w;
	float *P, *S, dist, x, y;
	C = 0;

	i = transform(&comando[2]);
	w = posic(comando, 2);
	j = transform(&comando[w+1]);

	i = getpos(formas,i);
	j = getpos(formas,j);

	if(formas[i].pos[0] >= formas[j].pos[0]){
		P = intervalos(formas, j);
		S = intervalos(formas, i);
	}else{
		S = intervalos(formas, i);
		P = intervalos(formas, j);
	}


	if(formas[i].tipo == 'c' && formas[j].tipo == 'c'){
		dist = calcula_dist(formas,i,j);
		if(dist <= formas[i].tam[0]+formas[j].tam[0]){
			C = 1;
		}
	}else if(formas[i].tipo=='r'&&formas[j].tipo=='r'){
		/*printf("P: %f %f %f %f\n",P[0],P[1],P[2],P[3]);
		printf("S: %f %f %f %f\n",S[0],S[1],S[2],S[3]);*/
		if(P[0] >= S[0] && P[0] <= S[1]){
			if(P[2] <= S[3] && P[3] >= S[2]){
				C = 1;
			}else if(P[2] <= S[2] && P[3] >= S[3]){
				C = 1;
			}
		}
	}else{
		if(formas[j].tipo == 'c'){
			w = i;
			i = j;
			j = w;
		}

		x = max(formas[j].pos[0], min(formas[i].pos[0], formas[j].pos[0] + formas[j].tam[0]));
		y = max(formas[j].pos[1], min(formas[i].pos[1], formas[j].pos[1] + formas[j].tam[1]));
		dist = sqrt(pow(abs(formas[i].centro[0]-x),2)+pow(abs(formas[i].centro[1]-y),2));
		if(dist*dist <= pow(formas[i].tam[0],2)){
			C = 1;
		}
	}

	if(C==1){
		formas[i].sobrep = j;
		formas[j].sobrep = i;
	}


	return formas;
}

char *interno(char *comando, forma *formas){
	char *booleano;
	int i,j;
	float ponto[2], *area, dist;

	booleano = calloc(4,sizeof(char));
	strcpy(booleano,"NÃO\n");

	i = transform(&comando[2]);
	j = posic(comando,2);
	ponto[0] = transformF(&comando[j+1]);
	j = posic(comando, j+1);
	ponto[1] = transformF(&comando[j+1]);

	i = getpos(formas,i);
	area = intervalos(formas, i);

	if(formas[i].tipo=='c'){
		dist = sqrt(pow(abs(formas[i].pos[0]-ponto[0]),2)+pow(abs(formas[i].pos[1]-ponto[1]),2));
		if(dist <= formas[i].tam[0]){
			strcpy(booleano,"SIM\n");
		}
	}else if(ponto[0] >= area[0] && ponto[0] <= area[1] && ponto[1] >= area[2] && ponto[1] <= area[3]){
		strcpy(booleano,"SIM\n");
	}


	return booleano;
}


void centro(forma *formas, int id){

	id = getpos(formas,id);

	if(formas[id].tipo == 'c'){
		formas[id].centro[0] = formas[id].pos[0];
		formas[id].centro[1] = formas[id].pos[1];
	}else{
		formas[id].centro[0] = (formas[id].tam[0]/2)+formas[id].pos[0];
		formas[id].centro[1] = (formas[id].tam[1]/2)+formas[id].pos[1];
	}

}

float calcula_dist(forma *formas, int i, int j){
	float dist;

	i = getpos(formas,i);
	j = getpos(formas,j);

	dist = sqrt(pow(abs(formas[i].centro[0]-formas[j].centro[0]),2)+pow(abs(formas[i].centro[1]-formas[j].centro[1]),2));

	return dist;
}

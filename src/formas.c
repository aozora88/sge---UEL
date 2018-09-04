#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "formas.h"
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


forma *criar(int max){
	forma *formas;
	int i;

	if(max > 0){
		formas = calloc((max+1),sizeof(forma));
	}else{
		formas = calloc(1002,sizeof(forma));
		max = 1001;
	}

	for(i=0;i<max;i++){
		formas[i].tipo = 'n';
		formas[i].sobrep = -1;
	}
	formas[i].tipo = 'f';
	formas[i].sobrep = -1;

	return formas;
}

int procura_id(forma *formas,int c){
	int i;

	for(i=0;formas[i].tipo!='f';i++){
		if(formas[i].id==c){
			return i;
		}
	}

	return -1;
}

forma *add(char *comando, forma *formas){

	int id, i, c;
	char *aux;
	float fl;


	
	aux = calloc(strlen(comando),sizeof(char));

	
	c = 0;
	for(i=2;comando[i]!=' ';i++){
		aux[c] = comando[i];
		c++;
	}

	c = atoi(aux);
	id = procura_id(formas,c);
	if(id==-1){
		id = defID(formas);
	}
	//id = c;

	formas[id].id = c;
	
	formas[id].tipo = comando[0];

	
	c = 0;

	for(i=i+1;comando[i]!=' ';i++){
		formas[id].borda[c] = comando[i];
		c++;
	}
	formas[id].borda[c] = '\0';

	
	c = 0;
	for(i=i+1;comando[i]!=' ';i++){
		formas[id].cor[c] = comando[i];
		c++;
	}
	formas[id].cor[c] = '\0';

	
	i++;
	if(formas[id].tipo=='c'){
		fl = transformF(&comando[i]);
		formas[id].tam[0] = fl;
	}else{
		fl = transformF(&comando[i]);
		formas[id].tam[0] = fl;
		i = posic(comando,i);
		fl = transformF(&comando[i+1]);
		formas[id].tam[1] = fl;
	}

	
	i = posic(comando, i+1);
	fl = transformF(&comando[i+1]);
	formas[id].pos[0] = fl;
	i = posic(comando, i+1);
	fl = transformF(&comando[i+1]);
	formas[id].pos[1] = fl;


	centro(formas, formas[id].id);

	return formas;

}



void defcores(char *comando, char **stcor, char **stborda){

	char *word;
	int i;

	word = takeword(&comando[3]);
	*stborda = calloc(sizeof(word),sizeof(char));
	strcpy(*stborda,word);
	free(word);

	i = posic(comando,3);
	word = takeword(&comando[i+1]);
	*stcor = calloc(sizeof(word),sizeof(char));
	word = tiran(word);
	strcpy(*stcor,word);
	
}

forma *removeForma(forma *formas,int i){

	i = getpos(formas,i);

	formas[i].tipo = 'n';

	return formas;

}

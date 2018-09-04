#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set_var.h"

struct reg{

	char *path_entrada;
	char *seg_entrada;
	char *ec_entrada;
	char *pm_entrada;
	char *entrada;
	char *saida;
};

float min(float x, float y){

	if(x < y){
		return x;
	}else{
		return y;
	}

}

float max(float x, float y){

	if(x > y){
		return x;
	}else{
		return y;
	}

}

int transform(char *aux){

	int i, j;
	char *prot;

	prot = calloc(sizeof(aux), sizeof(char));


	j = 0;
	for(i=0;aux[i]!=' ' && aux[i]!='\0';i++){
		prot[j] = aux[i];
		j++; 
	}

	i = atoi(prot);

	return i;

}


float transformF(char *aux){

	int j,i;
	float f;
	char *prot;

	prot = calloc(strlen(aux), sizeof(char));


	for(i=0;aux[i]!=' ' && aux[i]!='\0';i++){
		prot[i] = aux[i]; 
	}

	f = atof(prot);

	return f;

}

int posic(char *comando, int j){

	for(;comando[j]!=' ';j++);

	return j;
}

char *transformS(float n){

	char *convertido;
	int tam;

	convertido = calloc(20,sizeof(char));

	sprintf(convertido, "%f", n);
	//tam = strlen(convertido);
	//convertido = realloc(convertido, tam*sizeof(char));

	return convertido;

}


char *takeword(char *comando){

	int j;
	char *word;

	word = calloc(strlen(comando),sizeof(char));

	for(j=0;comando[j]!=' ' && comando[j]!='\n';j++){
		word[j] = comando[j];
	}

	word = tiran(word);

	return word;

}

char *tiran(char *str){
	int i;

	for(i=0;i<strlen(str);i++){
		if(str[i] == '\n'){
			str[i] = '\0';
		}
	}

	return str;
}


char *gera_comando(char *comando, int ct){
	char *cd, *word, *vet[4];
	int i,c;

	cd = calloc(strlen(comando)+14,sizeof(char));

	if(ct == 0){
		strcpy(cd, "r 1 red none ");
		c = 4;
	}else{
		strcpy(cd, "c 1 red none ");
		c = 3;
	}

	if(comando[1]=='l'){
		c++;
	}

	word = strtok(comando, " ");

	for(i=0;i<c;i++){
		word = strtok(NULL, " ");
		vet[i] = word;
	}
	vet[i-1] = tiran(vet[i-1]);
	i--;

	if(ct==0){
		strcat(cd,vet[i-1]);
		strcat(cd," ");
		strcat(cd,vet[i]);
		strcat(cd," ");
		strcat(cd,vet[i-3]);
		strcat(cd," ");
		strcat(cd,vet[i-2]);
	}else{
		strcat(cd,vet[i]);
		strcat(cd," ");
		strcat(cd,vet[i-2]);
		strcat(cd," ");
		strcat(cd,vet[i-1]);
	}

	return cd;
	
}

char *gera_comando_i(float x, float y){
	char *cd, *pos[2];

	pos[0] = transformS(x);
	pos[1] = transformS(y);
	cd = calloc(strlen(pos[0])+strlen(pos[1])+6,sizeof(char));

	strcpy(cd,"i 1 ");
	strcat(cd,pos[0]);
	strcat(cd," ");
	strcat(cd,pos[1]);

	return cd;
}

char *tira_ext(char *str){
	int tam, i;
	char *str2;

	tam = strlen(str);
	str2 = calloc(tam,sizeof(char));
	strcpy(str2,str);

	for(i=0;i<=4;i++){
		str2[tam] = '\0';
		tam--;
	}

	return str2;
}

char *tira_path(char *str){
	int i, tam,c,j;
	int flag = 0;
	char *new;

	tam = strlen(str);
	for(i=0;i<tam;i++){
		if(str[i] == '/'){
			flag = i;
		}
	}
	new = calloc(tam-flag,sizeof(char));
	c = 0;
	for(j=flag+1;j<tam;j++){
		new[c] = str[j];
		c++;
	}

	if(flag > 0){
		return new;
	}else{
		return str;
	}

}

char *gera_saida(reg *end, int c){
	char *saida, *new;

	if(c==0){
		saida = calloc(strlen(end->saida)+strlen(end->entrada)+5,sizeof(char));	
		strcpy(saida,end->saida);
		new = tira_ext(end->entrada);
		strcat(saida,new);
		strcat(saida,".txt");
	}else{

		saida = calloc(strlen(end->saida)+strlen(end->entrada)+strlen(end->seg_entrada),sizeof(char));
		strcpy(saida,end->saida);
		new = tira_ext(end->entrada);
		strcat(saida,new);
		strcat(saida,"-");
		free(new);
		new = tira_path(end->seg_entrada);
		new = tira_ext(new);
		strcat(saida,new);
		strcat(saida,".svg");
	}

	free(new);

	return saida;
}

void separaSTR(char *comando, char *vet[]){
	int i;
	char *tokens;

	tokens = strtok(comando," ");

	i = 0;
	tokens = strtok(NULL," ");
	while(tokens!=NULL){
		vet[i] = (char *) malloc(strlen(tokens)*sizeof(char));
		strcpy(vet[i],tokens);
		tokens = strtok(NULL," ");
		i++;
	}
}
			
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kd_tree.h"

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

struct ECtipo{
	char *tipo;
	char *descript;
	struct EC *lista;
	struct ECtipo *dir;
	struct ECtipo *esq;
};

//==============================================
eqp *createTree(char *id, char tipo,float x, float y, float l, float a){
	eqp *tree = calloc(1,sizeof(eqp));
	
	tree->id = calloc(strlen(id),sizeof(char));
    strcpy(tree->id,id);
    tree->tipo = tipo;
	tree->pos[0]=x;
	tree->pos[1]=y;
    tree->tam[0]=l;
    tree->tam[1]=a;
    tree->fill = NULL;
	tree->stroke = NULL;
	tree->esq=NULL;
	tree->dir=NULL;

	return tree;
}

 /*ECtipo *createComercio(char *sigla, char *descrição){
	 ECtipo *aux = malloc(sizeof(ECtipo));

	 aux->tipo = malloc(strlen(sigla)*sizeof(char));
	 strcpy(aux->tipo,sigla);
	 aux->descript = malloc(strlen(descriçao)*sizeof(char));
	 strcpy(aux->descript,descriçao);
	 aux->lista = NULL;
	 aux->dir = NULL;
	 aux->esq = NULL;

	 return aux;
}*/
//==============================================
void insereTree(void *st, eqp *aux, int i){

	eqp *tree = st;
	

	if(tree->id==NULL){
		subNo(&tree, aux->id,aux->tipo, aux->pos[0], aux->pos[1], aux->tam[0], aux->tam[1], aux->esq, aux->dir, aux->fill, aux->stroke);
		return;
	}
	

	if(aux->pos[i] < tree->pos[i]){
		if(tree->esq==NULL){
			subNo(&(tree)->esq,aux->id,aux->tipo, aux->pos[0], aux->pos[1], aux->tam[0], aux->tam[1], aux->esq, aux->dir, aux->fill, aux->stroke);
			return;
		}
        insereTree(tree->esq,aux,!i);
	}else{
		if(tree->dir==NULL){
			subNo(&(tree)->dir,aux->id,aux->tipo, aux->pos[0], aux->pos[1], aux->tam[0], aux->tam[1], aux->esq, aux->dir, aux->fill, aux->stroke);
			return;
		}
		insereTree(tree->dir,aux,!i);
	}
}

//=============================================
eqp *getPai(eqp *tree,eqp *filho){

	if(tree->esq != NULL){
		/*if(strcmp(tree->esq->id,filho->id)==0){
			return tree;
		}*/

		if(tree->esq==filho){
			return tree;
		}
	}

	if(tree->dir != NULL){
		/*if(strcmp(tree->dir->id,filho->id)==0){
			return tree;
		}*/

		if(tree->dir==filho){
			return tree;
		}
	}

	struct eqp *result = NULL;
	
	if(tree->esq != NULL){
		result = getPai(tree->esq,filho);
		return result;
	}

	if(tree->dir != NULL){
		result = getPai(tree->dir,filho);
	}

	return result;

}
//==============================================
eqp *procuraID(eqp *tree,char *ID){

	if(strcmp(tree->id,ID)==0){
		return tree;
	}

	struct eqp *result = NULL;
	
	if(tree->esq != NULL){
		result = procuraID(tree->esq,ID);
	}

	if(tree->dir != NULL){
		result = procuraID(tree->dir,ID);
	}

	return result;

}
//==============================================
int getAltura(eqp *tree, eqp *no, int alt){
	
	if(tree==no){
		return alt;
	}
	
	if(tree->esq != NULL){
		alt++;
		alt = getAltura(tree->esq,no,alt);
	}

	if(!alt && tree->dir != NULL){
		alt++;
		alt = getAltura(tree->dir,no,alt);
	}

	return alt;

}
//==============================================
int parImpar(int alt){

	if(alt%2==0){
		return 0;
	}else{
		return 1;
	}

}
//==============================================
void getMinI(eqp *tree,eqp *menor,int pos){

	if(tree->pos[pos] < menor->pos[pos]){
		menor = tree;
	}

	if(tree->dir!=NULL){
		getMinI(tree->dir,menor,pos);
	}

	if(tree->esq!=NULL){
		getMinI(tree->esq,menor,pos);
	}
}
//=============================================
void subNo(eqp **tree,char *id,char tipo, float x, float y, float l, float a, eqp *esq, eqp *dir, char *fill, char *stroke){
	
	if(*tree==NULL){
		*tree = calloc(1,sizeof(eqp));
	}
	
	(*tree)->id = calloc(strlen(id),sizeof(char));
    strcpy((*tree)->id,id);
    (*tree)->tipo = tipo;
	(*tree)->pos[0]=x;
	(*tree)->pos[1]=y;
    (*tree)->tam[0]=l;
    (*tree)->tam[1]=a;
	if(fill!=NULL){
		(*tree)->fill = calloc(strlen(fill),sizeof(char));
		strcpy((*tree)->fill,fill);
	}else{
		(*tree)->fill = NULL;
	}
	if(stroke!=NULL){
		(*tree)->stroke = calloc(strlen(stroke),sizeof(char));
		strcpy((*tree)->stroke,stroke);
	}else{
		(*tree)->stroke = NULL;
	}
	(*tree)->esq=esq;
	(*tree)->dir=dir;
	
}
//==============================================

void removeNo(eqp **tree,eqp **filho){
	eqp *pai;

	pai = getPai(*tree,*filho);

	if((*filho)->esq==NULL && (*filho)->dir==NULL){
		if(pai==NULL){
			free((*filho));
			(*filho) = NULL;
		}else if(pai->esq==(*filho)){
			free(pai->esq);
			pai->esq = NULL;
		}else{
			free(pai->dir);
			pai->dir = NULL;
		}
	}else{
		eqp *min;
		//calcula altura do nó
		int alt = getAltura(*tree,*filho,0);
		//altura par ou impar
		int pos = parImpar(alt);
		if((*filho)->dir != NULL){
			//get min
			min = (*filho)->dir;
			getMinI(*tree,min,pos);
		
		}else{
			//get min
			min = (*filho)->esq;
			getMinI(*tree,min,pos);
			//rotaciona
			free((*filho)->dir);
			(*filho)->esq = NULL;	
		}

		//substitui
		//trocaNo(filho,min);
		subNo(filho,min->id,min->tipo,min->pos[0],min->pos[1],min->tam[0],min->tam[1],(*filho)->esq,(*filho)->dir,min->fill,min->stroke);
		//remove
		removeNo(filho,&min);
	}

}

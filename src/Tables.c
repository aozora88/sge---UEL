#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set_var.h"
#include "Tables.h"

struct EC{
	char *CNPJ;
	char *tipo;
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
	struct EC *estabs;
};

struct Morador{
	char *CPF;
	char *nome;
	char *sobrenome;
	char sexo;
	char *nascimento;
	char *CEP;
	char face;
	int num;
	char *compl;
    struct Morador *next;
};


void createTipo(ECtipo *aux,char *comando){

    char *str, *desc;
    int i;

    str = takeword(comando);
    i = posic(comando,0);
    desc = &comando[i+1];

    aux->tipo = calloc(strlen(str)+1,sizeof(char));
    strcpy(aux->tipo,str);
    aux->descript = calloc(strlen(desc)+1,sizeof(char));
    strcpy(aux->descript,desc);
    aux->estabs = NULL;

}

void AddHashCPF(struct Morador hash,char *vet[]){
    Morador *aux;

   if(hash.nome!=NULL){
       //verificar colisao
       aux = hash.next;
       AddHashCPF(*aux,vet);
       return;
   }

    hash.CPF = calloc(strlen(vet[0])+1,sizeof(char));
    strcpy(hash.CPF,vet[0]);
    hash.nome =  calloc(strlen(vet[1])+1,sizeof(char));
    strcpy(hash.nome,vet[1]);
    hash.sobrenome = calloc(strlen(vet[2])+1,sizeof(char));
    strcpy(hash.sobrenome,vet[2]);
    char *str = vet[3];
    hash.sexo = str[0];
    hash.nascimento = calloc(strlen(vet[4])+1,sizeof(char));
    strcpy(hash.nascimento,vet[4]);

    hash.next = calloc(1,sizeof(EC));
    hash.next->nome = NULL;

}

void AddHashCEP(struct Morador hash,char *vet[]){
    Morador *aux;

   if(hash.nome!=NULL){
       //verificar colisao
       aux = hash.next;
       AddHashCEP(*aux,vet);
       return;
   }

    hash.CPF = calloc(strlen(vet[0])+1,sizeof(char));
    strcpy(hash.CPF,vet[0]);
    hash.CEP =  calloc(strlen(vet[1])+1,sizeof(char));
    strcpy(hash.CEP,vet[1]);
    char *str = vet[2];
    hash.face = str[0];
    hash.num = atoi(vet[3]);
    hash.compl = calloc(strlen(vet[4])+1,sizeof(char));
    strcpy(hash.compl,vet[4]);

    if(hash.next==NULL){
         hash.next = calloc(1,sizeof(EC));
        hash.next->nome = NULL;
    }
}

void hashPessoa(Morador *hash,char *comando,int tam,int choose){
    char *vet[6];
    int i, ac;

    char *cmd = calloc(strlen(comando),sizeof(char));
    strcpy(cmd,comando);
    separaSTR(cmd,vet);

    char *key = calloc(strlen(vet[choose])+1,sizeof(char));
    strcpy(key,vet[choose]);

    ac = 0;
    for(i=0;i<strlen(key);i++){
        ac += key[i];
    }

    i = ac%tam;

    if(choose==0){
       //add hash cpf 
       AddHashCPF(hash[choose],vet);
    }else{
       //add hash cep 
       AddHashCEP(hash[choose],vet);
    }
}

void hashEC(EC *hash,char *comando,int tam,int choose){
    char *vet[6];
    int i, ac;

    char *cmd = calloc(strlen(comando),sizeof(char));
    strcpy(cmd,comando);
    separaSTR(cmd,vet);

    char *cep = calloc(strlen(vet[choose])+1,sizeof(char));
    strcpy(cep,vet[choose]);

    ac = 0;
    for(i=0;i<strlen(cep);i++){
        ac += cep[i];
    }

    i = ac%tam;

    if(choose==0){
         addEC_CEP(hash[i],vet);
    }else{
         addEC_CNPJ(hash[i],vet);
    }
}

void addEC_CEP(struct EC hash,char *vet[]){
    EC *aux;

   if(hash.nome!=NULL){
       //verificar colisao
       aux = hash.dir;
       addEC_CEP(*aux,vet);
       return;
   }

    hash.CNPJ = calloc(strlen(vet[0])+1,sizeof(char));
    strcpy(hash.CNPJ,vet[0]);
    hash.tipo =  calloc(strlen(vet[1])+1,sizeof(char));
    strcpy(hash.tipo,vet[1]);
    hash.CEP = calloc(strlen(vet[2])+1,sizeof(char));
    strcpy(hash.CEP,vet[2]);
    hash.num = atoi(vet[3]);
    hash.nome = calloc(strlen(vet[4])+1,sizeof(char));
    strcpy(hash.nome,vet[4]);

    hash.dir = calloc(1,sizeof(EC));
    hash.dir->nome = NULL;

}

void addEC_CNPJ(struct EC hash,char *vet[]){
    EC *aux;

   if(hash.nome!=NULL){ 
       //verificar colisao
       aux = hash.dir;
       addEC_CNPJ(*aux,vet);
       return;
   }
    
    hash.CNPJ = calloc(strlen(vet[0])+1,sizeof(char));
    strcpy(hash.CNPJ,vet[0]);
    hash.tipo =  calloc(strlen(vet[1])+1,sizeof(char));
    strcpy(hash.tipo,vet[1]);
    hash.CEP = calloc(strlen(vet[2])+1,sizeof(char));
    strcpy(hash.CEP,vet[2]);
    hash.num = atoi(vet[3]);
    hash.nome = calloc(strlen(vet[4])+1,sizeof(char));
    strcpy(hash.nome,vet[4]);

    hash.dir = calloc(1,sizeof(EC));
    hash.dir->nome = NULL;

}

void heap(ECtipo *vet, int fim){
	int i, a;
    a = 0;
    ECtipo *aux;

	if(fim == 1){
		return;
	}

//monta arvore: if vet[i] > vet[2i+1 || 2i+2 -> troca
	for(i=0;i<(fim-1);i++){
        while(a>=0 && (2*i)+1 < fim){
            if(vet[i].tipo[a] > vet[(2*i)+1].tipo[a]){
                break;
            }
            if(vet[i].tipo[a] < vet[(2*i)+1].tipo[a]){
			    aux = calloc(1,sizeof(ECtipo));
                aux->tipo = malloc(strlen(vet[1].tipo)*sizeof(char));
                strcpy(aux->tipo,vet[i].tipo);
                aux->descript = malloc(strlen(vet[i].descript)*sizeof(char));
                strcpy(aux->descript,vet[i].descript);

			    vet[i] = vet[(2*i)+1];
			    vet[(2*i)+1] = *aux;
                a = -2;
		    }
            a = a+1;
        }
		a = 0;
        while(a>=0 && (2*i)+2 < fim){
            if(vet[i].tipo[a] > vet[(2*i)+2].tipo[a]){
                break;
            }
            if(vet[i].tipo[a] < vet[(2*i)+2].tipo[a]){
			    aux = calloc(1,sizeof(ECtipo));
                aux->tipo = malloc(strlen(vet[1].tipo)*sizeof(char));
                strcpy(aux->tipo,vet[i].tipo);
                aux->descript = malloc(strlen(vet[i].descript)*sizeof(char));
                strcpy(aux->descript,vet[i].descript);

			    vet[i] = vet[(2*i)+2];
			    vet[(2*i)+2] = *aux;
                a = -2;
		    }
            a=a+1;
        }	
        a = 0;
	}
    //vet[0] troca com ultima posiçao
	aux = calloc(1,sizeof(ECtipo));
    aux->tipo = malloc(strlen(vet[0].tipo)*sizeof(char));
    strcpy(aux->tipo,vet[0].tipo);
    aux->descript = malloc(strlen(vet[0].descript)*sizeof(char));
    strcpy(aux->descript,vet[0].descript);

	vet[0] = vet[fim-1];
	vet[fim-1] = *aux;

	heap(vet,fim-1);
}

struct Morador search_CPF(Morador *hashCPF,char *key,int size){
    int i, ac, id;

    for(i=0;key[i]!=' ';i++){
        ac += key[i];
    }

    id = ac%size;

    return hashCPF[id];

}

struct Morador search_pessoaCEP(Morador *hash,char *key,int size){

    int i, ac, id;

    for(i=0;key[i]!=' ';i++){
        ac += key[i];
    }

    id = ac%size;

    return hash[id];
}
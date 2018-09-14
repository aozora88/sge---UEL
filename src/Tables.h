#ifndef _TABLES_LIB_
#define _TABLES_LIB_

typedef struct ECtipo ECtipo;
typedef struct EC EC;
typedef struct Morador Morador;

/*
preenche a estrutura com os tipos
*/
void createTipo(ECtipo *aux,char *comando);

/*
adiciona um morador pelo cpf
*/
void AddHashCPF(struct Morador hash,char *vet[]);

/*
adiciona um morador pelo cep
*/
void AddHashCEP(struct Morador hash,char *vet[]);

/*
gera codigo hash para pessoa
*/
void hashPessoa(Morador *hash,char *comando,int tam,int choose);

/*
gera codigo hash para estabelecimento
*/
void hashEC(EC *hash,char *comando,int tam,int choose);

/*
adiciona um estabelecimento na hash de/pelo CEP
*/
void addEC_CEP(struct EC hash,char *vet[]);

/*
adiciona um estabelecimento na hash de/pelo CNPJ
*/
void addEC_CNPJ(struct EC hash,char *vet[]);

/*
ordena o vetor de tipos de estabelecimento
*/
void heap(ECtipo *vet, int fim);

/*
retorna morador com cpf procurado
*/
struct Morador search_CPF(Morador *hashCPF,char *key,int size);

/*
retorna morador com cep procurado
*/
struct Morador search_pessoaCEP(Morador *hash,char *key,int size);


#endif 
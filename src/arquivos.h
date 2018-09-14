#ifndef _ARQUIVOS_LIB_
#define _ARQUIVOS_LIB_


typedef struct reg reg;
typedef struct forma forma;
typedef struct eqp eqp;
typedef struct cor cor;
typedef struct structure structure;
typedef struct ECtipo ECtipo;
typedef struct EC EC;
typedef struct hashTables hashTables;
typedef struct Morador Morador;


/*
formata as strings de entrada e saida
*/
reg *path_treat(reg *arquivos_path, int aux);

/*
le os comandos do arquivo .pm
*/
void lerPM(hashTables *hashTables,char *path,char *name);

/*
le os comandos do arquivo .ec
*/
hashTables *lerEC(structure *arvores, char *path, char *name);

/*
lê os comandos do arquivo .qry
*/
void ler_2(reg *arquivos_path,cor *cores,structure *arvores);

/*
lê os comandos do arquivo de entrada
*/
void ler(reg *arquivos_path,int argc, const char *argv[]);

/*
gera o arquivo txt
*/
void consultas(reg *arquivos_path, char *comando, forma *formas, char *parm);

/*
formata o endereço de saida do arquivo svg produzido pelo comando 'a'
*/
char *geraSVG(reg *arquivos_path,forma *formas,char *comando);


#endif 
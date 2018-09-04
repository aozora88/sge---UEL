#ifndef _CITY_CONSULTAS_LIB_
#define _CITY_CONSULTAS_LIB_

typedef struct forma forma;
typedef struct quadra quadra;
typedef struct eqp eqp;
typedef struct reg reg;
//typedef struct structure structure;
typedef struct cor cor;


/*
retorna a cor do equipamento
*/
char *get_cor(eqp *est,cor *cores, int c);

/*
cria a forma que delimita analise do mapa
*/
void delimita(forma *formas, reg *end, char *comando,int i);

/*
verifica se dois objetos da cidade se sobrepoe
*/
int verifica_sob(forma *formas);

/*
verifica se um ponto é interno a uma forma
*/
int loop(forma *formas,float x, float y);

/*
reporta equipamentos
*/
void rangeSearch(eqp **tree,eqp **root,forma *area,cor *cores,char kind,int key, int i, char *saida);

/*
print txt equipamentos selecionados
*/
void reporta(eqp *lista,cor *cores,char *saida);

/*
gera svg do .qry
*/
void mapa(eqp *tree, cor *cores,char *saida);

#endif
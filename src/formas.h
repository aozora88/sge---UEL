#ifndef _FORMAS_LIB_
#define _FORMAS_LIB_

typedef struct forma forma;


/*
cria a estrutura que guarda as formas geométricas
*/
forma *criar(int max);

/*
verifica se existem duas formas com mesmo id
*/
int procura_id(forma *formas,int c);

/*
adiciona uma forma geometrica (circulo ou retangulo)
*/
forma *add(char *comando, forma *formas);

/*
guarda as cores das quadras e equipamentos urbanos
*/
void defcores(char *comando, char **stcor, char **stborda);

/*
remove forma de id igual a i
*/
forma *removeForma(forma *formas,int i);

#endif 
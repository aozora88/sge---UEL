#ifndef _OP_LIB_
#define _OP_LIB_

typedef struct forma forma;

/*
retorna a posiçao da forma com id informado
*/
int getpos(forma *formas,int id);

/*
retorna id do espaço vazio na estrutura formas
*/
int defID(forma *formas);

/*
calcula o tamanho do retangulo pontilhado em volta das ofrmas que se sobrepõe
*/
float *tam_sobrep(float *f1, float *f2);

/*
calcula os intervalos de x e y que determinada forma ocupa
*/
float *intervalos(forma *formas, int i);

/*
verifica se duas formas se sobrepõe
*/
forma *calc_sob(char *comando, forma *formas);

/*
verifica se determinado ponto é interno a determinada figura
*/
char *interno(char *comando, forma *formas);

/*
calcula o ponto central de determinada figura
*/
void centro(forma *formas, int id);

/*
calcula distancia de duas figuras
*/
float calcula_dist(forma *formas, int i, int j);

#endif
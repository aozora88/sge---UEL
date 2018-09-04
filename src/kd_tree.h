#ifndef _KD_TREE_LIB_
#define _KD_TREE_LIB_

typedef struct eqp eqp;

/*
cria um nó
*/
eqp *createTree(char *id,char tipo, float x, float y, float l, float a);

/*
insere na arvore
*/
void insereTree(eqp **tree, eqp *aux, int i);

/*
retorna o nó pai do nó filho
*/
eqp *getPai(eqp *tree,eqp *filho);

/*
procura equipamento por id na arvore
*/
eqp *procuraID(eqp *tree,char *ID);

/*
retorna a altura do nó
*/
int getAltura(eqp *tree, eqp *no, int alt);

/*
retorna 0 se a altura for impar e 1 se for par
*/
int parImpar(int alt);

/*
retorna o minimo da subarvore passada
*/
void getMinI(eqp *tree,eqp *menor,int pos);

/*
substitui o conteudo de um nó
*/
void subNo(eqp **tree,char *id,char tipo, float x, float y, float l, float a, eqp *esq, eqp *dir, char *fill, char *stroke);

/*
remove um elemento da arvore
*/
void removeNo(eqp **tree, eqp **filho);

#endif 
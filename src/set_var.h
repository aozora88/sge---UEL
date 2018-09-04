#ifndef _SET_VAR_LIB_
#define _SET_VAR_LIB_

typedef struct reg reg;

/*
retorna o menor valor passado
*/
float min(float x, float y);

/*
retorna o maior valor passado
*/
float max(float x, float y);

/*
transforma string em int
*/
int transform(char *aux);

/*
transforma string em float
*/
float transformF(char *aux);

/*
posiciona no proximo valor do comando
*/
int posic(char *comando, int j);

/*
transforma float em string
*/
char *transformS(float n);

/*
retorna uma palavra do comando
*/
char *takeword(char *comando);

/*
tira \n do fim da string
*/
char *tiran(char *str);

/*
gera uma string de comando
*/
char *gera_comando(char *comando, int ct);

/*
gera comando para verificar ponto interno
*/
char *gera_comando_i(float x, float y);

/*
tira a estensao de um arquivo
*/
char *tira_ext(char *str);

/*
retorna o nome do arquivo str
*/
char *tira_path(char *str);

/*
gera path de saida para comandos do .qry
*/
char *gera_saida(reg *end, int c);

/*
retorna as palavras da string separadas
*/
void separaSTR(char *comando,char *vet[]);

#endif
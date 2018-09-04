#ifndef _ARQS_SVG_LIB_
#define _ARQS_SVG_LIB_

typedef struct reg reg;
typedef struct forma forma;
typedef struct eqp eqp;
typedef struct cor cor;

/*
desenha o que foi definido no comando 'a' 
*/
void desenha_a(forma *formas, char *saida, char *comando);

/*
desenha a linha pontilhada envolta das formas que se sobrepõe
*/
void desenha_sobrep(forma *formas, char *saida);

/*
printa a forma que delimita a consulta dos comandos do .qry
*/
void print_delimita(char *out,forma *formas);

/*
desenha as formas no arquivo svg
*/
void fecha_svg(char *saida,forma *formas);

/*
formata o endereço de saida do arquivo svg produzido ao fim do programa
*/
char *defsaida(reg *arquivos_path);

/*
muda a cor de uma quadra ou equipamento
*/
void muda_cor(eqp **urbanEQP,char *comando);

/*
desenha um equipamento no svg
*/
void printTreeSVG(eqp *tree, cor *cores, FILE *saida);

#endif
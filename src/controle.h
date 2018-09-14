#ifndef _CONTROLE_LIB_
#define _CONTROLE_LIB_

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
salva o endereço dos arquivos de saida e entrada definidos nos argumentos do programa
*/
reg *argumentos(reg *arquivos_path, int argc, const char **argv);

/*
define cores das quadras e equipamentos urbanos
*/
void color(cor *cores, char *comando);

/*
identifica comando do arquivo .geo
*/
void control(reg *arquivos_path, char *comando, forma *formas, cor *cores, structure *arvores);

/*
identifica comando do arquivo .qry
*/
void control2(reg *arquivos_path,char *comando,cor *cores,forma *formas,structure *arvores);

/*
identifica comandos do arquivo .ec
*/
void controlEC(structure *arvores, hashTables *hashTables,char *comando);

/*
identifica comandos do arquivo .pm
*/
void controlPM(hashTables *hash, char *comando);

#endif 
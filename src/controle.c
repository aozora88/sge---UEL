#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivos.h"
#include "formas.h"
#include "set_var.h"
#include "op.h"
#include "arqs_svg.h"
#include "controle.h"
#include "city_consultas.h"
#include "kd_tree.h"
#include "Tables.h"

struct reg{
	
	char *path_entrada;
	char *seg_entrada;
	char *ec_entrada;
	char *pm_entrada;
	char *entrada;
	char *saida;
};

struct forma{

	int id;
	char tipo;
	float tam[2];
	char cor[20];
	char borda[20];
	float pos[2];
	float centro[2];
	int sobrep;
	
};

struct eqp{
    char *id;
	char tipo;
	float pos[2];
	float tam[2];
    char *fill;
	char *stroke;
    struct eqp *esq;
	struct eqp *dir;
	//struct eqp *pai;
};

struct cor{

	char *fillQ;
	char *bordaQ;
	char *fillH;
	char *bordaH;
	char *fillT;
	char *bordaT;
	char *fillS;
	char *bordaS;

};

struct EC{
	char *CNPJ;
	char tipo;
	char *CEP;
	char face;
	int num;
	char *nome;
	struct EC *dir;
	struct EC *esq;
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

struct ECtipo{
	char *tipo;
	char *descript;
	struct EC *estabs;
};

struct structure{

	struct eqp *tree;
	struct eqp *quadras;
	struct eqp *torres;
};

struct hashTables{

	struct ECtipo *hashTipos;
	struct EC *hashCNPJ;
	struct EC *ec_CEP;
	struct Morador *hashCPF;
	struct Morador *CEP;
	int size[4];
};



reg *argumentos(reg *arquivos_path, int argc, const char **argv){

	int i;

	arquivos_path->path_entrada = malloc(2*sizeof(char));
	arquivos_path->saida = malloc(2*sizeof(char));
	arquivos_path->seg_entrada = NULL;
	arquivos_path->ec_entrada = NULL;
	arquivos_path->pm_entrada = NULL;

	strcpy(arquivos_path->path_entrada , "\0");
	strcpy(arquivos_path->saida , "\0");

	for(i=0;i<argc;i++){
		if(strcmp(argv[i],"-e")==0){
			arquivos_path->path_entrada = malloc(strlen(argv[i+1])*sizeof(char));
			strcpy(arquivos_path->path_entrada,argv[i+1]);
			arquivos_path = path_treat(arquivos_path,1);
			
		}else if(strcmp(argv[i],"-f")==0){
			arquivos_path->entrada = malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy(arquivos_path->entrada,argv[i+1]);
			
		}else if(strcmp(argv[i],"-o")==0){
			arquivos_path->saida = malloc(sizeof(argv[i+1])*sizeof(char));
			strcpy(arquivos_path->saida,argv[i+1]);
			arquivos_path = path_treat(arquivos_path,2);
			
		}else if(strcmp(argv[i],"-q")==0){
			arquivos_path->seg_entrada = calloc(strlen(argv[i+1])+1,sizeof(char));
			strcpy(arquivos_path->seg_entrada,argv[i+1]);
		
		}else if(strcmp(argv[i],"-ec")==0){
			arquivos_path->ec_entrada = calloc(strlen(argv[i+1])+1,sizeof(char));
			strcpy(arquivos_path->ec_entrada,argv[i+1]);

		}else if(strcmp(argv[i],"-pm")==0){
			arquivos_path->pm_entrada = calloc(strlen(argv[i+1])+1,sizeof(char));
			strcpy(arquivos_path->pm_entrada,argv[i+1]);
		}
	}

	
	return arquivos_path;
}


void control(reg *arquivos_path, char *comando, forma *formas, cor *cores, structure *arvores){

	int max, i, j, tf;
	char *booleano, *saida, *word, *tokens, *vet[5];
	float dist;
	eqp *new;

	//printf("%s\n",comando);

	switch(comando[0]){
		case 'n':
			max = transform(&comando[3]);
			formas = criar(max);
			break;
		case 'c':
			switch(comando[1]){
				case ' ':
					formas = add(comando,formas);
					break;
				case 'q':
					defcores(comando,&cores->fillQ,&cores->bordaQ);
					break;
				case 'h':
					defcores(comando,&cores->fillH,&cores->bordaH);
					break;
				case 't':
					defcores(comando,&cores->fillT,&cores->bordaT);
					break;
				case 's':
					defcores(comando,&cores->fillS,&cores->bordaS);
       				break;
			}
			break;
		case 'r':
			formas = add(comando,formas);
			break;
		case 'o':
			formas = calc_sob(comando,formas);
			consultas(arquivos_path, comando, formas, "-1");
			
			break;
		case 'i':
			booleano = interno(comando,formas);
			consultas(arquivos_path, comando, formas, booleano);
			break;
		case 'd':
			i = transform(&comando[2]);
			tf = posic(comando, 2);
			j = transform(&comando[tf+1]);

			dist = calcula_dist(formas,i,j);
			booleano = transformS(dist);
			consultas(arquivos_path, comando, formas, booleano);
			break;
		case 'q':
			//le string
			separaSTR(comando,vet);
			vet[4] = tiran(vet[4]);

			//insere uma quadra na arvore e na arvore de quadras
			new = createTree(vet[0], 'q', atof(vet[1]), atof(vet[2]), atof(vet[3]), atof(vet[4]));

			insereTree(arvores->tree, new, 0);
			insereTree(arvores->quadras, new, 0);
			break;
		case 'h':
			//insere um hifrante na arvore e na arvore de equipamentos
			separaSTR(comando,vet);
			vet[2] = tiran(vet[2]);

			new = createTree(vet[0], 'h', atof(vet[1]), atof(vet[2]), 5, 5);
			insereTree(arvores->tree, new, 0);
			
			break;
		case 's':
			//insere um semaforo na arvore e na arvore de equipamentos
			separaSTR(comando,vet);
			vet[2] = tiran(vet[2]);

			new = createTree(vet[0], 's', atof(vet[1]), atof(vet[2]), 5, 5);
			insereTree(arvores->tree, new, 0);

			break;
		case 't':
			//insere uma torre na arvore e na arvore de torres
			separaSTR(comando,vet);
			vet[2] = tiran(vet[2]);

			new = createTree(vet[0], 't', atof(vet[1]), atof(vet[2]), 5, 5);
			insereTree(arvores->tree, new, 0);
			insereTree(arvores->torres, new, 0);
			break;
		case 'a':
			saida = geraSVG(arquivos_path,formas,comando);
			fecha_svg(saida,formas);
			desenha_a(formas,saida,comando);
			break;
		case '#':
			saida = defsaida(arquivos_path);
			if(formas[0].tipo!='n'){
				fecha_svg(saida,formas);
				desenha_sobrep(formas,saida);
			}
			mapa(arvores->tree,cores,saida);
			break;
		default:
			printf("comando %s invalido\n", comando);
			break;

	}

}


void control2(reg *arquivos_path,char *comando,cor *cores, forma *formas, structure *arvores){
	eqp *lista = NULL;
	char *saida, *aux, *auxi[2];
	int i;

	if(comando[0]=='q' && comando[1]=='?'){
		delimita(formas,arquivos_path,comando,0);
		saida = gera_saida(arquivos_path,0);
		rangeSearch(&(arvores)->tree, &(arvores)->tree, formas, cores, 'n', 0, 0, saida);
	
	}else if(comando[0]=='Q' && comando[1]=='?'){
		delimita(formas,arquivos_path,comando,1);
		saida = gera_saida(arquivos_path,0);
		rangeSearch(&(arvores)->tree, &(arvores)->tree, formas, cores, 'n', 0, 0, saida);

	}else if(comando[1]=='q'){
		if(comando[0]=='d'){
			delimita(formas,arquivos_path,comando,0);
		}else{
			delimita(formas,arquivos_path,comando,1);
		}
		saida = gera_saida(arquivos_path,0);
		rangeSearch(&(arvores)->tree, &(arvores)->tree, formas, cores, 'q', 0, 1, saida);

	}else if(comando[1]=='c'){
		muda_cor(&(arvores)->tree,comando);
	}else if(comando[2]=='d'){
		saida = gera_saida(arquivos_path,0);
		aux = takeword(&comando[5]);
		lista = procuraID(arvores->tree,aux);
		if(lista==NULL){
			printf("equipamento buscado nao existe\n");
		}else{
			reporta(lista,cores,saida);
		}
	}else if(comando[1]=='l'){
		saida = gera_saida(arquivos_path,1);
		if(comando[0]=='d'){
			delimita(formas,arquivos_path,comando,0);
		}else{
			delimita(formas,arquivos_path,comando,1);
		}
		saida = gera_saida(arquivos_path,0);
		for(i=4;comando[i]!=' ';i++){
			if(comando[i]=='q'){
				rangeSearch(&(arvores)->tree, &(arvores)->tree, formas, cores, 'q', 0, 1, saida);
			}else if(comando[i]=='t'){
				rangeSearch(&(arvores)->tree, &(arvores)->tree, formas, cores, 't', 0, 1, saida);
			}else if(comando[i]=='s'){
				rangeSearch(&(arvores)->tree, &(arvores)->tree, formas, cores, 's', 0, 1, saida);
			}else if(comando[i]=='h'){
				rangeSearch(&(arvores)->tree, &(arvores)->tree, formas, cores, 'h', 0, 1, saida);
			}
		}

	}else{
		printf("comando %s invalido\n", comando);
	}
	/*else if(comando[2]=='b'){
		int tam = getTamT(torres);
		radio *vetL = moveLista(torres,tam);
		vetL = heap(vetL,tam,0);
		radio *vetY = heap(vetL,tam,1);
		sm *menorED = closestPoints(vetL,vetY,tam);

		radio *r1 = procuraTorre(torres,menorED->id1);
		radio *r2 = procuraTorre(torres,menorED->id2);

		char *command = marca_torre(r1);
		delimita(formas,arquivos_path,command,1);
		command = marca_torre(r2);
		delimita(formas,arquivos_path,command,1);
	}*/
}

void controlEC(structure *arvores, hashTables *hashTables, char *comando){

	struct ECtipo *aux;
	static int tam, i, cont;
	int j;

	switch(comando[0]){
		case 't':
			//insere na tabela
			if(hashTables->hashTipos==NULL){
				hashTables->size[0] = 100;
				tam = 100;
				hashTables->hashTipos = malloc(100*sizeof(ECtipo));
				i = 0;
				createTipo(&hashTables->hashTipos[i],&comando[2]);
				i++;

			}else if(i==tam){
				hashTables->size[0] += 100;
				tam = tam+100;
				hashTables->hashTipos = (ECtipo *) realloc(hashTables->hashTipos, tam*(sizeof(ECtipo)));
				createTipo(&hashTables->hashTipos[i],&comando[2]);
				i++;

			}else{
				createTipo(&hashTables->hashTipos[i],&comando[2]);
				i++;
			}

			break;
		case 'e':
			//hash cep
			if(hashTables->ec_CEP == NULL){
				//ordena vetor tipos//
				heap(hashTables->hashTipos,i);

				hashTables->size[1] = 100;
				tam = 100;
				hashTables->ec_CEP = calloc(tam,sizeof(EC));
				for(j=0;j<tam;j++){
					hashTables->ec_CEP[j].nome = NULL;
					hashTables->ec_CEP[j].dir = NULL;
				}
				cont = 0;
			}
			hashEC(hashTables->ec_CEP,comando,tam,2);

			//hash cnpj
			if(hashTables->hashCNPJ == NULL){
				hashTables->hashCNPJ = calloc(tam,sizeof(EC));
				for(j=0;j<tam;j++){
					hashTables->hashCNPJ[j].nome = NULL;
					hashTables->hashCNPJ[j].dir = NULL;
				}
			}
			hashEC(hashTables->hashCNPJ,comando,tam,0);

			cont++;
			if(cont>=(tam/100)*8){
				tam = tam+100;
				hashTables->size[1] += 100;
				hashTables->ec_CEP = (struct EC *) realloc(hashTables->ec_CEP, tam*(sizeof(EC)));
				hashTables->hashCNPJ = (struct EC *) realloc(hashTables->hashCNPJ, tam*(sizeof(EC)));
			}
			break;
		default:
			printf("comando %s invalido\n", comando);
			break;
	}
}

void controlPM(hashTables *hashTables, char *comando){
	static int cont;
	static int tam;
	int j;

	switch(comando[0]){
		case 'p':
			//hash CPF
			if(hashTables->hashCPF == NULL){
				tam = 100;
				hashTables->size[2] = 100;
				hashTables->hashCPF = calloc(tam,sizeof(Morador));
				for(j=0;j<tam;j++){
					hashTables->hashCPF[j].nome = NULL;
					hashTables->hashCPF[j].next = NULL;
				}
				cont = 0;
			}
			hashPessoa(hashTables->hashCPF,comando,tam,0);

			cont++;
			if(cont>=(tam/100)*8){
				tam = tam+100;
				hashTables->size[2] += 100;
				hashTables->hashCPF = (struct Morador *) realloc(hashTables->hashCPF, tam*(sizeof(Morador)));
			}
			break;
		case 'm':
			//hash cep
			if(hashTables->CEP == NULL){
				tam = 100;
				hashTables->size[3] = 100;
				hashTables->CEP = calloc(tam,sizeof(Morador));
				for(j=0;j<tam;j++){
					hashTables->CEP[j].nome = NULL;
					hashTables->CEP[j].next = NULL;
				}
				cont = 0;
			}
			hashPessoa(hashTables->CEP,comando,tam,1);

			char *vet[6];
			separaSTR(comando,vet);
			//procura e retorna por cpf
			struct Morador found = search_CPF(hashTables->hashCPF,&comando[2],hashTables->size[2]);
       		//preenche hash cpf
			AddHashCEP(*(hashTables)->hashCPF,vet);
			//preenche hash cep
			struct Morador found2 = search_pessoaCEP(hashTables->CEP,&comando[4],hashTables->size[3]);
			//sub a mao
			found2.CPF = calloc(strlen(found.CPF)+1,sizeof(char));
    		strcpy(found2.CPF,found.CPF);
    		found2.nome =  calloc(strlen(found.nome)+1,sizeof(char));
   	 		strcpy(found2.nome,found.nome);
    		found2.sobrenome = calloc(strlen(found.sobrenome)+1,sizeof(char));
    		strcpy(found2.sobrenome,found.sobrenome);
    		found2.sexo = found.sexo;
    		found2.nascimento = calloc(strlen(found.nascimento)+1,sizeof(char));
    		strcpy(found2.nascimento,found.nascimento);

			cont++;
			if(cont>=(tam/100)*8){
				tam = tam+100;
				hashTables->size[3] += 100;
				hashTables->CEP = (struct Morador *) realloc(hashTables->CEP, tam*(sizeof(Morador)));
			}
			break;
		default:
			printf("comando %s invalido\n", comando);
			break;
	}

}

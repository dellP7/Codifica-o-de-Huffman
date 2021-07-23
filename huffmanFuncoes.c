#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "huffmanModulos.h" 

//dado char que ira guardar o byte
typedef unsigned char byte;

typedef struct arvore{
	byte ch;
    int frequencia;
	struct arvore *esquerda;
	struct arvore *direita;
} ARVORE;

//proximo noh da lista
typedef struct nodoList{
    ARVORE *no;
    struct nodoList *prox;
} NODO;

//definicao da lista
typedef struct list{
    NODO *cabeca;
    int valor;
} lista;

//aloca memoria para uma nova variavel tipo NODO
NODO* novoNodoList(ARVORE* noTree){
	NODO* novo;
	if((novo = malloc(sizeof(*novo))) == NULL) return NULL;
	novo->no = noTree;
	novo->prox = NULL;
	return novo;
}

//aloca memoria para uma nova variavel no tipo ARVORE
ARVORE* novoNodoArvore(byte c, int f, ARVORE* e, ARVORE* d){
	ARVORE* novo;
	if((novo = malloc(sizeof(*novo))) == NULL) return NULL;
	novo->ch = c;
	novo->frequencia = f;
	novo->esquerda = e;
	novo->direita = d;
	return novo;
}

//inserir na cabeca se a lista de frequencia estiver vazia ou nao
void insereCabList(NODO* n, lista* newElemento){	
	if (!newElemento->cabeca){
		newElemento->cabeca=n;
	}
	else if (n->no->frequencia < newElemento->cabeca->no->frequencia){
		n->prox = newElemento->cabeca;
		newElemento->cabeca =n;
	} else{
		NODO *temp = newElemento->cabeca->prox;
		NODO *aux = newElemento->cabeca;
		while (temp && temp->no->frequencia <= n->no->frequencia){
			aux = temp;
			temp = aux->prox;
		}
		aux->prox = n;
		n->prox= temp;
	}
	newElemento->valor++;
}

//tirar o menor no da lista de frequencia 
ARVORE* desenfileiraMenor(lista* elemento){
	NODO* aux = elemento->cabeca;
	ARVORE* aux2 = aux->no;
	elemento->cabeca = aux->prox;
	free(aux);
	aux = NULL;
	elemento->valor--;
	return aux2;
}

//contar a frequencia de ocorrencia de caractere ou símbolo
void countFreq(FILE *entrada, unsigned int* listaTamanho){
	byte aux;
	while (fread(&aux, 1, 1, entrada) >= 1){
		listaTamanho[(byte)aux]++;
	}
	rewind(entrada);
}

//buscar um determinado byte
bool buscaByte(ARVORE* n, byte c, char* busca, int tam){
	if (!(n->esquerda || n->direita) && n->ch==c){
		busca[tam] = '\0';
		return true;
	} else {
		bool alvo = false;
		if (n->esquerda){
			busca[tam] = '\0';
			alvo = buscaByte(n->esquerda, c, busca, tam+1);
		}
		if (!alvo && n->direita){
			busca[tam] = '1';
			alvo = buscaByte(n->direita, c, busca, tam+1);
		}
		if (!alvo){
			busca[tam] = '\0';
		}
		return alvo;
	}
}

//criar a arvore de huffman
ARVORE* arvoreHuffman(unsigned* listaB){
	lista nLista = {NULL, 0};
	for (int i = 0; i < 256; i++){
		if (listaB [i]){
			insereCabList(novoNodoList(novoNodoArvore(i, listaB[i], NULL, NULL)), &nLista);
		}
	}
	while (nLista.valor > 1){
		ARVORE *noEsq = desenfileiraMenor(&nLista);
		ARVORE *noDir = desenfileiraMenor(&nLista);
		ARVORE *soma = novoNodoArvore('/', noEsq->frequencia + noDir->frequencia, noEsq, noDir);
		insereCabList(novoNodoList(soma), &nLista);
	}
	return desenfileiraMenor(&nLista);
}

//liberar a arvore de huffman
void freeArvoreHuffman(ARVORE* n){
	if (!n) return;
	else{
		ARVORE *esq = n->esquerda;
		ARVORE *dir = n->direita;
		free(n);
		freeArvoreHuffman(esq);
		freeArvoreHuffman(dir);
	}
}

//verificar um nodo e retornar um bit
int returnBit(FILE* ent, int p, byte* n){
	(p % 8 == 0) ? fread(n, 1, 1, ent) : NULL == NULL;
	return !!((*n) & (1 << (p % 8)));
}

//comprimir um arquivo
void comprimeArquivo(const char* arqEntrada, const char* arqSaida){
	clock_t inicio, fim;
	double time;
	inicio = clock();
	unsigned listaB[256] = {0};
	
	FILE* in = fopen(arqEntrada, "rb");
	(!in) ? printf ("ERRO! N ENCONTRADO\n") : NULL == NULL;

	FILE* out = fopen(arqSaida, "wb");
	(!out) ? printf ("ERRO! N ENCONTRADOe\n") : NULL == NULL;

	countFreq (in, listaB);

	ARVORE* r = arvoreHuffman(listaB);

	fwrite(listaB, 256, sizeof(listaB[0]), out);

	fseek(out, sizeof(unsigned int), SEEK_CUR);

	byte c, aux = 0;	
	unsigned tam = 0;

	while (fread(&c, 1, 1, in) >= 1){
		char buffer [1024] = {0};
		buscaByte(r, c, buffer, 0);
		for (char *i = buffer; *i ; i++){
			if (*i == '1'){
				aux = aux | (1 << (tam % 8));
			}
			tam++;
			if (tam % 8 == 0){
				fwrite(&aux, 1, 1, out);
				aux = 0;
			}
		}
	}
	fwrite (&aux, 1, 1, out);
	fseek (out, 256 *sizeof(unsigned int), SEEK_SET);
	fwrite (&tam, 1, sizeof(unsigned), out);

	fim = clock();
	time = (double)(fim - inicio) / CLOCKS_PER_SEC;

	fseek(in, 0L, SEEK_END);
	double tamInicio = ftell(in);

	fseek(out, 0L, SEEK_END);
	double tamFim = ftell(out);

	freeArvoreHuffman(r);

	printf("ARQUIVO DE ENTRADA: %s (%g bytes)\nARQUIVO DE SAIDA: %s (%g bytes)\nTEMPO DE PROCESSAMENTO: %gs\n", arqEntrada, tamInicio / 1000, arqSaida, tamFim / 1000, time);
    printf("COMPRESSAO: %d%%\n", (int)((100 * tamFim) / tamInicio));

	fclose(in);
	fclose(out);
}

//descomprimir um arquivo 
void descomprimeArquivo(const char* arqEntrada, const char* arqSaida){
	clock_t inicio, fim;
	double time;
	inicio = clock();
	unsigned listaB[256] = {0};

	FILE* in = fopen(arqEntrada, "rb");
	(!in) ? printf ("ERRO! N ENCONTRADO\n") : NULL == NULL;

	FILE* out = fopen(arqSaida, "wb");
	(!out) ? printf ("ERRO! N ENCONTRADO\n") : NULL == NULL;

	fread(listaB, 256, sizeof(listaB[0]), in);
	
	ARVORE* r = arvoreHuffman(listaB);

	byte aux = 0;	
	unsigned tam = 0, position = 0;
	fread(&tam, 1, sizeof(tam), in);

	while (position < tam){
		ARVORE* noAtual = r;
		while (noAtual->esquerda || noAtual->direita){
			noAtual = returnBit(in, position++, &aux) ? noAtual->direita : noAtual->esquerda; 
		}
		fwrite(&(noAtual->ch), 1, 1, fim);
	}
	freeArvoreHuffman(r);
	fim = clock();
	time = (double)(fim - inicio)/ CLOCKS_PER_SEC;

	fseek(in, 0L, SEEK_END);
	double tamEntrada = ftell(in);

	fseek(out, 0L, SEEK_END);
	double tamSaida = ftell(out);

	printf("ARQUIVO DE ENTRADA: %s (%g bytes)\nARQUIVO DE SAIDA: %s (%g bytes)\nTEMPO DE PROCESSAMENTO: %gs\n", arqEntrada, tamEntrada / 1000, arqSaida, tamSaida / 1000, time);
    printf("COMPRESSA: %d%%\n", (int)((100 * tamSaida) / tamEntrada));

	fclose(in);
	fclose(out);
}	

void comoUsar() {
    printf("TECLE '-c' PARA COMPRIMIR E '-d' PARA DESCOMPRIMIR.");
}



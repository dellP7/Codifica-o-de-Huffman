//arquivo .h

//dado char que ira guardar o byte
typedef unsigned char byte;

typedef struct arvore ARVORE;

//proximo no da lista
typedef struct nodoList NODO;

//definicao da lista
typedef struct list lista;

//aloca memoria para uma nova variavel tipo NODO
NODO* novoNodolist(ARVORE* noTree);

//aloca memoria para uma nova variavel no tipo ARVORE
ARVORE* novoNodoArvore(byte c, int f, ARVORE* e, ARVORE* d);

//inserir na cabeca se a lista de frequencia estiver vazia ou nao
void insereCabList(NODO* n, lista* newElemento);

//tirar o menor no da lista de frequencia 
ARVORE* desenfileiraMenor(lista* elemento);

//contar a frequencia de ocorrencia de caractere ou símbolo
void countFreq(FILE *entrada, unsigned int* listaTamanho);

//buscar um determinado byte
bool buscaByte(ARVORE* n, byte c, char* busca, int tam);

//criar a arvore de huffman
ARVORE* arvoreHuffman(unsigned* listaB);

//liberar a arvore de huffman
void freeArvoreHuffman(ARVORE* n);

//verificar um nodo e retorna um bit
int returnBit(FILE* ent, int p, byte* n);

//comprimir um arquivo
void comprimeArquivo(const char* arqEntrada, const char* arqSaida);

//descomprimir um arquivo 
void descomprimeArquivo(const char* arqEntrada, const char* arqSaida);	

//como usar
void comoUsar();

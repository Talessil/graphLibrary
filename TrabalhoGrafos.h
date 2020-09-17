#ifndef TRABALHOGRAFOS_H_INCLUDED
#define TRABALHOGRAFOS_H_INCLUDED
#include <stdbool.h>

struct Aresta{
    int sucessor;
    int peso;
    struct Aresta *prox;
};
typedef struct Aresta aresta;

 struct No{
     int id;
     int grau;w
     aresta *adj;
     struct No *prox;
 };
 typedef struct No no;

struct Info{
    int numNo;
    int numAresta;
};
typedef struct Info info;

/// 1ª parte do trabalho
void menu(no *grafo,info *inf);
void inicializa(no *grafo);
int vazia(no *grafo);
void freqRelativa(no *grafo, info *inf);
void leArquivo(no *grafo, info *inf, int flag);
no *buscaNo(no *grafo, int num);
void insereNo(no *grafo, int valor, info *inf, int flag);
void removeNo(no *grafo, int valor, info *inf);
aresta *buscaAresta(no *tvertice, int i);
void insereAresta(no *grafo, info *inf, int i, int j, int peso, int flag);
void excluiAresta(no *grafo, info *inf, int i, int j, int flag);
int retornaGrauNo(no *grafo, int i);
int kRegular(no *grafo);
int completo(no *grafo, info *inf);
int adjacente(no *grafo, int i, int j);
void imprimeGrafo(no *grafo);
void imprimeAresta(no *grafo);
int proxComponente(no *grafo,info *inf, int *visitados);
void buscaProfundidade(no *grafo, info *inf, int *visitados, int pos, int val);
int grafoConexo(no *grafo, info *inf, int *visitados);
int buscaPosicao(no *grafo, int val);
int noArticulacao(no *grafo, int id, info *inf);
int arestaPonte(no *grafo, int id1, int id2, info *inf);
void distribuicaoEmpirica(no *grafo, info *inf, int *vet);
void quickSort(int *lista, int iniVet,int fimVet,int mat[][2]); /// FUNÇÃO AUXILIAR DE ORDENAÇÃO, para quem precisar
void inicializaVet(int *vet, int tam);

/// 2ª parte do trabalho

no *buscaNo2(no *grafo, int pos);

void cliqueMax(no *grafo, info *inf, int *maxClique); //ok
void imprimeClique(int *vet, int tam);

void fechoDireto(no *grafo, no *vertice, info *inf); //ok

void grafoTransposto(no *grafo, no* grafoTrans, info *infTrans);
void fechoIndireto(no *grafo, no *vertice, info *inf); //ok

int minKey(int key[], bool mstSet[], int c);
void prim(no *grafo, info *inf);  //verificar

void compForteConexa(no *grafo, info *inf);
int proxComponente2(no *grafo, info *inf, int *visitados, int *novo);
void buscaProfundidade2(no *grafo, info *inf, int *visitados, int pos, int val, int *ordem, int *vetp);
void buscaProfundidade3(no *grafo, info *inf, int *visitados, int pos, int val);

void kruskal(no *grafo, info *inf); // arrumar
void menorAresta(no *grafo, info *inf, int *vet);


//vetor de posição começa com 0, ou seja, associa posição 0 do vetor ao 1º elemento da lista vertical

/// 3ª parte

void cliqueGulosa(no *grafo, info *inf);

void cliqueGulosaRandom(no *grafo, info *inf, int per, int qnt);

void cliqueReativa(no *grafo, info *inf, int qnt);

void quickSort2(int *lista, int *lista2, int iniVet,int fimVet,int mat[][2]);
void inicializaVetReal(float *vet, int tam);



#endif // TRABALHOGRAFOS_H_INCLUDED

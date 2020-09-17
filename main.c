#include <stdio.h>
#include <stdlib.h>
#include "TrabalhoGrafos.h"

int main()
{
    //aloca espaço de memória para ponteiro do tipo no
    no *grafo = (no*) malloc(sizeof(no));
    if(!grafo)
    {
        printf("\n sem espaco");
        exit(1);
    }
    //aloca espaço de memória para ponteiro do tipo info
    info *inf = (info*) malloc(sizeof(info));
    inf->numAresta=0;
    inf->numNo=0;


    inicializa(grafo);
    menu(grafo,inf);
    free(grafo);
    return 0;
}

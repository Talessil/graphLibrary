#include "TrabalhoGrafos.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define INFINITO 9999

/*
/ menu(grafo g, info do grafo)
/ fun��o que cuida da chamada de todas as outras fun��es da biblioteca
*/
void menu(no *grafo, info *inf)
{
    leArquivo(grafo,inf,0);
    printf("\nArquivo lido com sucesso");
    int escolha = 0, vezes=0, per=0, qnt=0;
    int vet[inf->numNo];
    while(escolha!=4)
    {
        printf("\n 1-Calcular clique a partir da heuristica gulosa");
        printf("\n 2-Calcular clique a partir da heuristica gulosa randomizada");
        printf("\n 3-Calcular clique a partir da heuristica reativa");
        printf("\n 4-sair");
        printf("\n");
        scanf("%d", &escolha);
        if(escolha==1)
        {
            cliqueGulosa(grafo,inf);
            printf("\n");
        }
        if(escolha==2)
        {
            printf("\nDigite o percentual em inteiro. ex: 10, 15, 20 porcento");
            scanf("%d", &per);
            printf("\nDigite o numero de vezes a rodar o calculo com o alpha escolhido");
            scanf("%d", &vezes);
            cliqueGulosaRandom(grafo,inf,per,vezes);
            printf("\n");
        }
        if(escolha==3)
        {
            printf("\nDigite o numero de vezes a ser atualizada a lista de alphas");
            scanf("%d", &qnt);
            printf("\nAlphas ja determinados como 10,20,30,40 e 50 porcento respectivamente");
            printf("\ncalculando...");
            cliqueReativa(grafo,inf,qnt);
            printf("\n");
        }
    }
}

/*
/ leArquivo(grafo g, info do grafo)
/ fun��o que l� um arquivo de texto no diret�rio do projeto e, a medida que l� o arquivo, vai inserindo os n�s na lista de n�s
*/
void leArquivo(no *grafo, info *inf, int flag) //fun��o que le o arquivo que contem os vertices e arestas do grafo
{
    FILE *file;
    char nome[30];
    printf("\nDigite nome do arquivo:");
    gets(nome);
    file = fopen(nome, "r"); // aqui colocar o nome do arquivo que contem o grafo para leitura

    if(file==NULL)
    {
        printf("Arquivo nao encontrado");
        return;
    }
    int n_linhas,entrada,saida, cont;
    cont=1;

    fscanf(file, "%d", &n_linhas);
    while(!feof(file))  //enquanto le o arquivo, vai inserindo os nos e arestas
    {
        if(cont!=1){
        fscanf(file, "%d %d", &entrada, &saida);
        insereAresta(grafo,inf,entrada,saida,1,flag); //fun�ao que insere as arestas do grafo, e os nos caso eles nao existam
        }
        cont++;
    }
    fclose(file);
}

/*
/ incializa(grafo g)
/ fun��o inicializa o grafo com NULL
*/
void inicializa(no *grafo)
{
    grafo->prox=NULL;
    grafo->adj=NULL;
}

/*
/ inicializaVet(vetor v, tamanho do vetor)
/ fun��o que retorna vetor inicializado com '0' em todas as posi��es 'i' dele
*/
void inicializaVet(int *vet, int tam)
{
    int i;
    for(i=0;i<tam;i++)
    {
        vet[i]=0;
    }
}

/*
/ vazia(grafo g)
/ return 1(caso o grafo seja vazio)
/ return 0(caso o grafo n�o seja vazio)
/ fun��o que verifica se o grafo � vazio ou n�o
*/
int vazia(no *grafo)
{
    if(grafo->prox==NULL)
    {
        return 1;
    }
    else
        return 0;
}

/*
/ freqRelativa(grafo g, info do grafo)
/
/ Essa fun��o � respons�vel por criar um arquivo de sa�da contendo a frequ�ncia relativa de cada grau, ou seja, com que frequ�ncia
/ cada n� de grau: 1...n aparece no grafo. Para isso a fun��o chama a fun��o distribuicaoEmpirica, para preencher o vetor 'vet com
/ a quantidade de n�s, de cada grau 'x', que o grafo possui.
*/
void freqRelativa(no *grafo, info *inf)
{
    int vet[(inf->numNo)];
    int i;

    for(i=0;i<inf->numNo;i++)
    {
        vet[i]=0;
    }
    distribuicaoEmpirica(grafo,inf,vet); //chama a fun��o pra preencher o vetor "vet" de acordo com a distribui��o empirica

    FILE *file;                          //arquivo para escrita
    file = fopen("relatorio.txt","w");
    if(!file){
      printf( "Erro na abertura do arquivo");
      return;
    }

    int grauMedio = ((2*inf->numAresta)/inf->numNo);
    fprintf(file, "\nNumeros de vertices: %d \nNumeros de arestas: %d\nGrau medio: %d\nDustribuicao Empirica:\n",inf->numNo, inf->numAresta,grauMedio);
    for(i=0;i<inf->numNo;i++)
    {
        if(vet[i]!=0)
        {
            float freq = (float)(vet[i])/(inf->numNo);
            fprintf(file, "Frequencia relativa do grau %d : %f \n", i, freq);
        }
    }
    printf("\n\n Arquivo com dados de saida criado\n\n");
    fclose(file);
}

/*
/ distribuicaoEmpirica(grafo g, info do grafo, vetor v)
/
/ fun��o para calcular o n�mero de n�s n que possuem grau 'x', isso para todos os n�s. Utilizamos um m�todo com desperd�cio de
/ espa�o, em que assossiamos � posi��o i do vetor 'vet' com o grau de um n� e incrementamos em 1 caso mais de um n� possua o
/ mesmo grau
*/
void distribuicaoEmpirica(no *grafo, info *inf, int *vet) //fun��o para preencher o vetor com a quantidade de nos com grau i
{
    no *aux = grafo->prox;
    int cont=0;
    while(aux!=NULL)
    {
        aresta *a = aux->adj;
        while(a!=NULL) //conta o n�mero de arestas que o n� tem
        {
            cont++;
            a=a->prox;
        }
        vet[cont]=vet[cont]+1;
        cont=0;
        aux=aux->prox;
    }
}

/*
/ buscaNo(grafo g, id do n� a ser buscado)
/ return(retorna NULL caso o n� n�o exista ou um ponteiro para o n�, caso ele exista)
/ Essa fun��o realiza uma busca na lista de n�s comparando o id de cada n� com o valor 'num' passado.
*/
no *buscaNo(no *grafo, int num)
{
    no *aux = grafo->prox;
    if(aux!=NULL)
    {
        while(aux!=NULL)
        {
            if(aux->id==num)
            {
                return aux;
            }
            aux=aux->prox;
        }
    }
    else
    {
        return NULL;
    }
    return aux;
}

/*
/ imprimeGrafo(grafo g)
/ Essa fun��o imprime o id de todos os n�s do grafo
*/
void imprimeGrafo(no *grafo)
{
    no *tmp = grafo->prox;
    printf("\n Nos do grafo:\n");
    while(tmp!=NULL)
    {
        printf("\n%d", tmp->id);
        tmp=tmp->prox;
    }
}

/*
/ insereNo(grafo g, id do n� a ser inserido, info do grafo)
/ fun��o que insere um novo n� na lista de n�s. Insere na primeira posi��o caso n�o exista nenhum n�, ou percorre a lista at�
/ o �ltimo elemento e insere depois dele.
*/
void insereNo(no *grafo, int valor, info *inf, int flag)
{
    no *novo=(no*) malloc(sizeof(no)); //aloca espa�o para o novo elemento
    if(!novo)
    {
        printf("sem memoria disponivel\n");
        return;
    }
    novo->id=valor;
    novo->grau=0;
    novo->adj=NULL;
    novo->prox=NULL;

    if(vazia(grafo)) //se o grafo for vazio, insere na primeira posi��o da lista
    {
        grafo->prox = novo;
        inf->numNo++;
    }
    else
    {
        no *aux = buscaNo(grafo,valor);
        if(aux!=NULL)
        {
            printf("\n O no ja existe no grafo");
            free(novo);
        }
        else
        {
            no *tmp = grafo->prox;
            while(tmp->prox!=NULL)
            {
                tmp=tmp->prox;
            }
            tmp->prox=novo;
            inf->numNo++;//aumenta 1 no numero total de nos do grafo
        }
    }
}

/*
/ removeNo(grafo g, id do no a ser removido, info do grafo)
/ Essa fun��o utiliza a fun��o 'buscaNo' para verificar a exist�ncia do n� a ser removido, se ele existe, remove o n� da
/ lista de v�rtices, caso n�o exista, imprime um aviso.
*/
void removeNo(no *grafo, int valor, info *inf)
{
    if(grafo->prox==NULL) //verifica se grafo � vazio
    {
        printf("o grafo nao possui nos a ser excluidos");
    }
    else
    {
        no *aux = buscaNo(grafo,valor);
        if(aux==NULL) //verifica se o n� existe no grafo
        {
            printf("\no no nao existe no grafo");
        }
        else
        {
            no *aux=grafo;
            no *aux2=aux->prox;;
            while(aux2->id!=valor)
            {
                aux=aux->prox;
                aux2=aux2->prox;
            }
            if(aux2->adj!=NULL) //remove as arestas ligadas ao nos, antes de remover o no
            {
                int cont=0;
                aresta *a = aux2->adj;
                while(a!=NULL)////////////////come�a a remocao d arestas
                {
                    cont++;
                    a=a->prox;
                }
                int vet[cont];
                a= aux2->adj;
                int i=0;
                while(a!=NULL)
                {
                    vet[i]=a->sucessor;
                    i++;
                    a=a->prox;
                }
                for(i=0;i<cont;i++)
                {
                    excluiAresta(grafo,inf,valor,vet[i],0);
                }////////////////////////termina a remocao de arestas
            }
            aux->prox=aux2->prox;
            free(aux2);
            inf->numNo--;  //diminui 1 no numero total de nos do grafo
        }
    }
}

/*
/ imprimeAresta(grafo g)
/ Essa fun��o imprime todas as arestas adjacentes a determinado n�, caso exista.
*/
void imprimeAresta(no *grafo)
{
    aresta *tmp = grafo->adj;
    if(tmp==NULL)
    {
        printf("\nno %d nao possui arestas", grafo->id);
    }
    else
    {
        while(tmp!=NULL)
        {
            printf("%d\t", tmp->sucessor);
            tmp=tmp->prox;
        }
    }
}

/*
/ buscaAresta(grafo g, id do v�rtice sucessor)
/ return(retorna um ponteiro para a aresta buscada, caso ela exista)
/ Essa fun��o recebe um v�rtice como par�metro, e busca uma aresta adjacente a esse v�rtice e a outro v�rtice de id 'i'
*/
aresta *buscaAresta(no *vertice, int i)
{
    aresta *percorre = vertice->adj;
    while(percorre!=NULL)
    {
        if(percorre->sucessor==i)
        {
            return percorre;
        }
        percorre=percorre->prox;
    }
    return percorre;
}

/*
/ insereAresta(grafo g, id do primeiro n�, id do segundo n�, peso da aresta, info do grafo, flag para digrafo)
/
/ Essa fun��o come�a checando o flag, se for igual a 1, ent�o temos um digrafo e a aresta s� � inserida na lista de adjac�ncia
/ do n� origem. Caso flag seja 0, a aresta � inserida na lista de adjac�ncia dos 2 n�s.
/ Para isso n�s alocamos mem�ria para as arestas e buscamos se a lista de adjac�ncia � vazia,
/ caso positivo as aresta ja s�o inseridas na primeira posi��o da lista de adjac�ncia, caso negativo, buscamos se a aresta
/ ja existe, caso exista, liberamos a mem�ria alocada e finalizamos a fun��o, caso n�o exista, buscamos o n�
/ atrav�s do id 'i' e inserimos a aresta na lista de adjac�ncia.
*/
void insereAresta(no *grafo, info *inf, int i, int j, int peso, int flag)
{
    if(flag==1) //verifica se � um d�grafo
    {
        aresta *novo1=(aresta*)malloc(sizeof(aresta)); //aloca espa�o de mem�ria
        novo1->prox=NULL;
        novo1->sucessor=j;
        novo1->peso=peso;

        no *aux1 = buscaNo(grafo,i); //busca os n�s adjacentas � aresta
        no *aux2 = buscaNo(grafo,j);
        if(aux1==NULL) //insere o n� caso ele n�o exista
        {
            insereNo(grafo,i,inf,0);
            aux1=buscaNo(grafo,i);
        }
        if(aux2==NULL) //insere o n� caso ele n�o exista
        {
            insereNo(grafo,j,inf,0);
            aux2=buscaNo(grafo,j);
        }

        aresta *checar = buscaAresta(aux1,j);
        if(checar==NULL) //verifica se a aresta n�o existe
        {
            if(aux1->adj==NULL)
            {
                aux1->adj=novo1;
            }
            else
            {
                aresta *tmp = aux1->adj;
                while(tmp->prox!=NULL)
                {
                    tmp=tmp->prox;
                }
                tmp->prox = novo1;
            }
            inf->numAresta++;
            aux1->grau++;
            aux2->grau++; //incrementar como grau de entrada depois
        }
        else //se a aresta j� existe
        {
            free(novo1); //libera espa�o de mem�ria
        }
    }
    else // se n�o for d�grafo
    {
        aresta *novo1=(aresta*)malloc(sizeof(aresta));
        aresta *novo2=(aresta*)malloc(sizeof(aresta));
        novo1->prox=NULL;
        novo2->prox=NULL;
        novo1->sucessor=j;
        novo2->sucessor=i;
        novo1->peso=peso;
        novo2->peso=peso;
    ///////////////////////////////verifica se os nos existe, se nao, insere eles
        no *aux1 = buscaNo(grafo,i);
        no *aux2 = buscaNo(grafo,j);
        if(aux1==NULL) //insere n� caso n�o exista
        {
            insereNo(grafo,i,inf,0);
            aux1=buscaNo(grafo,i);
        }
        if(aux2==NULL) //insere n� caso n�o exista
        {
            insereNo(grafo,j,inf,0);
            aux2=buscaNo(grafo,j);
        }

        aresta *checar = buscaAresta(aux1,j);

        if(checar==NULL) //verifica se a aresta existe, caso n�o exista
        {
            if(aux1->adj==NULL) //insere aresta na primeira posi��o
            {
                aux1->adj=novo1;
            }
            else
            {
                aresta *tmp = aux1->adj;
                while(tmp->prox!=NULL) //busca ultima posi��o da lista de adj, para inserir a aresta
                {
                    tmp=tmp->prox;
                }
                tmp->prox = novo1;
            }
            if(aux2->adj==NULL)
            {
                aux2->adj=novo2;
            }
            else
            {
                aresta *tmp = aux2->adj;
                while(tmp->prox!=NULL)
                {
                    tmp=tmp->prox;
                }
                tmp->prox = novo2;
            }
            inf->numAresta++;
            aux1->grau++;
            aux2->grau++;
        }
        else //se a aresta ja existe
        {
            free(novo1);
            free(novo2);
        }
    }
}

/*
/ excluiAresta(grafo g, info do grafo, id do primeiro n�, id do segundo n�, flag para digrafo)
/
/ Essa fun��o exclui um aresta do grafo. Ela come�a verificando se estamos tratando de um digrafo ou um grafo simples, se for
/ digrafo, ent�o ela busca a aresta, se existir, e exclui a aresta da lista adjacente do n� de id 'i' cujo o sucessor � o n�
/ de id 'j'. Se estivermos tratando de um grafo simples, ent�o excluimos a aresta da lista de adjac�ncia de ambos os n�s de id
/ 'i' e 'j'. Para ambos os casos, verificamos a exist�ncia dos n�s e s� damos prosseguimendo � fun��o caso eles existam.
*/
void excluiAresta(no *grafo, info *inf, int i, int j, int flag)
{
    if(flag==1) //verifica se � d�grafo
    {
        no *n1 = buscaNo(grafo,i);
        no *n2 = buscaNo(grafo,j);
        if(n1==NULL || n2==NULL) //verifica se algum dos n�s n�o existe
        {
            if(n1==NULL){
                printf("\n n1 nao existe");}
            if(n2==NULL){
                printf("\n n2 nao existe");}
        }
        else //se ambos os n�s existm
        {
            aresta *a1 = buscaAresta(n1,j);
            if(a1!=NULL) //se a aresta existe
            {
                aresta *aux=n1->adj;
                if(aux->sucessor==j)
                {
                    n1->adj=aux->prox;
                    free(a1);
                }
                else
                {
                    while(aux->prox!=a1)
                    {
                        aux=aux->prox;
                    }
                    aux->prox=a1->prox;
                    free(a1);
                }
                inf->numAresta--;
                n1->grau--;
                n2->grau--; //grau de entrada do vertice 2
            }
            else
            {
                printf("\n aresta nao existe");
            }
        }
    }
    else{ // se � grafo simples
    no *n1 = buscaNo(grafo,i);
    no *n2 = buscaNo(grafo,j);
    if(n1==NULL || n2==NULL) //verifica se ambos os n�s existem
    {
        if(n1==NULL){
            printf("\n n1 nao existe");}
        if(n2==NULL){
            printf("\n n2 nao existe");}
    }
    else
    {
        aresta *a1 = buscaAresta(n1,j);
        aresta *a2 = buscaAresta(n2,i);
        if(a1!=NULL && a2!=NULL) //verifica se a aresta existe na lista de adj de ambos os n�s
        {
            aresta *aux=n1->adj;
            if(aux->sucessor==j)
            {
                n1->adj=aux->prox;
                free(a1);
            }
            else
            {
                while(aux->prox!=a1)
                {
                    aux=aux->prox;
                }
                aux->prox=a1->prox;
                free(a1);
            }
            aresta *aux2=n2->adj;
            if(aux2->sucessor==i)
            {
                n2->adj=aux2->prox;
                free(a2);
            }
            else
            {
                while(aux2->prox!=a2)
                {
                    aux2=aux2->prox;
                }
                aux->prox=a2->prox;
                free(a2);
            }
            inf->numAresta--;
            n1->grau--;
            n2->grau--;
        }
        else // se a aresta n�o existe
            printf("\n aresta nao existe");
    }}
}

/*
/ retornaGrauNo(grafo g, id do n�)
/ return(retorna o grau do n�, com id = 'i', passado por par�metro)
/ Essa fun��o busca o n� de id 'i' passado por valor, caso exista, retorna seu grau.
*/
int retornaGrauNo(no *grafo, int i)
{
    no *aux = buscaNo(grafo,i);
    if(aux!=NULL)
    {
        return aux->grau;
    }
    else
        return 0;
}

/*
/ kRegular(grafo g)
/ return(retorna -1 ou cont)
/ Essa fun��o verifica a k-regularidade do grafo. Primeiro guardamos o grau do primeiro n� do grafo e comparamos todos os
/ outros com ele, se algum for diferente, ent�o o grafo n�o � regular e retornamos -1, caso todos sejam iguais,
/ ent�o retornamos 'cont', correspondente � cont-regularidade do grafo.
*/
int kRegular(no *grafo)
{
    no *aux = grafo->prox;
    if(aux==NULL) //se grafo for vazio
    {
        printf("\ngrafo nao possui nos");
        return -1;
    }
    else
    {
        int cont = aux->grau; //cont recebe valor do grau do primeiro n�

        while(aux!=NULL)
        {
            if(aux->grau!=cont) //verifica se cont = grau de todos os outros n�s
            {
                return -1; //caso algum n� tenha grau diferente, retorna -1
            }
            aux = aux->prox;
        }
    return cont;
    }
}

/*
/ completo(grafo g, info do grafo)
/ return(1 ou 0)
/ Essa fun��o utiliza a fun��o kRegular para verificar se o grafo � k-regular, se sim, ent�o verificamos se ele � n-1 regular,
/ sendo n = n�mero de n�s do grafo, caso positivo, ent�o o grafo � completo e retornamos 1, ao contr�rio, retornamos 0.
*/
int completo(no *grafo, info *inf)
{
    int regular;
    regular=kRegular(grafo);
    if(regular>=0)
    {
        if(regular == (inf->numNo-1))
        {
            return 1; // grafo completo
        }
        else
            return 0;
    }
    else
        return 0;
}

/*
/ adjacente(grafo g, id do primeiro n�, id do segundo n�)
/ return(1 ou 0)
/ Essa fun��o busca os n�s de id 'i' e 'j', caso eles existam, busca se existe aresta entre eles.
*/
int adjacente(no *grafo, int i, int j)
{
    no *n1 = buscaNo(grafo,i);
    no *n2 = buscaNo(grafo,j);
    aresta *a1 = buscaAresta(n1,j);
    aresta *a2 = buscaAresta(n2,i);
    if(n1!=NULL && n2!=NULL)
    {
        if(a1!=NULL || a2!=NULL)
        {
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}

/*
/ proxComponente(grafo g, info do grafo, vetor marcando os n� visitados)
/ return(i=posi��o do pr�ximo n� n�o visitado, -1=nenhuma posi��o n�o visitada)
/ Essa � uma fun��o auxiliar da fun��o 'grafoConexo', ela verifica no vetor 'visitados' a pr�xima posi��o, sempre a partir da
/ primeira, que possui o valor '0', indicando que o v�rtice na posi��o 'i', da lista de v�rtices, n�o foi visitado ainda.
*/
int proxComponente(no *grafo,info *inf, int *visitados)
{
    int i;
    for(i=0; i<inf->numNo; i++)
    {
        if(visitados[i]==0)
        {
            return i;
        }
    }
    return -1;
}

/*
/ buscaPosi��o(grafo g, id do n� buscado)
/ retorna(i = posi��o na lista de v�rtice na qual o n� buscado se encontra)
/ Essa � uma fun��o auxiliar que busca e retorna qual posi��o se encontra, na lista de v�rtices, determinado n�.
*/
int buscaPosicao(no *grafo, int val)
{
    int i = 0;
    no *aux = grafo->prox;
    while(aux->id!=val && aux!=NULL)
    {
        i++;
        aux=aux->prox;
    }
    return i;
}

/*
/ buscaProfundidade(grafo g, info do grafo, vetor com a posi��o dos n�s visitados, posi��o do n� a ser visitado,
/ val corresponde � componente conexa a qual estamos)
/ Essa fun��o � uma fun��o recursiva, utilizada pela fun��o grafoConexo, para percorrer todos os n�s de um grafo.
/ Atrav�s dessa fun��o, percorremos todas as arestas e n�s a partir de um determinado n� e paramos quando n�o h� mais n�s a serem
/ visitados.
*/
void buscaProfundidade(no *grafo, info *inf, int *visitados, int pos, int val)
{
    visitados[pos] = val; // marca o n� como visitado
    no *aux = grafo->prox;
    int i=0;
    while(i!=pos) //achar o n� de acordo com a posi��o dele na lista de v�rtices(lista vertical)
    {
        aux=aux->prox;
        i++;
    }
    aresta *are = aux->adj;
    while(are!=NULL)
    {
        if(visitados[buscaPosicao(grafo,are->sucessor)] == 0) //verifica se o n� adjacente foi visitado
        {
            buscaProfundidade(grafo,inf,visitados,buscaPosicao(grafo,are->sucessor),val); // se nao foi visitado, chama ele recusivamente
        }
        are=are->prox; //se foi visitado, chama pr�ximo n� adjacente
    }
}

/*
/ grafoConexo(grafo g, info do grafo, vetor de v�rtices visitados, maior componente conexa, menor componente conexa)
/ return(k = n�mero de componentes conexas do grafo)
/
/ Essa � a fun��o "principal" para descobrir o n�mero de componentes conexas do grafo. Ela cria o vetor para marcar os n�s
/ visitados(de acordo com sua componente conexa), conta o n�mero de componentes conexas do grafo e chama as fun��es
/ 'BuscaProfundidade' e 'proxComponente'. Al�m disso, ela conta quantos v�rtices tem cada componente conexa, salva a componente
/ com n�mero menor e maior de v�rtices e imprime esses valores.
*/
int grafoConexo(no *grafo, info *inf, int *visitados) //**********
{
    int i;
    int k=0;
    no *aux = grafo->prox;
    for(i=0; i<inf->numNo; i++) //incializa��o do vetor 'visitados'
    {
        visitados[i]=0;
    }
    if(aux!=NULL) // se grafo n�o � vazio
    {
        int pos=proxComponente(grafo,inf,visitados); //pega o primeiro no nao visitado, no caso o n� da posi��o 0 do vetor
        while(pos>=0)
        {
            k++;                                     //incrementa em 1 o n�mero de componenstes conexas do grafo
            buscaProfundidade(grafo,inf,visitados,pos,k);
            pos=proxComponente(grafo,inf,visitados);  //busca a posi��o do pr�ximo n�, n�o visitado, do grafo
        }
    }
    else
    {
        printf("\n grafo vazio");
        return k;
    }
    /////////////////////////////////////////////////////
    int j, cont=0,x=1;
    int maior=0, menor=0;
    while(x<=k) //aqui busca o tamanho da maior e da menor componente do grafo
    {
        for(j=0; j<=inf->numNo; j++)
        {
            if(visitados[j]==x)
            {
                cont++;
            }
        }
        if(x==1)
        {
            maior=cont;
            menor=cont;
        }
        else
        {
            if(maior<cont)
            {
                maior=cont;
            }
            if(menor>cont)
            {
                menor=cont;
            }
        }
        cont=0;
        x++;
    }
    //printf("\n maior componente: %d, menor componente: %d", maior, menor);
    return k; //retorna n�mero de componentes conexas do grafo
}

/*
/ noArticulacao(grafo g, id do n�, info do grafo)
/ return(retorna 1 se � n�s de articula��o, 0 caso contr�rio)
/ Essa fun��o verifica se dado n� � de articula��o, para isso ela verifica quantas componentes conexas o grafo tem, a seguir
/ remove o n� do grafo e ent�o verifica o n�mero do componentes conexas novamente, se o n�mero de componentes aumentou, ap�s
/ a remo��o do n�, ent�o o n� � de articula��o.
*/
int noArticulacao(no *grafo, int id, info *inf)
{
        int validar = 0;
        int visitados[inf->numNo];
        int comp = grafoConexo(grafo,inf,visitados); //analiza quantas comp. conexas o grafo tem
        removeNo(grafo,id,inf); //remove no
        int novo = grafoConexo(grafo,inf,visitados); //verifica novamente a conexidade do grafo, se a conexidade aumentou em 1, o no � de articula��o
        if(novo>comp)
        {
            validar=1;
        }
        return validar;
}

/*
/ arestaPonte(grafo g, id do primeiro n�, id do segundo n�, info do grafo)
/ return(1 se aresta � ponte, 0 caso contr�rio)
/ Essa fun��o tem a mesma id�ia da fun��o noArticulacao.
/ 1�: verifica o n�mero de componentes conexas do grafo
/ 2�: remove aresta
/ 3�: verifica o n�mero de componentes conexas novamente, se for maior, ent�o aresta � ponte
*/
int arestaPonte(no *grafo, int id1, int id2, info *inf) // verifica se dada aresta � ponte
{
        int validar = 0;
        int visitados[inf->numNo];
        int i = grafoConexo(grafo,inf,visitados); //similar a fun��o "noArticulacao"
        excluiAresta(grafo,inf,id1,id2,0);
        int novo = grafoConexo(grafo,inf,visitados);
        if(novo>i)
        {
            validar = 1;
        }
        return validar;
}

/*
/ quickSort(lista de quaisquer valores, �ndice para o in�cio do vetor, �ndice para o fim do vetor, matriz auxiliar)
/ Fun��o de ordena��o
*/
void quickSort(int *lista, int iniVet,int fimVet,int mat[][2])
{
    int i,j,pivo,aux,aux2,aux3;
    i=iniVet;
    j=fimVet;
    pivo = lista[(i+j)/2];
    while(i<j)
    {
        while(lista[i]<pivo)
        {
            i=i+1;
        }
        while(lista[j]>pivo)
        {
            j=j-1;
        }
        if(i<=j)
        {
            aux = lista[i];
            lista[i]=lista[j];
            lista[j]=aux;
            aux2=mat[i][0];
            aux3=mat[i][1];
            mat[i][0]=mat[j][0];
            mat[i][1]=mat[j][1];
            mat[j][0]=aux2;
            mat[j][1]=aux3;
            i=i+1;
            j=j-1;
        }
    }
    if(j>iniVet)
    {
        quickSort(lista,iniVet,j,mat);
    }
    if(i<fimVet)
    {
        quickSort(lista,i,fimVet,mat);
    }
}

/*
/ buscaNo2(grafo, posi��o do n� na lista de adj)
/ retorna um ponteiro pro n� na posi��o 'pos'
/ Fun��o que busca o n� que se encontra na posi��o 'pos' da lista de adj. Ao inv�s de percorrer e buscar o n� na lista de
/ adjac�ncia, essa fun��o j� recebe em que posi��o o n� est� e faz a busca.
*/
no *buscaNo2(no *grafo, int pos) //fun��o que busca e retorna o n� na posi��o 'pos'
{
    no *aux = grafo->prox;
    int i = 0;
    while(i!=pos)
    {
        aux=aux->prox;
        i++;
    }
    return aux;
}

/*
/ cliqueMax(grafo g, info do grafo, vetor auxiliar para guardar a clique max)
/
/ Essa fun��o trabalha com cada componente conexa do grafo tentando encontrar a clique m�xima atrav�s de um algoritmo exaustivo.
/ N�s testamos cada n� de cada componente como subconjuntos e criamos um vetor para armazenar sua clique, por exemplo:
/ pegamos o n� 'x' da componente 'k', e criamos um vetor vetClique para esse n�, ent�o fazemos o seguinte teste,
/ para cada v�rtice adjacente a esse n� 'x', incluimos ele no vetor se ele for adjacente a todos os outros n�s desse vetor.
/
/ ex: grafo: 1 2       'esse com apenas 1 componente conexa, para exemplo'
/            2 3
/            1 3
/            1 4
/ vamos pegar o n� 1.
/ inserimos 1 no vetor. vetClique = {1)
/ verificamos 2, 2 � adjacente a 1? sim
/ inserimos 2. vetClique = {1,2}
/ verificamos 3, 3 � adjacente a 1 e 2? sim
/ inserimos 3. vetClique = {1,2,3}
/ por fim, verificamos 4. 4 � adjacente a 1,2,3? n�o, apenas a 1. n�o inserimos 4.
/ cliqueMax = vetClique = {1,2,3}
*/
void cliqueMax(no *grafo, info *inf, int *maxClique)
{
    int visitados[inf->numNo]; // vetor para guardar a comp. conexa de cada n�
    int k = grafoConexo(grafo,inf,visitados); // k recebe o num de componentes conexas do grafo
    int cont=0, i, j;
    int compo=1;   //vari�vel que indica em qual comp. conexa estamos atuando
    int num=0, num2=0;
    int l=0;       //indices do vetor clique
    int change=0; //flag para mudan�a de vetor

    no *noAux;
    aresta *areAux;

    while(compo<=k) // iremos percorrer todas as componentes do grafo de 1 a k
    {
        cont=0;
        for(i=0;i<inf->numNo;i++)  //conta n�mero de n�s da componente x
        {
            if(visitados[i]==compo)
            {
                cont++;
            }
        }
        int vetAux[cont]; //cria vetor com tamanho = n�mero de n�s da componente x
        j=0;
        for(i=0;i<inf->numNo;i++) //preencher o vetor com a posi��o dos n�s da componente x
        {
            if(visitados[i]==compo)
            {
                vetAux[j]=i;
                j++;
            }
        }

        int vetClique[cont]; // vetor auxiliar para achar cliques
        for(i=0;i<cont;i++) //inicializa vetClique
        {
            vetClique[i]=0;
        }
        ////////////////////////////////////// ***** in�cio da busca por cliques ******
        for(i=0;i<cont;i++)
        {
            l=0;
            noAux = buscaNo2(grafo, vetAux[i]); // busca o n� da posi��o 'i'
            areAux = noAux->adj;
            vetClique[l]=noAux->id; // insere o primeiro n� na posi��o '0' do vetClique
            l++;
            while(areAux!=NULL) //verifica se as arestas forma clique
            {
                num2=0;
                for(j=0;j<l;j++) //verifica se as arestas s�o clique
                {
                    if(adjacente(grafo,vetClique[j],areAux->sucessor)==1) //compara a adjacencia aresta com cada no do vetClique
                    {
                        num2++;  //incrementa vari�vel contendo o n�mero de arestas adjacentes
                    }
                }
                if(num2==l) // se a aresta for adjacente a todos e ela n�o existe no vetor, insere ela no vetClique
                {
                    vetClique[l]=areAux->sucessor;
                    l++;
                }
                if(l>num) // se l > num, ent�o temos uma clique maior que a que tinhamos, change receber o valor 1 ent�o.
                {
                    num=l;
                    change=1;
                }
                areAux=areAux->prox;
            }
            if(change==1) // se change igual a 1, ent�o nosso vetor maxClique recebe o vetor vetClique
            {
                inicializaVet(maxClique,l);
                for(j=0;j<l;j++)
                {
                    maxClique[j]=vetClique[j];
                }
                change=0;
            }
            for(j=0;j<cont;j++) //inicializa vetClique
            {
                vetClique[j]=0;
            }
        }
    compo++; //incrementa compo para a proxima componete conexa
    }
    imprimeClique(maxClique,num); // ao final de toda a busca, chama fun��o para imprimir a clique m�xima
}

/*
/ imprimiClique(vetor contendo a clique, tamanho do vetor)
/ fun��o para imprimir vetor contendo a clique m�xima.
*/
void imprimeClique(int *vet, int tam)
{
    int i;
    int c=0;
    printf("\n CLIQUE MAX:\t");
    for(i=0;i<tam;i++)
    {
        c++;
        //printf("\t%d", vet[i]);
    }
    printf("%d",c);
}

/*
/ fechoDireto(grafo g, v�rtice v, info do grafo)
/ Fun��o que calcula o fechoDireto do no v�rtice 'v' passado na fun��o. Essa fun��o chama a fun��o buscaProfundidade
/ para o v�rtice 'v' e guarda todos os n�s alcan�ados a partir de dele. Esses v�rtices formam o fechoDireto do v�rtice.
*/
void fechoDireto(no *grafo, no *vertice, info *inf)
{
    int fechoD[inf->numNo];
    int i;
    for(i=0;i<inf->numNo;i++) //inicializa vetor para guardar o fecho direto
    {
        fechoD[i]=0;
    }

    no *aux = grafo->prox;
    int cont=0;
    while(aux->id!=vertice->id) //busca posi��o do v�rtice na lista de adj
    {
        aux=aux->prox;
        cont++;
    }
    buscaProfundidade(grafo,inf,fechoD,cont,1); //faz a busca em profundidade, passando a posi��o do v�rtice como a posi��o inicial da busca

    for(i=0;i<inf->numNo;i++) // verifica os n�s alcan�ados
    {
        if(fechoD[i]==1)
        {
            aux=buscaNo2(grafo,i);
            printf("\t%d",aux->id);
        }
    }
}

/*
/ grafoTransposto(grafo g, grafo g(t): ir� conter o transposto de g, info do grafo transposto)
/ Essa fun��o insere arestas de v->u respectivamente, no grafo transposto, para cada aresta de u->v continda no grafo 'g'.
/ Ou seja, cria um grafo transposto de 'g'.
*/
void grafoTransposto(no *grafo, no* grafoTrans, info *infTrans)
{
    no *aux = grafo->prox;
    aresta *a;
    while(aux!=NULL) //percorre o grafo 'g'
    {
        a = aux->adj;
        while(a!=NULL)
        {
            insereAresta(grafoTrans,infTrans,a->sucessor,aux->id,a->peso,1); //insere as arestas no sentido oposto
            a=a->prox;
        }
        aux=aux->prox;
    }
}

/*
/ fechoIndireto(grafo g, vertice v, info do grafo)
/ Essa fun��o calcula o fecho indireto do v�rtice 'v', para isso usamos a mesma id�ia de busca em profundidade que usamos para
/ calcular o fecho direto, exceto que nessa fun��o usamos a busca a partir do grafo transposto de 'g' para encontrar os n�s
/ que alcan�am 'v'.
*/
void fechoIndireto(no *grafo, no *vertice, info *inf)
{
    int fechoD[inf->numNo];
    int i;
    for(i=0;i<inf->numNo;i++)
    {
        fechoD[i]=0;
    }
    no *grafoTrans = (no*) malloc(sizeof(no)); //cria grafo auxiliar para ser o grafo transposto de 'g'
    if(!grafo)
    {
        printf("\n sem espaco");
        exit(1);
    }
    //aloca espa�o de mem�ria para ponteiro do tipo info
    info *infTrans = (info*)malloc(sizeof(info));
    infTrans->numAresta=0;
    infTrans->numNo=0;
    inicializa(grafoTrans);

    grafoTransposto(grafo,grafoTrans,infTrans); //chama fun��o para calcular o transposto do grafo 'g'

    no *aux = grafoTrans->prox;
    int cont=0;
    while(aux->id!=vertice->id) //acha a posi��o do n� 'v�rtice' nas lista de adj
    {
        aux=aux->prox;
        cont++;
    }
    buscaProfundidade(grafoTrans,inf,fechoD,cont,1); // faz a busca em profundidade do grafo, a partir da posi��o do respectivo n�
    //imprime fecho indireto
    for(i=0;i<inf->numNo;i++)
    {
        if(fechoD[i]==1)
        {
            aux=buscaNo2(grafo,i);
            printf("\t%d",aux->id);
        }
    }
    free(grafoTrans);
    free(infTrans);
}

/*
/ minKeu(vetor key, vetor mstSet, info do grafo g)
/ retorna min_index
/ Fun��o auxiliar da fun��o prim
*/
int minKey(int key[], bool mstSet[], int c)
{
    // Initialize min value
    int mi = 999, min_index;
    int v;
    for (v = 0; v < c; v++)
    {
        if (mstSet[v] == false && key[v] < mi)
        {
            mi = key[v];
            min_index = v;
        }
    }
    return min_index;
}

/*
/ prim(grafo g, info do grafo)
/ fun��o gulosa que retorna a AGM do grafo 'g', percorrendo-se todos os n�s do grafo.
*/
void prim(no *grafo, info *inf)
{
    int tam=0;
    int vet[inf->numNo];
    int comp = grafoConexo(grafo,inf,vet);
    int k;
    for(k=1; k<=comp; k++)
    {
        int j,i, c=0, c2=0;
        for(j=0; j<inf->numNo; j++)
        {
            if(vet[j]==k)
            {
                c++;
            }
        }
        int vetP[c];
        c=0;
        for(j=0; j<inf->numNo; j++)
        {
            if(vet[j]==k)
            {
                vetP[c]=j;
                c++;
            }
        }

        int pais[c];
        int chave[c];
        bool mstSet[c];
        int graph[c][c];

        for(i=0;i<c;i++) //inicializa matriz
        {
            for(j=0;j<c;j++)
            {
                graph[i][j]=0;
            }
        }

        no *aux = buscaNo2(grafo,vetP[c2]);
        aresta *a = aux->adj;
        int ps, l=0;
        if(a!=NULL)
        {
            for(i=0; i<c; i++) // preenche a matriz com o peso de cada aresta
            {
                aux = buscaNo2(grafo, vetP[i]);
                a = aux->adj;
                while(a!=NULL)
                {
                    ps = buscaPosicao(grafo,a->sucessor);
                    for(j=0;j<c;j++)
                    {
                        if(vetP[j]==ps)
                        {
                            l=j;
                        }
                    }
                    graph[i][l]=a->peso;
                    a = a->prox;
                }
            }

            printf("\n");
            for(i=0; i<c; i++)
            {
                chave[i] = 999;
                mstSet[i] = false;
            }

            chave[0] = 0;
            pais[0] = -1;

            int cont;
            for(cont=0; cont< c-1; cont++) //busca pelo caminho m�nimo
            {
                int u = minKey(chave,mstSet,c);

                mstSet[u] = true;
                int v;
                for(v=0; v<c; v++)
                {
                    if (graph[u][v] && mstSet[v] == false && graph[u][v] <  chave[v])
                    {
                        pais[v]=u;
                        chave[v] = graph[u][v];
                    }
                }
            }
            printf("componente %d: \n", k);
            for (i = 1; i < c; i++) //calcular tamanho do caminho m�nimo
            {
                tam = tam + graph[pais[i]][i];
                printf("%d - %d    %d \n", buscaNo2(grafo,vetP[pais[i]])->id, buscaNo2(grafo,vetP[i])->id, graph[pais[i]][i]); //imprimir arestas do caminho min
            }

        }
    }

    printf("\ntamanho do caminho minimo: %d\n", tam);
}

/*
/ compForteConexa(grafo g, info do grafo)
/ Essa fun��o usa a id�ia do algoritmo de Tarjan que segue os seguintes passos:
/ 1- Chama a fun��o de busca em profundidade computando, em um vetor, o tempo de cada n� do grafo
/ 2- computamos o grafo transposto de g
/ 3- chamamos a fun��o de busca em profundida para o grafo transposto, considerando o tempo de t�rmino de cada n�, em ordem,
/ decrescente
/ 4- imprimimos cada �rvore como uma componente fortemente conexa
/ Essa fun��o utiliza mais 3 fun��es auxiliares, 2 para busca em profundidade e uma para pegar o o maior elemento do vetor
/ contendo a ordem que cada v�rtice foi visitado
*/
void compForteConexa(no *grafo, info *inf)
{
    int ordem[inf->numNo]; // vetor que ir� conter a ordem em que cada n� foi visitado
    int visitados[inf->numNo]; // vetor auxiliar para a fun��o de busca em profundidade, nele teremos os n�s ja visitados
    int i;

    inicializaVet(ordem,inf->numNo);
    inicializaVet(visitados,inf->numNo);

    int *vetp =(int*)malloc(sizeof(int)); // ponteiro auxiliar que cont�m valor referente a posi��o que cada n� est� sendo visitado
    vetp[0] = 0;
    int k=0;

    int pos=proxComponente(grafo,inf,visitados); //pega o primeiro no nao visitado, no caso o n� da posi��o 0 do vetor
    while(pos>=0) //busca em profundidade para os n�s do grafo
    {
        k++;
        buscaProfundidade2(grafo,inf,visitados,pos,k,ordem,vetp);
        pos=proxComponente(grafo,inf,visitados);
    }

    /////////////////////// inverter vetor 'ordem' => desnecess�rio, mas como j� haviamos feito algumas mudan�as, deixamos para
    ////////////////////// remover isso posteriormente
    int novo[inf->numNo];
    int ind = inf->numNo-1;
    for(i=0;i<inf->numNo;i++)
    {
        novo[i] = ordem[ind];
        ind--;
    }
    ///////////////////////

    no *grafoTrans = (no*) malloc(sizeof(no)); //cria grafo auxiliar para ser o grafo transposto
    if(!grafo)
    {
        printf("\n sem espaco");
        return;
    }
    //aloca espa�o de mem�ria para ponteiro do tipo info
    inicializa(grafoTrans);
    info *infTrans = (info*) malloc(sizeof(info));
    infTrans->numAresta=0;
    infTrans->numNo=0;
    grafoTransposto(grafo,grafoTrans,infTrans);

    inicializaVet(visitados,inf->numNo);
    k=0;
    pos=proxComponente2(grafoTrans,infTrans,visitados,novo);
    while(pos>=0) //busca em profundidade, dessa vez imprimindo cada componente forte conexa
    {
        k++;
        printf("\n componente %d: ",k);
        buscaProfundidade3(grafoTrans,infTrans,visitados,pos,k);
        pos=proxComponente2(grafoTrans,infTrans,visitados,novo);
        //pos = -1;
    }

    free(vetp);
    free(grafoTrans);
    free(infTrans);
}
/*
/ proxComponente2(grafo g, info do grafo, vetor contendo os n�s visitados, vetor contendo a ordem que cada v�rtice foi visitado)
/ return(p = posi��o do pr�ximo n� a ser visitado)
/ Fun��o auxiliar da fun��o de componente fortemente conexa que retorna o pr�ximo n� que ainda n�o foi visitado e que possui
/ o maior valor no vetor 'novo'
*/
int proxComponente2(no *grafo, info *inf, int *visitados, int *novo)
{
    int i;
    int p=-1;
    int k;
    for(i=0; i<inf->numNo; i++)
    {
        k = novo[i];
        if(visitados[k]==0 && p==-1) //se n� nao foi visitado ainda, e ele p = -1, p recebe a posi��o desse n�
        {
            p = novo[i];
        }
        if(visitados[k]==0 && novo[i]>p) // se n� n�o foi visitado e seu valor no vetor 'novo' for maior que o valor de 'p',
        {                                // 'p' recebe a posi��o desse n�
            p=novo[i];
        }
    }
    return p;
}

/*
/ buscaProfundidade2(grafo g, info do grafo, vetor auxiliar contendo os n�s que ja foram visitados, posi��o do primeiro n�,
/ valor da componente em que os n�s est�o, vetor para contar a ordem em que cada n� ser� visitado, ponteiro contendo
/ o valor referente a ordem que cada n� est� sendo visitado)
/
/ A �nica diferen�a dessa busca em profundidade � que na volta da recurs�o, salvamos no vetor 'ordem' a ordem em que cada n�
/ est� sendo visitado
*/
void buscaProfundidade2(no *grafo, info *inf, int *visitados, int pos, int val, int *ordem, int *vetp)
{
    visitados[pos] = val; // marca o n� como visitado
    no *aux = grafo->prox;
    int i=0;
    while(i!=pos) //achar o n� de acordo com a posi��o dele na lista de v�rtices(lista vertical)
    {
        aux=aux->prox;
        i++;
    }
    aresta *are = aux->adj;

    while(are!=NULL)
    {
        if(visitados[buscaPosicao(grafo,are->sucessor)] == 0) //verifica se o n� adjacente foi visitado
        {
            buscaProfundidade2(grafo,inf,visitados,buscaPosicao(grafo,are->sucessor),val,ordem,vetp); // se nao foi visitado, chama ele recusivamente
        }
        are=are->prox; //se foi visitado, chama pr�ximo n� adjacente
    }
    ////////////////////// salva no vetor 'ordem' a ordem em que cada n� foi visitado
    int k = vetp[0];
    ordem[k]=pos;
    vetp[0] = k+1; //incrementa o valor no ponteiro 'vetp'

}

/*
/ buscaProfundidade2(grafo g, info do grafo, vetor auxiliar contendo os n�s que ja foram visitados, posi��o do n� inicial,
/ valor da componente em que os n�s est�o)
/
/ A �nica diferen�a dessa fun��o de busca em profundidade, � que ela vai imprimindo os n�s de cada �rvore contida no grafo
/ de acordo com a posi��o inicial 'pos' passada
*/
void buscaProfundidade3(no *grafo, info *inf, int *visitados, int pos, int val)
{
    visitados[pos] = val; // marca o n� como visitado
    no *aux = grafo->prox;
    int i=0;
    while(i!=pos) //achar o n� de acordo com a posi��o dele na lista de v�rtices(lista vertical)
    {
        aux=aux->prox;
        i++;
    }
    printf("\t%d", aux->id);
    aresta *are = aux->adj;

    while(are!=NULL)
    {
        if(visitados[buscaPosicao(grafo,are->sucessor)] == 0) //verifica se o n� adjacente foi visitado
        {
            buscaProfundidade3(grafo,inf,visitados,buscaPosicao(grafo,are->sucessor),val); // se nao foi visitado, chama ele recusivamente
        }
        are=are->prox; //se foi visitado, chama pr�ximo n� adjacente
    }
}

/*
/ kruskal(grafo g, info do grafo)
/ fun��o para calcular a AGM do grafo. Utiliza a fun��o auxiliar 'menorAresta'
*/
void kruskal(no *grafo, info *inf)
{
    int vet[3];            // vetor que guarda na posi��o 0,1 e 2, id do n� 1, id do n� 2 e peso da aresta entre eles
    inicializaVet(vet,3);

    no *g2=(no*) malloc(sizeof(no)); //cria grafo auxiliar para ser o grafo transposto
    if(!g2)
    {
        printf("\n sem espaco");
        exit(1);
    }
    //aloca espa�o de mem�ria para ponteiro do tipo info
    inicializa(g2);
    info *i2 = (info*) malloc(sizeof(info)); //info do grafo auxiliar
    i2->numAresta=0;
    i2->numNo=0;

    while(vet[2]!=-1)
    {
        menorAresta(grafo,inf,vet);               // chama fun��o para pegar a menor aresta do grafo 'g'
        printf("\nok");
        insereAresta(g2,i2,vet[0],vet[1],vet[2],0); // insere no novo grafo 'g2'
        if(arestaPonte(g2,vet[0],vet[1],i2)==1) // verifica se essa aresta � ponte
        {
            insereAresta(g2,i2,vet[0],vet[1],vet[2],0); // caso seja, insere ela, definitivamente, no grafo 'g2'
        }
    }

    no *lala = g2->prox;
    aresta *lele = lala->adj;
    while(lala!=NULL)  // agora percorremos o grafo 'g2', que � nossa AGM, e imprimimos suas arestas
    {
        lele=lala->adj;
        while(lele!=NULL)
        {
            printf("\n%d-%d : peso: %d", lala->id, lele->sucessor, lele->peso);
            excluiAresta(g2,i2,lala->id,lele->sucessor,0);
            lele=lele->prox;
        }
        lala=lala->prox;
    }
    free(g2);
    free(i2);
}

/*
/ menorAresta(grafo g, info do grafo, vetor que ir� conter: id do n�1, id do n�2, peso da aresta entre n�1 e n�2)
/ Fun��o auxiliar da fun��o Kruskal que busca no grafo 'g' a aresta de menor peso
*/
void menorAresta(no *grafo, info *inf, int *vet)
{
    no *aux = grafo->prox;
    aresta *a;
    int no1,no2,menor;

    menor = -1;
    no1 = -1;
    no2 = -1;

    while(aux!=NULL) //busca pela aresta de menor peso
    {
        a=aux->adj;
        while(a!=NULL)
        {
            if(menor==-1)
            {
                menor =  a->peso;
                no1 = aux->id;
                no2 = a->sucessor;
            }
            if(a->peso<menor)
            {
                menor = a->peso;
                no1 = aux->id;
                no2 = a->sucessor;
            }
            a=a->prox;
        }
        aux=aux->prox;
    }

    vet[0]=no1; //insere id do n�1
    vet[1]=no2; //insere id do n�2
    vet[2]=menor; // insere peso da aresta entre n�1 e n�2

    excluiAresta(grafo,inf,no1,no2,0); // ap�s toda a busca, removemos a aresta do grafo g

}

/*
/ cliqueGulosa(grafo, inf do grafo)
/ fun��o que usa a heur�stica gulosa para calcular uma clique
*/
void cliqueGulosa(no *grafo, info *inf)
{
    int p[inf->numNo];
    int posicao[inf->numNo];
    int g[inf->numNo];
    int graus[inf->numNo];

    no *aux = grafo->prox;
    int tam=0,i,k,j=0;
    while(aux!=NULL)
    {
        g[tam]=aux->grau;
        p[tam]=tam;
        tam++;
        aux=aux->prox;
    }

    int mat[tam][2];
    quickSort2(g,p,0,(tam-1),mat); //ordena nos da clique de acordo com o grau

    //inverte o vetor para ordem decrescente
    k=tam-1;
    for(i=0;i<tam;i++)
    {
        posicao[k]=p[i];
        graus[k]=g[i];
        k--;
    }

    int vetClique[graus[0]+1];
    inicializaVet(vetClique,graus[0]+1);
    int novoTam=tam;
    int vetP[tam];
    int vetG[tam];
    for(i=0;i<tam;i++)
    {
        vetP[i]=-1;
        vetG[i]=-1;
    }

    //acha a clique
    while(novoTam>0)
    {
        k=0;
        vetClique[j]=posicao[0];

        aux = buscaNo2(grafo,posicao[0]);
        for(i=0;i<novoTam;i++)
        {
            if(adjacente(grafo,aux->id,buscaNo2(grafo,posicao[i])->id))
            {
                vetG[k] = graus[i];
                vetP[k] = posicao[i];
                k++;
            }
        }
        novoTam = k;
        for(i=0;i<tam;i++)
        {
            posicao[i] = vetP[i];
            graus[i] = vetG[i];
        }
        for(i=0;i<tam;i++)
        {
            vetP[i] = -1;
            vetG[i] = -1;
        }

        j++;
    }

    printf("\nEncontrado\n");
    printf("\nTamanho da clique encontrada: %d", j);
    int si=0;
    printf("\nDeseja imprimir os nos dessa clique? 1-sim, 2-nao");
    scanf("%d", &si);
    if(si==1)
    {
        for(i=0;i<j;i++)
        {
            printf("%d\n", buscaNo2(grafo,vetClique[i])->id);
        }
    }
   // printf("\n\n MAIOR GRAU: %d", buscaNo2(grafo,vetClique[0])->grau);
}


/*
/ quickSort2(lista,lista, �ndice para o in�cio do vetor, �ndice para o fim do vetor, matriz auxiliar)
/ Fun��o de ordena��o de dois vetores simultaneamente
*/
void quickSort2(int *lista, int *lista2, int iniVet,int fimVet,int mat[][2])
{
    int i,j,pivo,aux,aux2,aux3;
    int l2;
    i=iniVet;
    j=fimVet;
    pivo = lista[(i+j)/2];
    while(i<j)
    {
        while(lista[i]<pivo)
        {
            i=i+1;
        }
        while(lista[j]>pivo)
        {
            j=j-1;
        }
        if(i<=j)
        {
            aux = lista[i];
            lista[i]=lista[j];
            lista[j]=aux;

            l2 = lista2[i];  // alterando a lista2 correspondente a posi��o
            lista2[i] = lista2[j];
            lista2[j]=l2;

            aux2=mat[i][0];
            aux3=mat[i][1];
            mat[i][0]=mat[j][0];
            mat[i][1]=mat[j][1];
            mat[j][0]=aux2;
            mat[j][1]=aux3;
            i=i+1;
            j=j-1;
        }
    }
    if(j>iniVet)
    {
        quickSort2(lista,lista2,iniVet,j,mat);
    }
    if(i<fimVet)
    {
        quickSort2(lista,lista2,i,fimVet,mat);
    }
}

/*
/ cliqueGulosaRandom(grafo, inf do grafo, percentual passado, quantidade de vezes a ser rodada)
/ Fun��o que acha a clique usando a heur�stica gulosa randomizada
*/
void cliqueGulosaRandom(no *grafo, info *inf, int per, int qnt)
{
    srand(time(NULL));

    int p[inf->numNo];
    int posicao[inf->numNo];
    int g[inf->numNo];
    int graus[inf->numNo];

    no *aux = grafo->prox;
    int tam=0,i,k;


    while(aux!=NULL)
    {
        g[tam]=aux->grau;
        p[tam]=tam;
        tam++;
        aux=aux->prox;
    }
    int mat[tam][2];
    quickSort2(g,p,0,(tam-1),mat); //ordena nos de acordo com o grau

    //inverte o vetor para ordem decrescente
    k=tam-1;
    for(i=0;i<tam;i++)
    {
        posicao[k]=p[i];
        graus[k]=g[i];
        k--;
    }
    for(i=0;i<tam;i++) // 'g' e 'p' ser�o uma c�pia original de 'posicao' e 'graus'
    {
        p[i]=posicao[i];
        g[i]=graus[i];
    }

    int vetClique[graus[0]+1];
    int vetCliqueMax[graus[0]+1];
    inicializaVet(vetClique,graus[0]+1);
    inicializaVet(vetCliqueMax,graus[0]+1);

    int novoTam=tam;
    int h,maxC=0;
    int j=0;
    int vez=0;
    int r,r2;

    int vetP[tam];
    int vetG[tam];
    for(i=0;i<tam;i++)
    {
        vetP[i]=-1;
        vetG[i]=-1;
    }
    for(h=0; h<qnt; h++)  // definir numero de vezes a ser testada
    {
        //acha a clique
        while(novoTam>0)
        {
            r2 = per*novoTam;
            r2 = (int)r2/100;
            if((r2)<=per) //testa se tamanho do vetor � menor que, o tamanho, dividido pela porcentagem
            {
                r=0;          //se for, ent�o pega o primeiro valor
            }
            else
            {
                r=rand()%(r2);   //sorteia valor aleat�rio entre 0 e 'r2'
            }

            k=0;
            vetClique[j]=posicao[r];

            aux = buscaNo2(grafo,posicao[r]);
            for(i=0;i<novoTam;i++)
            {
                if(adjacente(grafo,aux->id,buscaNo2(grafo,posicao[i])->id))
                {
                    vetG[k] = graus[i];
                    vetP[k] = posicao[i];
                    k++;
                }
            }
            novoTam = k;
            for(i=0;i<tam;i++)
            {
                posicao[i] = vetP[i];
                graus[i] = vetG[i];
            }
            for(i=0;i<tam;i++)
            {
                vetP[i] = -1;
                vetG[i] = -1;
            }

            j++;
        }
        //atualiza maior valor encontrado
        if(j>maxC)
        {
            vez++;
            maxC = j;
            for(i=0;i<j;i++)
            {
                vetCliqueMax[i]=vetClique[i]; //atualiza vetCliqueMax com a maior clique
            }
        }

        for(i=0;i<tam;i++)
        {
            posicao[i] = p[i];
            graus[i] = g[i];
        }
        inicializaVet(vetClique,graus[0]+1);
        novoTam=tam;
        j=0;

    }//termina for

    int si;
    printf("\nencontrado e melhorado %d vezes. Maior valor: %d\n", vez, maxC);
    printf("\nDeseja imprimir os nos da maior clique? 1-sim, 2-nao");
    scanf("%d", &si);
    if(si==1)
    {
        for(i=0;i<maxC;i++)
        {
            printf("%d\n", buscaNo2(grafo,vetCliqueMax[i])->id);
        }
    }
}

/*
/ cliqueReativa(grafo, info do grafo, quantidade de vezes a ser atualizada a lista de probabilidades)
/ fun��o que calcular uma cliquei usando a heur�stica reativa
*/
void cliqueReativa(no *grafo, info *inf, int qnt)
{
    srand(time(NULL));

    int p[inf->numNo];
    int posicao[inf->numNo];
    int g[inf->numNo];
    int graus[inf->numNo];

    no *aux = grafo->prox;
    int tam=0,i,k;


    while(aux!=NULL)
    {
        g[tam]=aux->grau;
        p[tam]=tam;
        tam++;
        aux=aux->prox;
    }
    int mat[tam][2];
    quickSort2(g,p,0,(tam-1),mat); //ordena nos de acordo com o grau

    //inverte o vetor para ordem decrescente
    k=tam-1;
    for(i=0;i<tam;i++)
    {
        posicao[k]=p[i];
        graus[k]=g[i];
        k--;
    }
    for(i=0;i<tam;i++) // 'g' e 'p' ser�o uma c�pia original de 'posicao' e 'graus'
    {
        p[i]=posicao[i];
        g[i]=graus[i];
    }

    int novoTam=tam;
    int h, maxC=0, v, val, c ,a;
    int j=0;
    int r,r2;
    float real,cont;

        //////////// inicializa vetor de alphas *************************************************
    int vetAlpha[5];
    vetAlpha[0] = 10;
    vetAlpha[1] = 20;
    vetAlpha[2] = 30;
    vetAlpha[3] = 40;
    vetAlpha[4] = 50;

    /////////// inicializa vetor de prob
    float vetProb[5];
    for(i=0;i<5;i++)
    {
        vetProb[i]=20.0;
    }

    int vetCount[5];
    int bestValue[5];
    float vetMedia[5];
    float vetQ[5];
    int vetSumCost[5]; //soma dos custos das cliques

    int vetP[tam];
    int vetG[tam];

    for(c=0;c<qnt;c++) //loop para atualizar vetProb
    {
        inicializaVet(vetCount,5);
        inicializaVet(bestValue,5);
        inicializaVetReal(vetMedia,5);
        inicializaVetReal(vetQ,5);
        inicializaVet(vetSumCost,5); //soma do maior valor, de todas as cliques

        for(h=0; h<100; h++)  // trocando alphas
        {
            v=0; //posi��o do alpha escolhido
            cont = 0.0;
            val = rand()%(100); //sorteia valor de alpha
            val = val+1;
            while(val>cont)  //acha a posi��o do vetAlpha de acordo com vetProb
            {
                cont = cont + vetProb[v];
                v++;
            }
            v=v-1;
            vetCount[v] = vetCount[v]+ 5;
            //printf("\nposi vet: %d", v);

            for(a=0; a<10; a++)  // definir numero de vezes a ser testada
            {
                j=0;
                for(i=0;i<tam;i++)
                {
                    vetP[i]=-1;
                    vetG[i]=-1;
                }

                //acha a clique
                while(novoTam>0)
                {
                    r2 = vetAlpha[v]*novoTam;
                    r2 = (int)r2/100;
                    if(r2<1) //testa se tamanho do vetor � menor que, o tamanho, dividido pela porcentagem
                    {
                        r=0;          //se for, ent�o pega o primeiro valor
                    }
                    else
                    {
                        r=rand()%(r2);   //sorteia valor aleat�rio entre 0 e 'r2'
                    }

                    k=0;

                    aux = buscaNo2(grafo,posicao[r]);
                    for(i=0;i<novoTam;i++)
                    {
                        if(adjacente(grafo,aux->id,buscaNo2(grafo,posicao[i])->id))
                        {
                            vetG[k] = graus[i];
                            vetP[k] = posicao[i];
                            k++;
                        }
                    }
                    novoTam = k;
                    for(i=0;i<tam;i++)
                    {
                        posicao[i] = vetP[i];
                        graus[i] = vetG[i];
                    }
                    for(i=0;i<tam;i++)
                    {
                        vetP[i] = -1;
                        vetG[i] = -1;
                    }

                    j++;
                }//termina 1 clique

                vetSumCost[v] = vetSumCost[v] + j;
                //atualiza maior valor encontrado
                if(j>bestValue[v])
                {
                    bestValue[v] = j;
                    maxC = j;
                }

                for(i=0;i<tam;i++)
                {
                    posicao[i] = p[i];
                    graus[i] = g[i];
                }

                novoTam=tam;
                j=0;
            }//termina 1 alpha

            real=(float)vetSumCost[v]/(a+1);

            if(vetMedia[v]!=0)
            {
                vetMedia[v] =(vetMedia[v]+real);
            }
            else
            {
                vetMedia[v] = real;
            }
            //printf("\nsumcost: %d - cont: %d", vetSumCost[v], vetCount[v]);
        }// termina troca de alphas

        //atualiza vetQ
        for(i=0;i<5;i++)
        {
            if(vetCount[i]!=0)
            {
                vetMedia[i] =(float)vetSumCost[i]/vetCount[i];
                real = (float)bestValue[i]/vetMedia[i];
            }
            else{
                vetMedia[i]=0.0;
                real=0.0;
            }
            vetQ[i] = real;
        }

        real=0.0;
        for(i=0;i<5;i++)
        {
            real = real + vetQ[i];
        }
        for(i=0;i<5;i++)
        {
            vetProb[i]=(float)(vetProb[i]+(float)((vetQ[i]/real)*100))/2;
        }

    }//termina loop de atualiza��o de alphas
    printf("\n Maior valor da clique encontrado: %d", maxC);
    printf("\n");
    for(i=0;i<5;i++)
    {
        printf("\n alpha: %d - probabilidade: %f", vetAlpha[i], vetProb[i]);
    }
}

/*
/ inicializaVetReal(vetor, tamanho do vetor)
/ inicializa uma lista de valores reais com 0.0
*/
void inicializaVetReal(float *vet, int tam)
{
    int i;
    for(i=0;i<tam;i++)
    {
        vet[i]=0.0;
    }
}

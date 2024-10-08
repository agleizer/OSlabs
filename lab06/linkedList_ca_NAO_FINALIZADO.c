#include <stdio.h>
#include <stdlib.h>

//cd "C:\Users\caiof\Documentos disco local\aaaComputacao_faculdade\sistemas operacionais\lab6 malloc" && gcc -o linkedList linkedList.c && linkedList

struct reg {
    int conteudo;
    struct reg *prox;
};
typedef struct reg celula;

//-------- 2 --------//
void imprimirValores(celula *cabeca){
    celula *atual = cabeca;
    while(atual != NULL){
        printf("%d ", atual->conteudo);
        atual = atual->prox;
    }
    printf("\n");
}

void main(){

    //-------- 1 --------//
    celula *cabeca = (celula*) malloc(sizeof(celula));
    celula *meio = (celula*) malloc(sizeof(celula));
    celula *rabo = (celula*) malloc(sizeof(celula));

    cabeca->conteudo = 10;
    cabeca->prox = meio;

    meio->conteudo = 14;
    meio->prox = rabo;

    rabo->conteudo = 21;
    rabo->prox = NULL;

    //-------- 2 --------//
    imprimirValores(cabeca);

    //-------- 3 --------//
    int quantMemoGasta = sizeof(celula);
    printf("A quantidade de memoria gasta por intancia da celula eh: %d", quantMemoGasta);

}
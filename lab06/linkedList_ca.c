// Lab 06 Alocacao dinamica de memoria
// Alan Gleizer - 10416804
// Caio Corsini - 10342005
#include <stdio.h>
#include <stdlib.h>

// O struct da celula (node)
struct reg {
    int conteudo;
    struct reg *prox;
};
typedef struct reg celula;

//-------- 2 --------//
// Funcao que imprime toda a linked list do head ate o tail
void imprimirValores(celula *cabeca){
    celula *atual = cabeca;
    while(atual != NULL){
        printf("%d ", atual->conteudo);
        atual = atual->prox;
    }
    printf("\n");
}

//-------- 4 --------//
// Funcao que remove todos os nos da linked list do head ate o tail
void removerCelulas(celula *cabeca){
    celula *atual = cabeca;
    while(atual != NULL){
        celula *aux = atual->prox;
        free(atual);
        atual = aux;
    }
}

void main(){

    //-------- 1 --------//
    // Criando os tres nodes para simular manualmente
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

    //-------- 4 --------//
    removerCelulas(cabeca);

}
#include <stdio.h>
#include <stdlib.h>  // tem malloc
#include <string.h>  // tem memset
#include "estruturasMemoria.h"

// Construtor
Fila *criarFila(){
    Fila *filaCriada = malloc(sizeof(Fila));
    if (!filaCriada) return NULL;

    // Incializa tudo com zero
    filaCriada -> primeiro = 0;
    filaCriada -> ultimo = 0;
    filaCriada -> contagem = 0;
    memset(filaCriada -> dados, 0, sizeof(pagina) * NUM_PAGINAS);  // Aloca memoria baseado no tipo de variavel, neste caso, pagina
    return filaCriada;
}

// Verifica se fila esta cheia
bool estaCheio(Fila *fila) {
    return fila->contagem == NUM_PAGINAS;  // Compara contagem com max
}

// Verifica se fila esta vazia
bool estaVazio(Fila *fila) {
    return fila->contagem == 0;
}

void enqueue(Fila *fila, pagina novaPagina){
    if(!fila || estaCheio(fila)) return;  // Caso fila esteja cheia

    int indiceUltimo = fila->ultimo;
    fila->dados[indiceUltimo] = novaPagina;  // Insere novo valor no final da fila
    fila->ultimo = (indiceUltimo+1) % NUM_PAGINAS;  // Atualiza o indice do ultimo. "Da a volta" se necessario
    ++fila->contagem;  // Incrementa contagem da fila
}

pagina dequeue(Fila *fila){
    if (!fila || estaVazio(fila)) return (pagina){0, 0, NULL};  // Caso fila esteja vazia

    pagina valor = fila->dados[fila->primeiro];  // Variavel para guardar a pagina de retorno
    fila->primeiro = (fila->primeiro + 1) % NUM_PAGINAS;  // Atualiza o indice do primeiro. "Da a volta" se necessario
    --fila->contagem;  // Descrementa a contagem da fila
    return valor;  // Retorna a pagina removida da fila
}


void imprimirFila(Fila *fila) {
    if (!fila || estaVazio(fila)) {
        printf("A fila está vazia.\n");
        return;
    }

    int i;
    printf("Elementos na fila:\n");
    for (i = 0; i < fila->contagem; i++) {
        // Calculate the index of the current element
        int indiceAtual = (fila->primeiro + i) % NUM_PAGINAS;
        printf("ID: %d\n", fila->dados[indiceAtual].id);  // Print the id of the pagina
    }
}
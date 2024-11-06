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
    memset(filaCriada -> dados, 0, sizeof(frame) * NUM_FRAMES);  // Aloca memoria baseado no tipo de variavel, neste caso, frame
    return filaCriada;
}

// Verifica se fila esta cheia
bool estaCheio(Fila *fila) {
    return fila->contagem == NUM_FRAMES;  // Compara contagem com max
}

// Verifica se fila esta vazia
bool estaVazio(Fila *fila) {
    return fila->contagem == 0;
}

void enqueue(Fila *fila, frame novoframe){
    if(!fila || estaCheio(fila)) return;  // Caso fila esteja cheia

    int indiceUltimo = fila->ultimo;
    fila->dados[indiceUltimo] = novoframe;  // Insere novo valor no final da fila
    fila->ultimo = (indiceUltimo+1) % NUM_FRAMES;  // Atualiza o indice do ultimo. "Da a volta" se necessario
    ++fila->contagem;  // Incrementa contagem da fila
}

frame dequeue(Fila *fila){
    if (!fila || estaVazio(fila)) return (frame){0, 0, NULL};  // Caso fila esteja vazia

    frame valor = fila->dados[fila->primeiro];  // Variavel para guardar a frame de retorno
    fila->primeiro = (fila->primeiro + 1) % NUM_FRAMES;  // Atualiza o indice do primeiro. "Da a volta" se necessario
    --fila->contagem;  // Descrementa a contagem da fila
    return valor;  // Retorna a frame removida da fila
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
        int indiceAtual = (fila->primeiro + i) % NUM_FRAMES;
        printf("ID: %d\n", fila->dados[indiceAtual].id);  // Print the id of the frame
    }
}

// Apesar de ter a estrutura de uma fila, sera possivel acessar elementos no meio dela para propositos deste projeto
frame pegarDado(Fila *fila, int i){
    int indice = (fila->primeiro + i) % NUM_FRAMES;
    return fila->dados[indice];
}
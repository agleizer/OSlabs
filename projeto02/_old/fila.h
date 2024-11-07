#ifndef fila
#define fila

#include <stdbool.h>
#include "estruturasMemoria.h"

Fila *criarFila();                     
bool estaCheio(Fila *fila);            
bool estaVazio(Fila *fila);            
void enqueue(Fila *fila, frame novaPagina); 
frame dequeue(Fila *fila);
void imprimirFila(Fila *fila);
frame pegarDado(Fila *fila, int i);

#endif 
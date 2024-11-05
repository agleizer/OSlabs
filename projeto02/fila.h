#ifndef fila
#define fila

#include <stdbool.h>
#include "estruturasMemoria.h"

Fila *criarFila();                     
bool estaCheio(Fila *fila);            
bool estaVazio(Fila *fila);            
void enqueue(Fila *fila, pagina novaPagina); 
pagina dequeue(Fila *fila);
void imprimirFila(Fila *fila);      

#endif 
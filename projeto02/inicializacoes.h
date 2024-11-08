#ifndef INICIALIZACOES_H
#define INICIALIZACOES_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // para rand
#include "estruturasMemoria.h"
#include "utils.h"

void inicializarMemoFisica(frame memoriaFisica[], int NUM_FRAMES, int TAMANHO_FRAME);
void inicializarMemoVirtual(pagina memoriaVirtual[], int NUM_PAGINAS, int TAMANHO_PAGINA);
void inicializarTabela(linhaTabelaDePaginas linhas_tabela[], int NUM_PAGINAS_PROC);
int inicializarProcesso(processo *proc, int pid, int NUM_PAGINAS_PROC, int TAMANHO_PAGINA);

#endif
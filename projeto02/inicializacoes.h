#ifndef INICIALIZACOES_H
#define INICIALIZACOES_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // para rand
#include "estruturasMemoria.h"
#include "utils.h"

void inicializarMemoFisica(frame memoriaFisica[]);
void inicializarMemoVirtual(pagina memoriaVirtual[]);
void inicializarTabela(linhaTabelaDePaginas linhas_tabela[]);
int inicializarProcesso(processo *proc, int pid);

#endif
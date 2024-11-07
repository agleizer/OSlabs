#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // para rand
#include "estruturasMemoria.h"
#include "inicializacoes.h"
#include "utils.h"

// ---------- FUNÇÕES DE GERENCIAMENTO DA TABELA DE PAGINAS ----------

// retorna o indice de um endereço virtual no espaço de endereçamento do processo
int buscarIndicePorEnderecoVirtual(linhaTabelaDePaginas tabelaPaginas[], int end_pagina);

// ---------- FUNÇÕES DE MAPEAMENTO ----------

int traduzirEndVirtualParaPagina(int endereco);

int verificarFrameDePagina(int indicePagina, processo *proc);

// procura por um frame livre na mem. física, retorna o indíce ou -1
int buscarFrameLivre(frame memoriaFisica[]);

// alocar um frame da memória física para uma página de um processo
int alocarFrame(frame memoriaFisica[], processo *proc, int end_pagina);

int indiceFrameParaIndicePagina(processo proc, int indiceFrame);

// desalocar um frame da memória física
int desalocarFrame(frame memoriaFisica[], processo processos[]);

void liberarMemoriaProcesso(processo *proc);

void liberarMemoriaFisica(frame memoriaFisica[]);

#endif
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
int buscarIndicePorEnderecoVirtual(linhaTabelaDePaginas tabelaPaginas[], int end_pagina, int NUM_PAGINAS);

// ---------- FUNÇÕES DE MAPEAMENTO ----------

int traduzirEndVirtualParaPagina(int endereco, int TAMANHO_PAGINA);

int verificarFrameDePagina(int indicePagina, processo *proc);

// procura por um frame livre na mem. física, retorna o indíce ou -1
int buscarFrameLivre(frame memoriaFisica[], int NUM_FRAMES);

// alocar um frame da memória física para uma página de um processo
int alocarFrame(frame memoriaFisica[], processo *proc, int end_pagina, int NUM_FRAMES, int TAMANHO_PAGINA);

int indiceFrameParaIndicePagina(processo proc, int indiceFrame, int NUM_PAGINAS_PROC);

// desalocar um frame da memória física
int desalocarFrame(frame memoriaFisica[], processo processos[], int NUM_PAGINAS_PROC, int NUM_FRAMES);

void liberarMemoriaProcesso(processo *proc, int NUM_PAGINAS_PROC);

void liberarMemoriaFisica(frame memoriaFisica[], int NUM_FRAMES);

#endif
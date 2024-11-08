#ifndef UTILS_H
#define UTILS_H

void imprimirHelp();

bool carregarConfig(char *NOME_CONFIG, int *TAMANHO_FRAME, int *TAMANHO_PAGINA, int *NUM_FRAMES,
                    int *NUM_PAGINAS, int *NUM_PAGINAS_PROC, int *QTD_PROCESSOS, int *QTD_ACESSOS,
                    int *DELAY_MEM_SEC, char *NOME_LOG);

int gerarNumeroAleatorio(int max);
// IMPRESSOES

#include <stdio.h>

void imprimirMemoriaFisica(frame frames[], FILE *arquivoLog, int NUM_FRAMES);

void printArray(int arr[], int size, FILE *arquivoLog);

void printProcessos(processo processos[], int size, FILE *arquivoLog);

void pausa(int milisegundos);

#endif
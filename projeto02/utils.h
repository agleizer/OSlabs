#ifndef UTILS_H
#define UTILS_H

int gerarNumeroAleatorio(int max);
// IMPRESSOES

#include <stdio.h>

void imprimirMemoriaFisica(frame frames[], FILE *arquivoLog);

void printArray(int arr[], int size, FILE *arquivoLog);

void printProcessos(processo processos[], int size, FILE *arquivoLog);

#endif
/*
1. Crie um programa capaz de ler os dados de uma matriz quadrada de inteiros.
Ao final da leitura o programa deverá imprimir o número da linha que contém o menor dentre todos os números lidos.
*/

#include <stdio.h>

int main() {
    // determinacao tamanho
    int tam;
    do {
    printf("Informe o tamanho da matriz desejada: ");
    scanf("%d", &tam);

    if (tam <= 0) printf("Tamanho inválido!\n");
    } while (tam <= 0);

    // leitura matriz
    int matriz[tam][tam];
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            printf("Informe o valor de matriz[%d][%d]", i+1, j+1);
            scanf("%d", &matriz[i][j]);
        }
    }

    // determinacao linha com menor valor
    int menorValor = matriz[0][0];
    int linhaMenorValor = 0;

    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (matriz[i][j] < menorValor) {
                menorValor = matriz[i][j];
                linhaMenorValor = i;
            }
        }
    }

    // print resultados
    printf("A linha com o menor valor é %d.\n", linhaMenorValor + 1);

    return 0;
}
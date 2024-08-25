/*
3. Faça o programa que apresenta a seguinte saída, perguntando ao usuário o número máximo (no exemplo, 9).
Este número deve ser sempre ímpar.
1 2 3 4 5 6 7 8 9
  2 3 4 5 6 7 8
    3 4 5 6 7
      4 5 6
        5   
*/

#include <stdio.h>
#include <math.h>

void imprimirLinha(int vetor[], int indiceInicial, int indiceFinal) {
  // imprimir espaços
  if (indiceInicial != 0) {
    for (int i = (indiceInicial) * 2; i > 0; i--) {
      printf(" ");
    }
  }

  // imprimir valores
  for (int i = indiceInicial; i <= indiceFinal; i++) {
    printf("%d ", vetor[i]);
  }
  printf("\n");
}

int main() {

  // determinacao numero maximo
  int numMax;
  do {
  printf("Informe o numero maximo (ímpar!): ");
  scanf("%d", &numMax);
  if (numMax < 1 || numMax % 2 == 0) printf("Numero invalido!\n");
  } while (numMax < 1 || numMax % 2 == 0);


  // criacao vetor
  int vetor[numMax];
  for (int i = 0; i < numMax; i++) {
    vetor[i] = i+1;
  }

  // impressao formatada
  int numeroLinhas = (int)(ceil(numMax / 2.0));

  for (int i = 0; i < numeroLinhas; i++) {
    imprimirLinha(vetor, i, numMax - i - 1);
    }
  }
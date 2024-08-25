#include <stdio.h>
#include <stdlib.h>

void main() {
  int multiplicador, qualLinha, qualColuna;
  
  printf("Digite numero: ");
  scanf("%d", &multiplicador);

  printf("\nAVISO: (comeca contando de 1!!!)\n");

  printf("Qual linha: ");
  scanf("%d", &qualLinha);

  printf("Qual coluna: ");
  scanf("%d", &qualColuna);
  
  int matriz[3][3] = { {1, 4, 2}, {3, 6, 8}, {10, 7, 0}};

  printf("\nAntes: \n");
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
    printf("%d ", matriz[i][j]);
    }
    printf("\n");
  }

  printf("\nDepois(linha): \n");
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      if(i+1==qualLinha) printf("%d ", matriz[i][j] * multiplicador);
      else printf("%d ", matriz[i][j]);
    }
    printf("\n");
  }

  printf("\nDepois(coluna): \n");
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      if(j+1==qualColuna) printf("%d ", matriz[i][j] * multiplicador);
      else printf("%d ", matriz[i][j]);
    }
    printf("\n");
  }
}
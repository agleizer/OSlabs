//Exercicio 1 - Caio Corsini
#include <stdio.h>

void main(void) {
  int exemploMatriz[2][2] = { {1, 4}, {3, 0} };

  int menor = exemploMatriz[0][0];
  int linhaDoMenor = 0;
  for(int i=0; i<2; i++){
    for(int j=0; j<2; j++){
      if(menor > exemploMatriz[i][j]){
        menor = exemploMatriz[i][j];
        linhaDoMenor = i;
        }
    }
  }
  printf("Linha do menor elemento: %d", linhaDoMenor);
}
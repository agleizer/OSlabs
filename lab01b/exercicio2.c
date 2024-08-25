//Exercicio 2 - Caio Corsini
#include <stdio.h>
#include <stdlib.h>

void main() {
  float N1, N2, N3, ME, MA;
  
  printf("Nota 1: ");
  scanf("%f", &N1);

  printf("Nota 2: ");
  scanf("%f", &N2);

  printf("Nota 3: ");
  scanf("%f", &N3);

  printf("Media dos exercicios: ");
  scanf("%f", &ME);

  MA = (N1+N2*2+N3*3+ME)/7;

  printf("Media final: %.2f\n", MA);
  if(MA>=9) printf("A");
  else if(MA>=7.5 && MA<9) printf("B");
  else if(MA>=6 && MA<7.5) printf("C");
  else if(MA>=4 && MA<6) printf("D");
  else if(MA<4) printf("E");
}
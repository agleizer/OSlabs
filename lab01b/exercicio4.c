//exercicio 4 - Caio Corsini
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main() {
  char nome1[50];
  char nome2[50];

  char nome1lower[50];
  char nome2lower[50];
  
  printf("Nome 1: ");
  scanf("%s", nome1);

  printf("Nome 2: ");
  scanf("%s", nome2);

  for(int i=0; i<50; i++){
    nome1lower[i] = tolower(nome1[i]);
    nome2lower[i] = tolower(nome2[i]);
}
  
  int aux = strcmp(nome1lower,nome2lower);

  if(aux <= 0) printf("%s | %s", nome1,nome2);
  else printf("%s | %s", nome2,nome1);
}
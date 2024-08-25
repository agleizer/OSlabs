//exercicio 3 - Caio Corsini
#include <stdio.h>
#include <stdlib.h>

void main() {
  int n = 2;
  printf("Digite numero: ");
  scanf("%d", &n);
  while(n%2 == 0){
    printf("Digite numero novamente. Tem que ser impar: ");
    scanf("%d", &n);
  }

  int espacoE = -1;
  int espacoD = n;

  while(espacoE<espacoD){
    for(int i=0; i<n; i++){
      if(espacoE>=i) printf("  ");
      else if(i>=espacoD) printf("  ");
      else printf("%d ",i+1);
    }
    printf("\n");
    espacoE++;
    espacoD--;
  }
}
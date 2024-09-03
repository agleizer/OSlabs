#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
int iOpcao;
char cParametro1[10] = "main.c";
char cParametro2[10] = "main.old";
char cParametro3[10] = "main.new";

do {
	printf("\n\n\t * * Lab0501 * * \n");
	printf("\n\t 1 - listar ");
	printf("\n\t 2 - copiar ");
	printf("\n\t 3 - mover  ");
	printf("\n\t 9 - Finalizar");
	printf("\n\n\tDigite sua opcao ");
	scanf("%d", &iOpcao);
	switch (iOpcao)
	{
	case 1:
        execl("/bin/ls", "ls", "-la", 0);
	break;
	case 2:
         execl("/bin/cp", "cp",cParametro1, cParametro2, 0);
	break;
	case 3:
        execl("/bin/mv", "mv" , cParametro2, cParametro3, 0);
	break;
	case 9:

	break;
	default:
	    printf("\n\n\tOpcao invalida ");
	}

}while (iOpcao != 9);


return 0;
}
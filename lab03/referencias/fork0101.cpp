#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
 int pid;

 pid = fork();

 if (pid < 0) {
       printf ("\n\n Erro fork \n\n");
        exit(1);
     }

if  (pid == 0)
     printf ("\nFilho: \t id is %d , pid (valor) is %d \n", getpid(), pid);
else
     printf ("\nPAI: \t is is %d , pid (filho) is %d \n", getpid(), pid);

system("date");

return 0;
} 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
 int pid;
 fork();
 fork();
 fork();
 printf ("\nPai: \t id is %d , pai do pai is %d \n", getpid(), getppid());

return 0;
} 









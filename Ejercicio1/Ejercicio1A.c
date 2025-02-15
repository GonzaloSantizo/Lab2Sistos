#include <stdio.h>
#include <unistd.h>
int main()
{
    printf("Inicio del programa\n");
    fork();
    fork();
    fork();
    fork();
    printf("%d\n", getpid());
    return 0;
}
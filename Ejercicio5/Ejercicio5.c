#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <x> <y>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *x = argv[1];
    char *y = argv[2];
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Proceso hijo
        execl("./ipc", "./ipc", "2", x, NULL); // Ejecuta ipc con n=2 y x
        perror("execl");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Proceso padre
        pid_t pid2 = fork();
        if (pid2 < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid2 == 0)
        {
            // Segundo proceso hijo
            execl("./ipc", "./ipc", "3", y, NULL); // Ejecuta ipc con n=3 e y
            perror("execl");
            exit(EXIT_FAILURE);
        }
    }
    wait(NULL); // Espera a que ambos procesos hijos terminen
    wait(NULL);
    exit(EXIT_SUCCESS);
}
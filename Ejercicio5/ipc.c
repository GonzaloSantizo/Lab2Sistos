#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#define SHM_SIZE 1024
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <n> <x>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    char x = argv[2][0];
    int shmid;
    key_t key = ftok("/tmp", 12345); // Genera una clave única
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    int *shmaddr = (int *)shmat(shmid, NULL, 0); // Attach a la memoria compartida
    if (shmaddr == (int *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    int fd_pipe[2];
    pipe(fd_pipe); // Crea un pipe
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Proceso hijo
        close(fd_pipe[1]); // Cierra el extremo de escritura del pipe
        int shmid_parent;
        read(fd_pipe[0], &shmid_parent, sizeof(int));                                                  // Lee el file descriptor de
        la memoria compartida desde el pipe int *shmaddr_parent = (int *)shmat(shmid_parent, NULL, 0); // Attach a la
        memoria compartida del padre if (shmaddr_parent == (int *)-1)
        {
            perror("shmat (hijo)");
            exit(EXIT_FAILURE);
        }
        while (*shmaddr_parent == 0)
        {
            sleep(1); // Espera a que el proceso padre inicie el ciclo
        }
        for (int i = 0; i < SHM_SIZE; i++)
        {
            if (i % n == 0)
            {
                *shmaddr_parent = x; // Escribe x en la memoria compartida cuando i
                es divisible por n
            }
        }
        shmdt(shmaddr_parent); // Desattach de la memoria compartida del padre
        close(fd_pipe[0]);     // Cierra el extremo de lectura del pipe
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Proceso padre
        close(fd_pipe[0]);                      // Cierra el extremo de lectura del pipe
        write(fd_pipe[1], &shmid, sizeof(int)); // Escribe el file descriptor de la
        memoria compartida en el pipe for (int i = 0; i < SHM_SIZE; i++)
        {
            if (i % n == 0)
            {
                *shmaddr = x; // Escribe x en la memoria compartida cuando i es
                divisible por n
            }
        }
        wait(NULL); // Espera a que el proceso hijo termine
        for (int i = 0; i < SHM_SIZE; i++)
        {
            printf("%c", *(shmaddr + i)); // Despliega el contenido de la memoria
            compartida
        }
        printf("\n");
        shmdt(shmaddr);                // Desattach de la memoria compartida
        shmctl(shmid, IPC_RMID, NULL); // Borra la memoria compartida
        close(fd_pipe[1]);             // Cierra el extremo de escritura del pipe
        exit(EXIT_SUCCESS);
    }
}
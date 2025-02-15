#include <stdio.h>
#include <time.h>
int main()
{
    clock_t start, end;
    start = clock(); // Capturamos el tiempo de inicio
    // Primer ciclo for
    for (int i = 0; i < 1000000; i++)
    {
        // No se realiza ninguna operación en este ciclo
    }
    // Segundo ciclo for
    for (int i = 0; i < 1000000; i++)
    {
        // No se realiza ninguna operación en este ciclo
    }
    // Tercer ciclo for
    for (int i = 0; i < 1000000; i++)
    {
        // No se realiza ninguna operación en este ciclo
    }
    end = clock(); // Capturamos el tiempo de finalización
    // Resta entre las variables start y end, y almacenamiento en una variable
    double double tiempo_total = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tiempo total en segundos (almacenado en una variable double): %f\n",
           tiempo_total);
    return 0;
}
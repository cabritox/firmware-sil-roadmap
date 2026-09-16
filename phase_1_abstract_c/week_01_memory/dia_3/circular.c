#include <stdio.h>
#include <stdint.h>
#include <unistd.h> /* Para la función sleep() */

#define TAMANO_BUFFER 4

int main(void)
{
    /* 1. Nuestro anillo de memoria físico en la SRAM */
    uint16_t sensor_buffer[TAMANO_BUFFER] = {0, 0, 0, 0};

    /* 2. El índice que controlará dónde escribimos (nuestro puntero lógico) */
    uint8_t indice_escritura = 0;

    printf("=== INICIANDO CAPTURA INFINITA DE SENSOR ===\n\n");

    /* 3. Simulamos un flujo infinito de datos llegando (un bucle que corre 10 veces) */
    for (uint16_t dato_entrante = 100; dato_entrante < 110; dato_entrante++)
    {

        /* LA MAGIA CIRCULAR: Forzamos el wrap-around matemático */
        uint8_t posicion_real = indice_escritura % TAMANO_BUFFER;

        /* Escribimos el voltaje en la memoria */
        sensor_buffer[posicion_real] = dato_entrante;

        /* Imprimimos el estado físico del anillo */
        printf("Dato %d guardado en indice [%d] -> Estado del Buffer: ", dato_entrante, posicion_real);
        printf("[ %d | %d | %d | %d ]\n",
                sensor_buffer[0], sensor_buffer[1],
                sensor_buffer[2], sensor_buffer[3]);

        /* Movemos el índice hacia adelante */
        indice_escritura++;

        /* Pequeña pausa para simular hardware real (1 segundo) */
        sleep(1);
    }

    return 0;
}
#include <stdio.h>
#include <stdint.h>

/* 1. Función para leer memoria usando aritmética pura */
void imprimir_trama(uint8_t *datos, uint8_t longitud)
{
    for (uint8_t i = 0; i < longitud; i++)
    {
        /* (datos + i) calcula la coordenada. El *() abre la memoria y extrae el valor */
        printf("Direccion: %p | Dato: 0x%X\n", (void *)(datos + i), *(datos + i));
    }
}

/* 2. Función para escribir/modificar memoria de forma remota */
void limpiar_ruido(uint8_t *datos, uint8_t longitud)
{
    printf("--- Aplicando filtro anti-ruido in-situ ---\n");
    for (uint8_t i = 0; i < longitud; i++)
    {
        /* Si el dato supera un umbral de ruido (ej. 0xAA) */
        if (*(datos + i) > 0xAA)
        {
            /* Disparamos el láser a la coordenada remota y sobrescribimos con ceros */
            *(datos + i) = 0x00;
        }
    }
}

int main(void)
{
    /* Nuestro búfer local en el Stack de main(). Simulamos datos corruptos (0xFF y 0xEE) */
    uint8_t trama_giroscopio[4] = {0x12, 0xFF, 0x34, 0xEE};

    printf("=== TRAMA ORIGINAL ===\n");
    /* Al pasar 'trama_giroscopio', C automáticamente entrega la dirección del primer byte */
    imprimir_trama(trama_giroscopio, 4);

    /* La función viaja a la memoria de main() y altera los voltajes físicos */
    limpiar_ruido(trama_giroscopio, 4);

    printf("\n=== TRAMA LIMPIA ===\n");
    imprimir_trama(trama_giroscopio, 4);

    return 0;
}
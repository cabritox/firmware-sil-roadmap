#include <stdio.h>
#include <stdint.h>

int main(void)
{
    /* 1. Creamos dos arreglos en el Stack de diferentes tamaños físicos */
    uint32_t buffer_pesado[3] = {0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC};
    uint8_t buffer_ligero[3] = {0xAA, 0xBB, 0xCC};

    /* 2. Punteros apuntando al inicio (índice 0) de cada búfer */
    uint32_t *ptr_32 = buffer_pesado; /* El nombre del arreglo ya es un puntero a la base */
    uint8_t *ptr_8 = buffer_ligero;

    printf("=== TOPOLOGIA DE SALTOS (Aritmetica de Punteros) ===\n\n");

    /* Iteracion 32-bits (Saltos de 4 bytes) */
    printf("--- Analisis uint32_t (4 bytes) ---\n");
    for (int i = 0; i < 3; i++)
    {
        /* (ptr_32 + i) genera la direccion.
           El asterisco exterior *() hace la "desreferencia": va a esa direccion y lee el valor. */
        printf("Desplazamiento +%d | Direccion: %p | Dato leido: 0x%X\n",
               i, (void *)(ptr_32 + i), *(ptr_32 + i));
    }

    /* Iteracion 8-bits (Saltos de 1 byte) */
    printf("\n--- Analisis uint8_t (1 byte) ---\n");
    for (int i = 0; i < 3; i++)
    {
        printf("Desplazamiento +%d | Direccion: %p | Dato leido: 0x%X\n",
               i, (void *)(ptr_8 + i), *(ptr_8 + i));
    }

    return 0;
}
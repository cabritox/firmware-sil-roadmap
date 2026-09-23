#include <stdio.h>
#include <stdint.h>

/* Función auxiliar (Paso 1): Imprime los 8 bits de un uint8_t */
void imprimir_binario(const char *mensaje, uint8_t byte)
{
    printf("%s: \t", mensaje); /* El \t es un tabulador para alinear el texto */
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (byte >> i) & 1);
    }
    printf("\n");
}

int main(void)
{
    /* 1. Estado inicial: Todos los interruptores apagados */
    uint8_t registro = 0b00000000;
    imprimir_binario("1. Estado Inicial", registro);

    /* 2. Misión OR (|): Encender Bit 0 y Bit 1
     * Usamos 0b00000011. Donde haya un 1, se encenderá. Donde haya un 0, se queda igual. */
    registro = registro | 0b00000011;
    imprimir_binario("2. OR  (Enciende 0 y 1)", registro);

    /* 3. Misión AND (&): Apagar SOLO el Bit 0
     * Usamos 0b11111110. Los 1s actúan como un escudo (dejan el valor intacto).
     * El 0 actúa como un hoyo negro que apaga el Bit 0. */
    registro = registro & 0b11111110;
    imprimir_binario("3. AND (Apaga el bit 0)", registro);

    /* 4. Misión XOR (^): Alternar el Bit 7 (Primer toggle)
     * Usamos 0b10000000. Donde haya un 1, el estado se invierte. Los 0s no hacen nada. */
    registro = registro ^ 0b10000000;
    imprimir_binario("4. XOR (Alterna Bit 7 on)", registro);

    /* 5. Misión XOR (^): Alternar el Bit 7 (Segundo toggle)
     * Volvemos a aplicar exactamente la misma máscara. Como ya estaba encendido, se apagará. */
    registro = registro ^ 0b10000000;
    imprimir_binario("5. XOR (Alterna Bit 7 off)", registro);

    return 0;
}
#include <stdio.h>
#include <stdint.h>

void imprimir_binario(const char *mensaje, uint8_t byte)
{
    printf("%s: \t", mensaje);
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (byte >> i) & 1);
    }
    printf("\n");
}

int main(void)
{
    uint8_t registro = 0;
    imprimir_binario("Estado Inicial", registro);

    /* 1. Misión OR Dinámica: Encender el Bit 4 */
    registro = registro | (1 << 4);
    imprimir_binario("Encender Bit 4", registro);

    /* 2. Misión AND Dinámica: Apagar el Bit 4
    * Usamos el inversor ~ para voltear la máscara antes de aplicar el AND */
    registro = registro & ~(1 << 4);
    imprimir_binario("Apagar Bit 4", registro);

    /* 3. Misión XOR Dinámica: Alternar el Bit 6 */
    registro = registro ^ (1 << 6);
    imprimir_binario("Alternar Bit 6", registro);

    return 0;
}
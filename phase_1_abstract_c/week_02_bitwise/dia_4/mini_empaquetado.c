#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/* Nuestra herramienta de rayos X para ver los 8 bits */
void imprimir_binario(const char *etiqueta, uint8_t valor)
{
    printf("%-28s: ", etiqueta);
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (valor >> i) & 1);
        if (i == 4)
            printf(" | "); /* Separador visual entre el carril izquierdo y derecho */
    }
    printf("  (Hex: 0x%02X)\n", valor);
}

int main(void)
{
    uint8_t id_celda = 3; /* 0000 | 0011 */
    uint8_t alerta = 5;   /* 0000 | 0101 */

    printf("--- 1. DATOS CRUDOS POR SEPARADO ---\n");
    imprimir_binario("id_celda original", id_celda);
    imprimir_binario("alerta original", alerta);

    printf("\n--- 2. PREPARACIÓN DE CARRILES ---\n");
    /* Paso A: Sanitizamos ambos con 0x0F (0000 1111) para asegurar que no midan más de 4 bits */
    uint8_t id_limpio = id_celda & 0x0FU;
    uint8_t alerta_limpia = alerta & 0x0FU;

    /* Paso B: Empujamos 'alerta' 4 espacios a la izquierda para llevarla a su carril [7..4] */
    uint8_t alerta_en_carril = alerta_limpia << 4;

    imprimir_binario("id_limpio (carril der)", id_limpio);
    imprimir_binario("alerta movida (carril izq)", alerta_en_carril);

    printf("\n--- 3. FUSIÓN EN UN SOLO BYTE ---\n");
    /* TODO: Usa el operador OR (|) para unir 'alerta_en_carril' con 'id_limpio' */
    uint8_t paquete_final = alerta_en_carril | id_limpio; /* <-- Reemplaza este 0 con tu operación OR */

    imprimir_binario("PAQUETE EMPAQUETADO", paquete_final);

    /* El juez verifica que en la izquierda haya un 5 y en la derecha un 3 (0x53) */
    assert(paquete_final == 0x53);
    printf("\n✅ ¡Mini-empaquetado completado con éxito!\n");

    return 0;
}
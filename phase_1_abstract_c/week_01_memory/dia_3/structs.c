#include <stdio.h>
#include <stdint.h>

/* Definimos el "molde" físico de nuestro paquete de datos */
struct Telemetria
{
    uint8_t id_sensor;    /* 1 byte (ej. ID del giroscopio) */
    uint32_t timestamp;   /* 4 bytes (Tiempo en milisegundos) */
    uint16_t temperatura; /* 2 bytes (Medición térmica) */
};

int main(void)
{
    /* Instanciamos el bloque en la memoria Stack */
    struct Telemetria paquete;

    printf("=== AUDITORIA DE MEMORIA STRUCT ===\n");

    /* Matemáticamente: 1 + 4 + 2 = 7 bytes */
    printf("Bytes logicos esperados : 7 bytes\n");

    /* Le preguntamos al procesador cuánto espacio físico asignó realmente */
    printf("Bytes fisicos en SRAM   : %zu bytes\n", sizeof(paquete));

    /* Imprimimos las direcciones de memoria para ver cómo se apilaron */
    printf("\n--- Mapa Hexadecimal ---\n");
    printf("Direccion id_sensor   : %p\n", (void *)&paquete.id_sensor);
    printf("Direccion timestamp   : %p\n", (void *)&paquete.timestamp);
    printf("Direccion temperatura : %p\n", (void *)&paquete.temperatura);

    return 0;
}
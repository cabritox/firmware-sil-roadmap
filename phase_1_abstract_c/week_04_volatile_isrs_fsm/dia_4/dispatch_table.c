#include <stdio.h>
#include <stdint.h>

/* 1. Funciones manejadoras (Handlers) para cada accion del hardware */
void bms_reportar_estado(void) { printf("[BMS] Estado: 100%% Bateria, Temp: 35C\n"); }
void bms_activar_rele(void) { printf("[BMS] Relé PRINCIPAL ACTIVADO.\n"); }
void bms_apagar_rele(void) { printf("[BMS] Relé PRINCIPAL APAGADO.\n"); }
void bms_error_comando(void) { printf("[ALERTA] Comando DESCONOCIDO o Invalido.\n"); }

/* 2. Definimos un alias (typedef) para el puntero a funcion.
      Esto hace que el codigo sea infinitamente mas facil de leer. */
typedef void (*ComandoHandler_t)(void);

int main(void)
{
    /* 3. LA TABLA DE DESPACHO (Arreglo de punteros a funciones) */
    /* Indice 0 = Estado, Indice 1 = Activar, Indice 2 = Apagar */
    ComandoHandler_t tabla_comandos[3] = {
        bms_reportar_estado, /* Indice 0 */
        bms_activar_rele,    /* Indice 1 */
        bms_apagar_rele      /* Indice 2 */
    };

    printf("=== Consola UART BMS ===\n");

    /* 4. Simulamos 4 bytes que acaban de llegar por el puerto Serie */
    uint8_t bytes_recibidos[] = {0, 2, 1, 99}; // El 99 es basura/ruido electrico

    for (int i = 0; i < 4; i++)
    {
        uint8_t comando = bytes_recibidos[i];
        printf("\n> Procesando byte: %d\n", comando);

        /* 5. MAGIA O(1): Ejecucion sin switch-case ni if-else */
        /* REGLA DE ORO: Siempre validar los limites del arreglo (Boundary check)
           antes de saltar en memoria, o el microcontrolador crasheara. */

        if (comando < 3)
        {
            tabla_comandos[comando](); /* Salto directo en memoria ejecutable */
        }
        else
        {
            bms_error_comando(); /* Manejo seguro de hardware */
        }
    }

    return 0;
}
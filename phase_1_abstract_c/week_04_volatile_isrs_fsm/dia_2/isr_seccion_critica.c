#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Macros para simular el control de interrupciones del hardware (cli / sei en AVR) */
#define DISABLE_INTERRUPTS() printf("[CPU] Interrupciones deshabilitadas (Inicio seccion critica)\n")
#define ENABLE_INTERRUPTS() printf("[CPU] Interrupciones habilitadas (Fin seccion critica)\n")

/* 1. Variables compartidas (Siempre volatile) */
volatile bool flag_nuevo_dato = false;
volatile uint32_t odometro_global = 0; /* Dato de 32 bits: vulnerable en micros de 8/16 bits */

/*
 * 2. Simulacion de la ISR (Rutina de Servicio de Interrupcion)
 * En hardware real, esta funcion la llama el procesador, no tu codigo.
 */
void ISR_Sensor_Velocidad(void)
{
    odometro_global += 150;
    flag_nuevo_dato = true;
}

int main(void)
{
    uint32_t odometro_local = 0;

    printf("=== Sistema de Telemetria Iniciado ===\n\n");

    /* Simulamos que el hardware externo dispara la interrupcion asincronamente */
    ISR_Sensor_Velocidad();

    /* 3. El main revisa la bandera periodicamente (Polling) */
    if (flag_nuevo_dato)
    {

        /* SECCION CRITICA: Bloqueamos al hardware para hacer una copia segura */
        DISABLE_INTERRUPTS();

        odometro_local = odometro_global; /* Copia atomica a variable local */
        flag_nuevo_dato = false;          /* Bajamos la bandera antes de reanudar el hardware */

        ENABLE_INTERRUPTS();
        /* Fin de la seccion critica */

        /* 4. Procesamiento pesado FUERA de la seccion critica */
        printf("Procesando odometro actualizado: %u metros\n", odometro_local);
    }

    return 0;
}